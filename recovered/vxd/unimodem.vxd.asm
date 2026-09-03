; UNIMODEM -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 11705 instructions recovered by recursive descent

Declare_Virtual_Device UNIMODEM, 1, 0, UNIMODEM_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xd98  R-X
UNIMODEM_Control:
        cmp    eax, 0x1b
        je     loc_3_0000
        clc    
        ret    
loc_1_000B:
        mov    dword ptr [edx + 0x4f4], 0
        mov    eax, 1
        xchg   dword ptr [edx + 0x624], eax
        or     eax, eax
        jne    loc_1_0053
        cmp    dword ptr [edx + 0x620], 0
        jne    loc_1_004B
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, 0x92                     ; -> loc_3_0092
        mov    ecx, 0xb
        sub    eax, eax
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [edx + 0x620], esi
loc_1_004B:
        xor    eax, eax
        mov    dword ptr [edx + 0x624], eax
loc_1_0053:
        ret    
loc_1_0054:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, 1
        xchg   dword ptr [edx + 0x624], eax
        or     eax, eax
        jne    loc_1_009A
        cmp    dword ptr [edx + 0x620], 0
        jne    loc_1_0092
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, 0x92                     ; -> loc_3_0092
        mov    ecx, 0xb
        sub    eax, eax
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [edx + 0x620], esi
loc_1_0092:
        xor    eax, eax
        mov    dword ptr [edx + 0x624], eax
loc_1_009A:
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_009E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        cmp    dword ptr [edx + 0x2e0], 7
        je     loc_1_00B8
        cmp    dword ptr [edx + 0x2e0], 0xb
        jne    loc_1_010E
loc_1_00B8:
        mov    eax, dword ptr [ebp + 0xc]
        or     dword ptr [edx + 0x5b2], eax
        mov    eax, dword ptr [edx + 0x29c]
        mov    eax, dword ptr [eax + 0x34]
        or     dword ptr [edx + 0x60c], eax
        mov    eax, 1
        xchg   dword ptr [edx + 0x624], eax
        or     eax, eax
        jne    loc_1_010E
        cmp    dword ptr [edx + 0x620], 0
        jne    loc_1_0106
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, 0x9a                     ; -> loc_3_009A
        mov    ecx, 0xb
        sub    eax, eax
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [edx + 0x620], esi
loc_1_0106:
        xor    eax, eax
        mov    dword ptr [edx + 0x624], eax
loc_1_010E:
        pop    edi
        pop    esi
        leave  
        ret    
UNIMODEM_PM_API:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 0x19
        jae    loc_1_015E
        mov    cx, word ptr [ebp + 0x38]
        or     cx, word ptr [ebp + 0x38]
        je     loc_1_0133
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
loc_1_0133:
        mov    esi, dword ptr [eax*4 + 0x5e8] ; -> obj1:0x05E8
        cmp    esi, 0x7341                   ; -> sub_3_7341
        jne    loc_1_014E
        push   eax
        push   edi
        call   esi
        add    esp, 8
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_1_014E:
        push   edi
        call   dword ptr [eax*4 + 0x5e8]     ; -> obj1:0x05E8
        add    esp, 4
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_1_015E:
        mov    word ptr [ebp + 0x1c], 2
        ret    
        db  000h, 000h, 000h
loc_1_0168:
        mov    ecx, 0x14
        jmp    loc_1_035B
sub_1_0172:
        push   ebp
        mov    ebp, esp
        mov    esi, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 0x10]
        cmp    edx, 4
        jne    loc_1_01DF
        mov    eax, dword ptr [ebp + 0x14]
        test   dword ptr [esi + 0x5ba], eax
        je     loc_1_0197
        push   esi
        push   eax
        push   esi
        call   sub_1_009E
        add    esp, 8
        pop    esi
loc_1_0197:
        test   dword ptr [ebp + 0x14], 4
        je     loc_1_01AA
        mov    dword ptr [esi + 0x5f6], 1
loc_1_01AA:
        test   dword ptr [esi + 0x2c4], 4
        je     loc_1_0226
        mov    eax, dword ptr [ebp + 0x14]
        test   dword ptr [esi + 0x5b6], eax
        je     loc_1_0226
        cmp    dword ptr [esi + 0x2bc], 0
        je     loc_1_0226
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [esi + 0x2c0]
        push   esi
        call   dword ptr [esi + 0x2bc]
        jmp    loc_1_0223
loc_1_01DF:
        cmp    edx, 1
        jne    loc_1_0202
        cmp    dword ptr [esi + 0x2cc], 0
        je     loc_1_0226
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [esi + 0x2c8]
        push   esi
        call   dword ptr [esi + 0x2cc]
        jmp    loc_1_0223
loc_1_0202:
        cmp    edx, 2
        jne    loc_1_0226
        cmp    dword ptr [esi + 0x2d8], 0
        je     loc_1_0226
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [esi + 0x2d4]
        push   esi
        call   dword ptr [esi + 0x2d8]
loc_1_0223:
        add    esp, 0x10
loc_1_0226:
        leave  
        ret    
loc_1_0228:
        push   dword ptr [esp + 8]
        call   sub_3_34E7
        add    esp, 4
        mov    ecx, 0
        jmp    sub_1_033F
loc_1_023E:
        mov    ecx, 8
        jmp    loc_1_035B
loc_1_0248:
        mov    ecx, 0xc
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        call   sub_1_033F
        add    esp, 8
        or     eax, eax
        je     loc_1_026D
        push   esi
        mov    esi, dword ptr [esp + 8]
        inc    dword ptr [esi + 0x2b4]
        pop    esi
loc_1_026D:
        ret    
loc_1_026E:
        mov    ecx, 0x18
        jmp    sub_1_033F
loc_1_0278:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 0xeffffff1
        jl     loc_1_02A0
        cmp    eax, 0xeffffff6
        jg     loc_1_02A0
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_3_3B9C
        add    esp, 0xc
        pop    ebp
        ret    
loc_1_02A0:
        cmp    eax, 0xc0000000
        jl     loc_1_02C4
        cmp    eax, 0xc0000005
        jg     loc_1_02C4
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_3_7E08
        add    esp, 0xc
        pop    ebp
        ret    
loc_1_02C4:
        mov    ecx, 0x24
        jmp    loc_1_035B
loc_1_02CE:
        mov    ecx, 0x30
        jmp    sub_1_033F
loc_1_02D5:
        mov    ecx, 0x34
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        call   sub_1_033F
        add    esp, 0x10
        mov    ecx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0
        jle    loc_1_0309
        push   esi
        mov    esi, dword ptr [esp + 8]
        add    dword ptr [esi + 0x2b4], ecx
        pop    esi
loc_1_0309:
        ret    
loc_1_030A:
        mov    ecx, 0x38
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        call   sub_1_033F
        add    esp, 0x10
        mov    ecx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0
        jle    loc_1_033E
        push   esi
        mov    esi, dword ptr [esp + 8]
        add    dword ptr [esi + 0x2b0], ecx
        pop    esi
loc_1_033E:
        ret    
sub_1_033F:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x2e0], 7
        je     loc_1_035B
        cmp    dword ptr [eax + 0x2e0], 0x1a
        je     loc_1_035B
        mov    eax, 0
        ret    
loc_1_035B:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [0x6ac], eax        ; -> obj1:0x06AC
        mov    eax, dword ptr [eax + 0x29c]
        or     eax, eax
        je     loc_1_0380
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     loc_1_0380
        mov    ecx, dword ptr [ecx + eax]
        jecxz  loc_1_0380
        jmp    ecx
loc_1_0380:
        mov    eax, dword ptr [0x6ac]        ; -> obj1:0x06AC
        mov    dword ptr [eax + 0xc], 0xfffffffa
        xor    eax, eax
        ret    
        db  000h
loc_1_0390:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0xc], 0xb
        cmp    dword ptr [eax + 0x18], 0
        jne    loc_1_03D9
        call   sub_1_04E8
        mov    dword ptr [ebp - 0x10], eax
        pushal 
        mov    ebx, dword ptr [ebp - 0x10]
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp - 4]
        lea    esi, [0x660f]                 ; -> sub_3_660F
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [ebp - 8], esi
        popal  
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0x18], eax
loc_1_03D9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_03E0:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_1_03FE
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_1_03FE:
        ret    0x10
        db  000h, 000h, 000h
sub_1_0404:
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
        db  000h
sub_1_0418:
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e801700
        add    byte ptr [eax], al
sub_1_0420:
        VxDCall  SHELL._SHELL_CancelAppyTimeEvent ; cd200f801700
        add    byte ptr [eax], al
sub_1_0428:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        add    byte ptr [eax], al
sub_1_0430:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        add    byte ptr [eax], al
sub_1_0438:
        VMMCall  VMM._RegCreateKey               ; cd204a810100
        add    byte ptr [eax], al
sub_1_0440:
        VMMCall  VMM._RegDeleteKey               ; cd204b810100
        add    byte ptr [eax], al
sub_1_0448:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        add    byte ptr [eax], al
sub_1_0450:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        add    byte ptr [eax], al
sub_1_0458:
        VxDCall  BIOS.Service_8005h              ; cd2005802b00
        add    byte ptr [eax], al
sub_1_0460:
        VxDCall  BIOS.Service_8006h              ; cd2006802b00
        add    byte ptr [eax], al
sub_1_0468:
        VxDCall  BIOS.Service_800Ah              ; cd200a802b00
        add    byte ptr [eax], al
sub_1_0470:
        VxDCall  BIOS.Service_800Bh              ; cd200b802b00
        add    byte ptr [eax], al
sub_1_0478:
        VxDCall  BIOS.Service_800Dh              ; cd200d802b00
        add    byte ptr [eax], al
sub_1_0480:
        VxDCall  BIOS.Service_800Eh              ; cd200e802b00
        add    byte ptr [eax], al
sub_1_0488:
        VxDCall  BIOS.Service_800Fh              ; cd200f802b00
        add    byte ptr [eax], al
sub_1_0490:
        VxDCall  BIOS.Service_8010h              ; cd2010802b00
        add    byte ptr [eax], al
sub_1_0498:
        VxDCall  BIOS.Service_8011h              ; cd2011802b00
        add    byte ptr [eax], al
sub_1_04A0:
        VxDCall  BIOS.Service_8012h              ; cd2012802b00
        add    byte ptr [eax], al
sub_1_04A8:
        VxDCall  BIOS.Service_8013h              ; cd2013802b00
        add    byte ptr [eax], al
sub_1_04B0:
        VxDCall  BIOS.Service_8015h              ; cd2015802b00
        add    byte ptr [eax], al
sub_1_04B8:
        VxDCall  BIOS.Service_8017h              ; cd2017802b00
        add    byte ptr [eax], al
sub_1_04C0:
        VxDCall  BIOS.Service_8018h              ; cd2018802b00
        add    byte ptr [eax], al
sub_1_04C8:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_1_04D0:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_1_04D8:
        VMMCall  VMM._HeapReAllocate             ; cd2050800100
        add    byte ptr [eax], al
sub_1_04E0:
        VMMCall  VMM._HeapGetSize                ; cd2052800100
        add    byte ptr [eax], al
sub_1_04E8:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  000h
sub_1_04F4:
        pop    edx
        pop    ecx
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  000h
sub_1_0504:
        pop    edx
        pop    eax
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        jmp    edx
        db  000h, 000h
sub_1_0510:
        pop    edx
        pop    eax
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        jmp    edx
        db  000h, 000h
sub_1_051C:
        pop    edx
        pop    eax
        pop    ecx
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        jmp    edx
        db  000h
sub_1_0528:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  000h, 000h, 000h
sub_1_0544:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  000h
sub_1_0554:
        VMMCall  VMM.Get_Exec_Path               ; cd20b6000100
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], ecx
        mov    eax, edx
        ret    4
        db  000h, 000h, 000h
sub_1_0568:
        VMMCall  VMM._BlockOnID                  ; cd20fa800100
        add    byte ptr [eax], al
sub_1_0570:
        VMMCall  VMM._SignalID                   ; cd2027810100
        add    byte ptr [eax], al
sub_1_0578:
        VxDCall  Dev_049D.Service_8003h          ; cd2003809d04
        add    byte ptr [eax], al
sub_1_0580:
        VxDCall  Dev_049D.Service_8002h          ; cd2002809d04
        add    byte ptr [eax], al
sub_1_0588:
        VxDCall  Dev_049D.Service_8001h          ; cd2001809d04
        add    byte ptr [eax], al
sub_1_0590:
        VxDCall  Dev_049D.Service_8004h          ; cd2004809d04
        add    byte ptr [eax], al
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'UNIMODEM'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  UNIMODEM_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  UNIMODEM_PM_API          ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        dd  loc_3_0700               ; fixup
        dd  sub_3_0823               ; fixup
        dd  loc_3_0987               ; fixup
        dd  loc_3_0A50               ; fixup
        dd  sub_3_0C35               ; fixup
        dd  sub_3_1034               ; fixup
        dd  loc_3_1194               ; fixup
        dd  loc_3_11BD               ; fixup
        dd  loc_3_68B0               ; fixup
        dd  loc_3_68D5               ; fixup
        dd  loc_3_7128               ; fixup
        dd  loc_3_71DD               ; fixup
        dd  loc_3_6D35               ; fixup
        dd  loc_3_6E5C               ; fixup
        dd  loc_3_6EFF               ; fixup
        dd  loc_3_7026               ; fixup
        dd  loc_3_7292               ; fixup
        dd  sub_3_7341               ; fixup
        dd  loc_3_696C               ; fixup
        dd  sub_3_7341               ; fixup
        dd  sub_3_7341               ; fixup
        dd  loc_3_6A4C               ; fixup
        dd  loc_3_6CB3               ; fixup
        dd  loc_3_68FC               ; fixup
        dd  loc_3_691B               ; fixup
        db  000h, 000h, 000h, 000h
loc_1_0650:
        sub    byte ptr [edx], al
        add    byte ptr [eax], al
        pop    es
        add    al, 0
        add    byte ptr [esi], bh            ; -> loc_1_023E
        add    al, byte ptr [eax]
        add    byte ptr [eax + 2], cl        ; -> loc_1_0248
        add    byte ptr [eax], al
        xor    al, 3
        add    byte ptr [eax], al
        push   0x6e000001                    ; -> loc_1_026E
        add    al, byte ptr [eax]
        add    byte ptr [ecx], dl            ; -> loc_3_0411
        add    al, 0
        add    byte ptr [eax], ah            ; -> loc_3_5220
        push   edx
        add    byte ptr [eax], al
        js     loc_1_0678
        add    byte ptr [eax], al
loc_1_0678:
        sub    dword ptr [eax + eax], eax
        add    byte ptr [ebx], dh            ; -> loc_3_0433
        add    al, 0
        add    dh, cl                        ; -> loc_1_02CE
        add    al, byte ptr [eax]
        add    ch, dl                        ; -> loc_1_02D5
        add    al, byte ptr [eax]
        add    byte ptr [edx], cl            ; -> loc_1_030A
        add    eax, dword ptr [eax]
        add    byte ptr [esi + 4], bl        ; -> loc_3_045E
        add    byte ptr [eax], al
        mov    ebx, 0x40000004               ; -> loc_3_0540
        add    eax, 0x5c50000                ; -> loc_3_05C5
        add    byte ptr [eax], al
        sti    
        push   ebx
        add    byte ptr [eax], al
        int3   
        push   esp
        add    byte ptr [eax], al
        iretd  
        db  005h, 000h, 000h
        dd  loc_3_05D9               ; fixup
        db  000h, 000h, 000h, 000h
        dd  sub_3_0137               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_06C0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        push   ecx
        VxDCall  BIOS.Service_0020h              ; cd2020002b00
        pop    ecx
        pop    ebp
        ret    
sub_1_06D3:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        push   ecx
        VxDCall  BIOS.Service_001Fh              ; cd201f002b00
        pop    ecx
        pop    ebp
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_06F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   0
        push   0
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VxDCall  SHELL._SHELL_PostMessage        ; cd2006001700
        add    esp, 0x18
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_1_071B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        shr    eax, 0x10
        mov    dword ptr [ebp - 0xc], eax
        call   sub_1_04E8
        mov    dword ptr [ebp - 8], eax
        push   ebx
        push   0
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        pop    ebx
        and    esi, 0xffff
        add    esi, dword ptr [ebp - 4]
        mov    eax, esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_075F:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0
        ret    4
sub_1_076C:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx], 0xffffffff
        mov    dword ptr [ecx + 4], eax
        push   0x204c5242
        call   sub_1_0570
        add    esp, 4
        ret    8
sub_1_078D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        call   sub_1_04E8
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [eax + 0x2c]
        mov    dword ptr [ebp - 8], eax
        test   ah, 2
        jne    loc_1_07BC
        int3   
        mov    ecx, dword ptr [ebp - 8]
        or     ecx, 0x200
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x2c], ecx
loc_1_07BC:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 0
        jne    loc_1_07D8
loc_1_07C4:
        push   1
        push   0x204c5242
        call   sub_1_0568
        add    esp, 8
        cmp    dword ptr [esi], 0
        je     loc_1_07C4
loc_1_07D8:
        mov    eax, dword ptr [esi + 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_07E4:
        push   ebp
        mov    ebp, esp
        push   ecx
        pushfd 
        mov    eax, dword ptr [ebp + 8]
        VxDCall  NDIS.Service_000Eh              ; cd200e002a00
        popfd  
        pop    ecx
        pop    ebp
        ret    
sub_1_07F6:
        push   ebp
        mov    ebp, esp
        pushfd 
        mov    eax, dword ptr [ebp + 8]
        VxDCall  NDIS.Service_0014h              ; cd2014002a00
        popfd  
        pop    ebp
        ret    
        db  000h, 000h
loc_1_0808:
        dec    ebp
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [edi + ebp*2 + 0x72], 0x63696f56
        push   esi
        imul   esp, dword ptr [ebp + 0x77], 0x6e4f
loc_1_081C:
        dec    ebp
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [edi + ebp*2 + 0x72], 0x63696f56
        push   esi
        imul   esp, dword ptr [ebp + 0x77], 0x66664f
loc_1_0830:
        dec    ebp
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [edi + ebp*2 + 0x72], 0x616e4500
        bound  ebp, qword ptr [ebp + 0x43]
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        jb     loc_1_088E
        inc    esp
        add    byte ptr [eax], al
loc_1_0848:
        inc    ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x44]
        imul   esi, dword ptr [ebx + 0x74], 0x74636e69
        imul   esi, dword ptr [esi + 0x65], 0x676e6952
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x6e], al
        jae    0x8db
        jb     loc_1_0867
loc_1_0867:
        add    byte ptr [ecx + 0x6e], cl
        imul   esi, dword ptr [eax + eax], 0x6f4e0000
        push   edx
        jae    loc_1_08E6
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jae    loc_1_08DF
        add    byte ptr [eax], al
loc_1_087C:
        dec    esi
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_0884:
        sub    ebp, dword ptr [ebx]
        sub    eax, dword ptr [eax]
loc_1_0888:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 0x35
loc_1_088E:
        aaa    
        cmp    byte ptr [ecx], bh
        popal  
        bound  esp, qword ptr [ebx + 0x64]
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_089C:
        dec    esi
        dec    edi
        inc    ebp
        dec    ebp
        push   ebx
        add    byte ptr [eax], al
        db  000h
loc_1_08A4:
        inc    edx
        insb   byte ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x64], 0x6e4f5f
        add    byte ptr [eax], al
        db  000h
loc_1_08B0:
        inc    edx
        insb   byte ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x64], 0x66664f5f
        add    byte ptr [eax], al
        add    byte ptr [ecx + ebp*2 + 0x61], al
        insb   byte ptr es:[edi], dx
        push   eax
        jb     0x928
        imul   di, word ptr [eax], 0x4400
        imul   esp, dword ptr [ecx + 0x6c], 0x66667553
        imul   edi, dword ptr [eax], 0x6e6f5400
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_08DC:
        push   eax
        jne    0x94b
loc_1_08DF:
        jae    loc_1_0946
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x6f], dl
loc_1_08E6:
        imul   esp, dword ptr [ebx + 0x65], 0x61446f54
        je     loc_1_0950
        inc    ecx
        outsb  dx, byte ptr [esi]
        jae    0x96a
        jb     loc_1_08F6
loc_1_08F6:
        add    byte ptr [eax], al
loc_1_08F8:
        or     eax, 0xd000000
        add    byte ptr [eax], al
        add    byte ptr [0xd000000], cl
        add    byte ptr [eax], al
        add    byte ptr [ebx], bh
        jb     0x94a
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0910:
        cmp    al, 0x6c
        add    byte ptr ds:[eax], al
        add    byte ptr [eax], al
loc_1_0918:
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_091C:
        xor    dword ptr [esi], ebp
        xor    eax, 0x3200
        add    byte ptr [eax], bl            ; -> loc_1_0918
        or     dword ptr [eax], eax
        add    byte ptr [ecx + ecx], bl      ; -> loc_1_091C
        add    byte ptr [eax], al
        and    byte ptr [ecx], cl
        add    byte ptr [eax], al
loc_1_0930:
        and    eax, 0x63252c64
        sub    al, 0x25
        sub    al, 0x25
        jae    loc_1_0946
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0940:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0944:
        add    byte ptr [eax], al
loc_1_0946:
        add    byte ptr [eax], al
loc_1_0948:
        push   edx
        jae    loc_1_09BC
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jae    loc_1_09B5
loc_1_0950:
        jae    loc_1_0952
loc_1_0952:
        add    byte ptr [eax], al
loc_1_0954:
        inc    ecx
        je     0x9cb
        popal  
        arpl   word ptr [eax + 0x65], bp
        push   esp
        outsd  dx, dword ptr [esi]
        add    byte ptr [eax], al
loc_1_0960:
        dec    edi
        jbe    0x9c8
        jb     0x9d7
        imul   esp, dword ptr [ebp], 0x50000000
        outsd  dx, dword ptr [esi]
        jb     loc_1_09E4
        dec    esi
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_0978:
        push   eax
        jb     0x9ea
        jo     loc_1_09E2
        jb     loc_1_09F3
        imul   esp, dword ptr [ebp + 0x73], 0x65440000
        popaw  
        jne    loc_1_09F6
        je     loc_1_098C
loc_1_098C:
        inc    esp
        inc    ebx
        inc    edx
        add    byte ptr [edi + ebp*2 + 0x67], cl
        imul   ebp, dword ptr [bp + 0x67], 0x676f4c00
        imul   ebp, dword ptr [bp + 0x67], 0x68746150
        add    byte ptr [edx + esi*2 + 0x69], al
        jbe    0xa0e
        jb     0x9ef
        jae    loc_1_0A11
        add    byte ptr [eax], al
loc_1_09B0:
        dec    ecx
        outsb  dx, byte ptr [esi]
        push   ax
        popal  
loc_1_09B5:
        je     loc_1_0A1F
        db  000h
loc_1_09B8:
        dec    ecx
        outsb  dx, byte ptr [esi]
        push   bx
loc_1_09BC:
        arpl   word ptr gs:[ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_09C4:
        dec    ebp
        popal  
        outsb  dx, byte ptr [esi]
        jne    loc_1_0A2F
        popal  
        arpl   word ptr [ebp + esi*2 + 0x72], si
        jb     loc_1_09D1
loc_1_09D1:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x6f], cl
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x65], dl
        jae    loc_1_0A45
        je     loc_1_09E2
loc_1_09E2:
        add    byte ptr [eax], al
loc_1_09E4:
        inc    esp
        jbe    0xa51
        arpl   word ptr [ebp + 0x54], sp
        jns    0xa5d
        add    byte ptr gs:[eax], al
loc_1_09F0:
        dec    eax
        popal  
        outsb  dx, byte ptr [esi]
loc_1_09F3:
        jne    loc_1_0A66
loc_1_09F6:
        add    byte ptr [eax], al
loc_1_09F8:
        dec    ecx
        outsb  dx, byte ptr [esi]
        popal  
        arpl   word ptr [ecx + ebp*2 + 0x76], si
        imul   esi, dword ptr [ecx + edi*2 + 0x53], 0x656c6163
        add    byte ptr [esi + 0x61], dl
        jb     loc_1_0A75
        popal  
        bound  ebp, qword ptr [ebp + 0x54]
loc_1_0A11:
        jb     0xa81
        imul   ebp, dword ptr [esi + 0x61], 0x726f74
        add    byte ptr [ebx + 0x61], al
        insb   byte ptr es:[edi], dx
loc_1_0A1F:
        insb   byte ptr es:[edi], dx
        jb     loc_1_0A6C
        inc    esp
        push   eax
        jb     loc_1_0A90
        jbe    loc_1_0A8A
        je     loc_1_0A90
        add    byte ptr [ebx + 0x61], al
        insb   byte ptr es:[edi], dx
loc_1_0A2F:
        insb   byte ptr es:[edi], dx
        jb     0xa7c
        inc    esp
        dec    edi
        jne    loc_1_0AAB
        jae    0xaa2
        add    byte ptr gs:[esi + 0x6f], dl
        imul   esp, dword ptr [ebx + 0x65], 0x666f7250
loc_1_0A45:
        imul   ebp, dword ptr [ebp], 0x53000000
        jo     0xab4
        popal  
        imul   esp, dword ptr [ebp + 0x72], 0x50
        push   0x53656e6f
        jo     0xac0
        arpl   word ptr [ebx], si
        add    byte ptr [eax], al
loc_1_0A60:
        dec    eax
        popal  
        outsb  dx, byte ptr [esi]
        jae    loc_1_0ACB
loc_1_0A66:
        je     loc_1_0AAB
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jae    loc_1_0AD1
loc_1_0A6C:
        inc    esp
        insb   byte ptr es:[edi], dx
        popal  
        jns    loc_1_0A72
loc_1_0A72:
        add    byte ptr [eax], al
loc_1_0A74:
        push   esp
loc_1_0A75:
        jb     0xae5
        imul   ebp, dword ptr [esi + 0x61], 0x6c506574
        popal  
        jns    loc_1_0A82
loc_1_0A82:
        add    byte ptr [eax], al
loc_1_0A84:
        inc    ecx
        bound  ebp, qword ptr [edi + 0x72]
        je     0xada
loc_1_0A8A:
        insb   byte ptr es:[edi], dx
        popal  
        jns    loc_1_0A8E
loc_1_0A8E:
        add    byte ptr [eax], al
loc_1_0A90:
        push   esp
        jb     loc_1_0B01
        imul   ebp, dword ptr [esi + 0x61], 0x65526574
        arpl   word ptr [edi + 0x72], bp
        add    byte ptr fs:[esp + ebp*2 + 0x65], al
        push   esp
        popal  
        bound  ebp, qword ptr [ebp]
        add    byte ptr [eax], al
loc_1_0AAB:
        add    byte ptr [ebx + 0x65], dl
        je     0xb24
        imul   ebp, dword ptr [esi + 0x67], 0x73
        db  000h
loc_1_0AB8:
        push   ebx
        je     loc_1_0B30
        imul   ebp, dword ptr [esi + 0x67], 0x6e495c73
        imul   esi, dword ptr [eax + eax], 0x65725000
loc_1_0ACB:
        imul   di, word ptr [eax], 0x5400
loc_1_0AD1:
        jb     loc_1_0B41
        imul   ebp, dword ptr [esi + 0x61], 0x726f74
        add    byte ptr [ebx + 0x61], al
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        push   ebx
        je     loc_1_0B59
        jo     0xb2c
        popal  
        imul   ebp, dword ptr [esp + edx*2 + 0x69], 0x72656d
        add    byte ptr [ecx + 0x6e], cl
        popal  
        arpl   word ptr [ecx + ebp*2 + 0x76], si
        imul   esi, dword ptr [ecx + edi*2 + 0x54], 0x6f656d69
        jne    0xb75
loc_1_0B01:
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x70], dl
        popal  
        imul   esp, dword ptr [ebp + 0x72], 0x56
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        jne    0xb7d
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 0x70], dl
        popal  
        imul   esp, dword ptr [ebp + 0x72], 0x4d
        outsd  dx, dword ptr [esi]
        add    byte ptr gs:[esi + 0x6c], al
        outsd  dx, dword ptr [esi]
        ja     loc_1_0B68
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0xb9b
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        add    byte ptr [ebp + 0x72], al
        jb     0xb9f
loc_1_0B30:
        jb     0xb75
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0xba8
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0B3C:
        inc    ebx
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     0xbb3
loc_1_0B41:
        jae    0xbb7
        imul   ebp, dword ptr [edi + 0x6e], 0x646f4d00
        jne    loc_1_0BB9
        popal  
        je     loc_1_0BB9
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_0B54:
        pop    edi
        inc    ebx
        inc    ebx
        dec    ecx
        push   esp
loc_1_0B59:
        push   esp
        add    byte ptr [eax], al
loc_1_0B5C:
        pop    edi
        inc    edx
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x43], bl
        inc    ebx
        dec    ecx
loc_1_0B68:
        push   esp
        push   esp
        pop    edi
        push   esi
        xor    dh, byte ptr [ebx]
        add    byte ptr [eax], al
loc_1_0B70:
        push   ebx
        jo     loc_1_0BD8
        dec    esi
        outsd  dx, dword ptr gs:[si]
        je     0xbe4
        popal  
        je     0xbe7
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0B84:
        pop    edi
        dec    esp
        outsd  dx, dword ptr [esi]
        ja     loc_1_0B89
loc_1_0B89:
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x4d], bl
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
loc_1_0B94:
        pop    edi
        dec    eax
        imul   esp, dword ptr [edi + 0x68], 0x5f000000
        inc    esp
        imul   esp, dword ptr [ecx + 0x6c], 0x5f000000
        push   ebx
        je     0xc1e
        jo     loc_1_0BAB
loc_1_0BAB:
        add    byte ptr [edi + 0x46], bl
        outsd  dx, dword ptr [esi]
        jb     loc_1_0C14
        add    byte ptr fs:[edi + 0x43], bl
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
loc_1_0BB9:
        jne    0xc27
        popal  
        jb     loc_1_0BBE
loc_1_0BBE:
        add    byte ptr [eax], al
loc_1_0BC0:
        pop    edi
        dec    eax
        popal  
        jb     0xc29
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x53], bl
        outsd  dx, dword ptr [esi]
        je     loc_1_0BCE
loc_1_0BCE:
        add    byte ptr [eax], al
loc_1_0BD0:
        pop    edi
        dec    edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BD8:
        pop    edi
        dec    edi
        outsb  dx, byte ptr [esi]
        db  000h
loc_1_0BDC:
        push   ebp
        jae    loc_1_0C44
        jb     0xc2a
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [eax + eax], 0x233c0000
        db  03Eh, 000h
loc_1_0BEC:
        dec    esp
        dec    edi
        inc    edi
        inc    edi
        inc    ebp
        push   edx
        push   esi
        pop    eax
        inc    esp
        add    byte ptr [eax], al
        dec    esp
        dec    edi
        inc    edi
        inc    edi
        inc    ebp
        push   edx
        add    byte ptr [eax], al
loc_1_0C00:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0C08:
        push   ebx
        je     loc_1_0C6C
        jb     loc_1_0C81
        push   eax
        insb   byte ptr es:[edi], dx
        popal  
        jns    loc_1_0C12
loc_1_0C12:
        add    byte ptr [eax], al
loc_1_0C14:
        push   ebx
        je     loc_1_0C86
        jo     0xc69
        insb   byte ptr es:[edi], dx
        popal  
        jns    loc_1_0C1D
loc_1_0C1D:
        add    byte ptr [eax], al
        db  000h
loc_1_0C20:
        push   ebx
        je     0xc84
        jb     0xc99
        push   edx
        arpl   word ptr gs:[edi + 0x72], bp
        db  064h, 000h
loc_1_0C2C:
        push   ebx
        je     0xc9e
        jo     loc_1_0C83
        arpl   word ptr gs:[edi + 0x72], bp
        add    byte ptr fs:[eax], al
loc_1_0C38:
        push   esi
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebx + 0x65], 0x6c616944
        dec    esi
        jne    0xcb1
loc_1_0C44:
        bound  esp, qword ptr [ebp + 0x72]
        push   ebx
        je     loc_1_0CC0
        jo     loc_1_0C4D
loc_1_0C4D:
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x6f], dl
        imul   esp, dword ptr [ebx + 0x65], 0x77736e41
        jb     loc_1_0C5C
loc_1_0C5C:
        add    byte ptr [ebx], cl
        or     eax, 0x10050110
        add    eax, dword ptr [ecx]
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x70], cl
        outsb  dx, byte ptr gs:[esi]
loc_1_0C6C:
        dec    eax
        popal  
        outsb  dx, byte ptr [esi]
        jae    loc_1_0CD7
        je     loc_1_0C74
loc_1_0C74:
        inc    ebx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jae    0xcde
        dec    eax
        popal  
        outsb  dx, byte ptr [esi]
        jae    loc_1_0CE4
        je     loc_1_0C81
loc_1_0C81:
        add    byte ptr [eax], al
loc_1_0C83:
        add    byte ptr [ebx + 0x70], dl
loc_1_0C86:
        popal  
        imul   esp, dword ptr [ebp + 0x72], 0x50
        push   0x45656e6f
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp]
        add    byte ptr [ebx + 0x70], dl
        popal  
        imul   esp, dword ptr [ebp + 0x72], 0x50
        push   0x44656e6f
        imul   esi, dword ptr [ebx + 0x61], 0x656c62
loc_1_0CAC:
        push   ebx
        jo     0xd14
        popal  
        imul   esp, dword ptr [ebp + 0x72], 0x50
        push   0x4d656e6f
        jne    0xd2f
        add    byte ptr gs:[eax], al
        add    byte ptr [eax], al
loc_1_0CC0:
        push   ebx
        jo     0xd28
        popal  
        imul   esp, dword ptr [ebp + 0x72], 0x50
        push   0x55656e6f
        outsb  dx, byte ptr [esi]
        dec    ebp
        jne    0xd45
        add    byte ptr gs:[eax], al
loc_1_0CD4:
        push   ebx
        jo     0xd3c
loc_1_0CD7:
        popal  
        imul   esp, dword ptr [ebp + 0x72], 0x50
        push   0x53656e6f
        je     loc_1_0D3A
loc_1_0CE4:
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        jne    0xd55
        inc    edi
        popal  
        imul   ebp, dword ptr [esi], 0x563c0000
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        add    byte ptr ds:[eax], al
        db  000h
loc_1_0CF8:
        cmp    al, 0x47
        popal  
        imul   ebp, dword ptr [esi + 0x3e], 0x65470000
        outsb  dx, byte ptr [esi]
        jb     0xd67
        je     0xd6d
        inc    esp
        imul   esp, dword ptr [edi + 0x69], 0x74
loc_1_0D10:
        cmp    al, 0x44
        imul   esp, dword ptr [edi + 0x69], 0xff003e74
        db  0FFh, 0FFh, 020h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh
loc_1_0D3A:
        dec    dword ptr [edi]
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Eh, 0FFh, 0FFh, 0FFh, 0FFh, 010h, 000h, 001h, 002h, 003h
        db  004h, 005h, 006h, 007h, 008h, 009h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 00Ah, 00Bh, 00Ch
        db  00Dh, 00Eh, 00Fh, 0FFh, 021h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 027h, 0FFh, 0FFh, 0FFh, 023h, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 025h, 026h, 027h
        db  028h, 02Bh, 02Ch, 0FFh, 022h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 02Ah, 024h, 029h
        db  021h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 011h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x9ab  R-X
        db  065h, 000h, 000h, 000h
loc_2_0004:
        fadd   dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        or     al, byte ptr [eax]
loc_2_0010:
        pop    ds
        add    byte ptr [eax + eax], dh
loc_2_0014:
        dec    ecx
        db  000h
loc_2_0016:
        pushal 
        db  000h
loc_2_0018:
        jnp    loc_2_001A
loc_2_001A:
        xchg   esi, eax
        db  000h
loc_2_001C:
        scasd  eax, dword ptr es:[edi]
        db  000h
loc_2_001E:
        retf   
        db  000h
loc_2_0020:
        in     eax, 0
loc_2_0022:
        sti    
        add    byte ptr [ebx], dl
        db  001h
loc_2_0026:
        sub    al, 1
loc_2_0028:
        dec    esp
        db  001h
loc_2_002A:
        pop    esi
        add    dword ptr [ebx - 0x43fe5aff], eax
        add    esi, eax
        add    eax, edx
        add    esi, ebx
        add    eax, edi
        add    dword ptr [eax], eax
        add    al, byte ptr [ebx]
        add    cl, byte ptr [edi]
        add    dl, byte ptr [edi]
        add    bl, byte ptr [0x34022802]
        add    bh, byte ptr [ecx]
        add    al, byte ptr [ebx + 2]
        dec    eax
        add    cl, byte ptr [edx + eax + 0x50]
        add    dl, byte ptr [edx + eax + 0x58]
        add    bl, byte ptr [edx + eax + 0x60]
        add    ah, byte ptr [edx + eax + 0x68]
        add    ch, byte ptr [ebp + 2]
        jb     loc_2_0064
        ja     loc_2_0066
loc_2_0064:
        jl     0x68
loc_2_0066:
        add    dword ptr [edx], 0x28b0286
        sahf   
        add    dh, byte ptr [ecx - 0x22fd3bfe]
        add    bh, bh
        add    bl, byte ptr [esi]
        add    edi, dword ptr [edx]
        add    ecx, dword ptr [ebp + 3]
loc_2_007C:
        arpl   word ptr [ebx], ax
loc_2_007E:
        test   dword ptr [ebx], eax
loc_2_0080:
        movsd  dword ptr es:[edi], dword ptr [esi]
        add    esi, dword ptr [ebx + eax + 0x3e303bd]
loc_2_0088:
        test   byte ptr [ebx], 2
        add    al, 8
        add    al, 0x1d
        add    al, 0x3e
        add    al, 0x5f
        add    al, 0x7a
        add    al, 0x95
        add    al, 0xb0
        add    al, 0xcb
        add    al, 0xe6
        add    al, 1
        add    eax, 0x53f052a
loc_2_00A4:
        jno    0xab
loc_2_00A6:
        xchg   byte ptr [0x5b5059e], al
loc_2_00AC:
        int3   
        db  005h
loc_2_00AE:
        cli    
        db  005h
loc_2_00B0:
        xor    byte ptr [esi], al
loc_2_00B2:
        dec    ebp
        push   es
loc_2_00B4:
        outsb  dx, byte ptr [esi]
        push   es
loc_2_00B6:
        pop    dword ptr [esi]
loc_2_00B8:
        mov    dh, 6
loc_2_00BA:
        fadd   qword ptr [esi]
loc_2_00BC:
        inc    dword ptr [esi]
loc_2_00BE:
        sbb    dword ptr [edi], eax
loc_2_00C0:
        xor    al, 7
        pop    ebx
        pop    es
loc_2_00C4:
        jns    0xcd
loc_2_00C6:
        cmpsd  dword ptr [esi], dword ptr es:[edi]
        pop    es
loc_2_00C8:
        rol    dword ptr [edi], 1
loc_2_00CA:
        and    dword ptr [eax], ecx
loc_2_00CC:
        or     byte ptr [eax], 0x8c
        or     byte ptr [eax + ecx + 0x8c708b0], bl
        ror    dword ptr [eax], cl
loc_2_00D8:
        inc    esp
        imul   esp, dword ptr [ecx + 0x6c], 0x2e676e69
        or     al, byte ptr [eax]
        push   edi
        popal  
        imul   esi, dword ptr [ecx + ebp*2 + 0x6e], 0x6f662067
        jb     loc_2_010E
        popal  
        and    byte ptr [ebx + 0x61], ah
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        or     al, byte ptr cs:[eax]
        inc    ecx
        outsb  dx, byte ptr [esi]
        jae    0x172
        jb     0x167
        outsb  dx, byte ptr [esi]
        and    byte ptr [si + 0x68], dh
        and    byte ptr gs:[ebx + 0x61], ah
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        or     al, byte ptr cs:[eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
loc_2_010E:
        imul   esi, dword ptr [ecx + ebp*2 + 0x61], 0x697a696c
        outsb  dx, byte ptr [esi]
        and    byte ptr [di + 0x6f], ch
        insd   dword ptr es:[edi], dx
        or     al, byte ptr cs:[eax]
        dec    eax
        popal  
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr [bp + 0x67], 0x20707520
        je     0x196
        and    byte ptr gs:[ebp + 0x6f], ch
        insd   dword ptr es:[edi], dx
        or     al, byte ptr cs:[eax]
        inc    ecx
        outsb  dx, byte ptr [esi]
        jae    0x1b3
        jb     0x1a8
        outsb  dx, byte ptr [esi]
        and    byte ptr [si + 0x68], dh
        and    byte ptr gs:[esi + 0x6f], dh
        imul   esp, dword ptr [ebx + 0x65], 0x6c616320
        insb   byte ptr es:[edi], dx
        or     al, byte ptr cs:[eax]
        push   ebx
        je     loc_2_01CC
        jo     0x179
        je     0x1ca
        and    byte ptr [ecx + ebp*2 + 0x61], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr [esi + 0x6f], dh
        imul   esp, dword ptr [ebx + 0x65], 0x6c616320
        insb   byte ptr es:[edi], dx
        or     al, byte ptr cs:[eax]
        dec    ecx
        jae    0x1e4
        jne    0x1d8
        imul   ebp, dword ptr [esi + 0x67], 0x696f7620
        arpl   word ptr [ebp + 0x20], sp
        arpl   word ptr [edi + 0x6d], bp
        insd   dword ptr es:[edi], dx
        popal  
        outsb  dx, byte ptr [esi]
        or     al, byte ptr cs:[eax]
        push   ebx
        je     loc_2_01EB
        jb     0x200
        imul   ebp, dword ptr [esi + 0x67], 0x76617720
        and    byte ptr gs:[eax + 0x6c], dh
        popal  
        jns    0x1c9
        jb     loc_2_0201
        arpl   word ptr [edi + 0x72], bp
        or     al, byte ptr cs:[eax]
        inc    ebp
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr fs:[esi + 0x67], 0x76617720
        and    byte ptr gs:[eax + 0x6c], dh
        popal  
        jns    loc_2_01E3
        jb     0x21b
        arpl   word ptr [edi + 0x72], bp
        or     al, byte ptr cs:[eax]
        push   ebx
        je     0x235
        imul   ebp, dword ptr [esi + 0x67], 0x76617720
        and    byte ptr gs:[esi + 0x6f], ah
loc_2_01CC:
        jb     loc_2_023B
        popal  
        je     loc_2_01FF
        or     al, byte ptr [eax]
        push   esi
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebx + 0x65], 0x646f4d20
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + 0x61], dl
        jbe    0x248
loc_2_01E3:
        and    byte ptr [edi + 0x70], cl
        outsb  dx, byte ptr gs:[esi]
        or     al, byte ptr cs:[eax]
loc_2_01EB:
        push   esi
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebx + 0x65], 0x646f4d20
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + 0x61], dl
        jbe    0x260
        and    byte ptr [ebx + 0x6c], al
        outsd  dx, dword ptr [esi]
loc_2_01FF:
        jae    loc_2_0266
loc_2_0201:
        or     al, byte ptr cs:[eax]
        dec    ecx
        jae    loc_2_027A
        jne    0x26e
        imul   ebp, dword ptr [esi + 0x67], 0x65705320
        popal  
        imul   esp, dword ptr [ebp + 0x72], 0x70
        push   0x20656e6f
        arpl   word ptr [edi + 0x6d], bp
        insd   dword ptr es:[edi], dx
        popal  
        outsb  dx, byte ptr [esi]
        or     al, byte ptr cs:[eax]
        inc    edi
        outsb  dx, byte ptr gs:[esi]
        jb     0x28b
        je     loc_2_0295
        outsb  dx, byte ptr [esi]
        and    byte ptr [si + 0x54], al
        dec    ebp
        inc    esi
        or     al, byte ptr cs:[eax]
        push   edx
        arpl   word ptr gs:[ebp + 0x69], sp
loc_2_023B:
        jbe    0x2a2
        and    byte ptr fs:[esp + ecx*2 + 0x45], al
        and    byte ptr [edx + 0x61], dl
        ja     0x284
        and    eax, 0x54202c64
        jb     0x2af
        outsb  dx, byte ptr [esi]
        jae    0x2bd
        popal  
        je     0x2b9
        cmp    eax, 0xa2e6425
        add    byte ptr [eax + 0x61], cl
        jb     0x2c3
        ja     0x2c2
        jb     loc_2_02C8
        and    byte ptr [eax + 0x61], ch
loc_2_0266:
        outsb  dx, byte ptr [esi]
        jne    loc_2_02DA
        and    byte ptr [edx + 0x79], ah
        and    byte ptr [edi + ebp*2 + 0x77], ch
        jb     loc_2_02DD
        outsb  dx, byte ptr [esi]
        and    byte ptr [si + 0x54], al
        push   edx
loc_2_027A:
        or     al, byte ptr cs:[eax]
        push   edx
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        je     0x2e8
        and    byte ptr [ebp + 0x6f], ch
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + 0x75], ch
        outsb  dx, byte ptr [esi]
        and    byte ptr [di + 0x70], dh
        or     al, byte ptr cs:[eax]
        push   ebx
loc_2_0295:
        outsb  dx, byte ptr gs:[esi]
        cmp    ah, byte ptr fs:[eax]
        and    eax, 0x52000a73
        arpl   word ptr gs:[esi + 0x3a], si
        and    byte ptr [0x3c000a73], ah
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        and    byte ptr [edx + 0x65], dh
        jae    0x320
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jae    0x319
        add    byte ptr ds:[ecx + 0x6e], cl
        je     0x31f
        jb     0x32c
        jb     loc_2_0323
        je     0x325
        and    byte ptr fs:[edx + 0x65], dh
        jae    loc_2_0336
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_2_02C8:
        jae    loc_2_032F
        cmp    ah, byte ptr [eax]
        and    eax, 0x55000a73
        outsb  dx, byte ptr [esi]
        imul   ebp, dword ptr [esi + 0x6f], 0x77
        outsb  dx, byte ptr [esi]
        add    byte ptr [edi + 0x6b], cl
loc_2_02DA:
        add    byte ptr [ecx + 0x6e], cl
loc_2_02DD:
        outsw  dx, word ptr [esi]
        jb     loc_2_034E
        popal  
        je     loc_2_034D
        jbe    0x34b
        add    byte ptr [ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        arpl   word ptr gs:[eax + eax + 0x45], si
        jb     0x364
        outsd  dx, dword ptr [esi]
        jb     loc_2_02F5
loc_2_02F5:
        dec    esi
        outsd  dx, dword ptr [esi]
        and    byte ptr [ebx + 0x61], al
        jb     0x36e
        imul   esp, dword ptr [ebp + 0x72], 0x206f4e00
        inc    esp
        imul   esp, dword ptr [ecx + 0x6c], 0x656e6f74
        add    byte ptr [edx + 0x75], al
        jae    loc_2_0389
        add    byte ptr [esi + 0x6f], cl
        and    byte ptr [ecx + 0x6e], al
        jae    0x38f
        jb     loc_2_031B
loc_2_031B:
        push   edx
        imul   ebp, dword ptr [esi + 0x67], 0x56535300
loc_2_0323:
        add    byte ptr [ebx + 0x4d], dl
        inc    esp
        add    byte ptr [ebx + 0x46], dl
        inc    ecx
        add    byte ptr [ebx + 0x52], dl
        inc    ecx
loc_2_032F:
        add    byte ptr [ebx + 0x52], dl
        push   ecx
        add    byte ptr [ebx + 0x52], dl
loc_2_0336:
        inc    ebx
        add    byte ptr [ebx + 0x54], dl
        dec    edi
        add    byte ptr [ebx + 0x56], dl
        dec    ebp
        add    byte ptr [edx + edx*2 + 0x4f], al
        dec    esi
        add    byte ptr [edx + edx*2 + 0x4f], al
        inc    esi
        add    byte ptr [ecx + eax*2 + 0x54], al
loc_2_034D:
        inc    ebp
loc_2_034E:
        add    byte ptr [ecx + ecx*2 + 0x4d], dl
        inc    ebp
        add    byte ptr [esi + 0x4d], cl
        inc    edx
        push   edx
        add    byte ptr [esi + 0x41], cl
        dec    ebp
        inc    ebp
        add    byte ptr [ebp + 0x45], cl
        push   ebx
        inc    edi
        add    byte ptr [ecx + ebp*2 + 0x73], al
        je     0x3d1
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ecx + ebp*2 + 0x76], si
        and    byte ptr gs:[edx + 0x69], dl
        outsb  dx, byte ptr [esi]
        and    byte ptr [bx + di], al
        inc    esp
        imul   esi, dword ptr [ebx + 0x74], 0x74636e69
        imul   esi, dword ptr [esi + 0x65], 0x6e695220
        and    byte ptr [bp + si], al
loc_2_0389:
        inc    esp
        imul   esi, dword ptr [ebx + 0x74], 0x74636e69
        imul   esi, dword ptr [esi + 0x65], 0x6e695220
        and    byte ptr [bp + di], al
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        arpl   word ptr gs:[ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x73], ah
        je     loc_2_040C
        bound  ebp, qword ptr [ecx + ebp*2 + 0x73]
        push   0xa2e6465
        add    byte ptr [ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        arpl   word ptr gs:[ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x73], ah
        je     loc_2_0425
        bound  ebp, qword ptr [ecx + ebp*2 + 0x73]
        push   0x61206465
        je     0x3ef
        and    eax, 0x73706264
        or     al, byte ptr cs:[eax]
        inc    ebp
        jb     0x44c
        outsd  dx, dword ptr [esi]
        jb     loc_2_040A
        arpl   word ptr [edi + 0x6e], bp
        je     0x454
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [ebp + 0x6e], dh
        imul   ebp, dword ptr [esi + 0x6f], 0x77
        outsb  dx, byte ptr [esi]
        or     al, byte ptr cs:[eax]
        inc    ebx
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        jne    loc_2_0468
        popal  
        jb     loc_2_041F
        jb     0x474
        outsd  dx, dword ptr [esi]
        jb     loc_2_0432
        arpl   word ptr [edi + 0x6e], bp
        je     0x47c
loc_2_040A:
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
loc_2_040C:
        and    byte ptr [edi + 0x6e], ch
        or     al, byte ptr cs:[eax]
        inc    ebp
        jb     0x487
        outsd  dx, dword ptr [esi]
        jb     loc_2_0445
        arpl   word ptr [edi + 0x6e], bp
        je     loc_2_048F
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
loc_2_041F:
        and    byte ptr [edi + 0x6e], ch
        or     al, byte ptr cs:[eax]
loc_2_0425:
        inc    esp
        popal  
        je     loc_2_048A
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     0x4a1
        jae    0x4a5
loc_2_0432:
        imul   ebp, dword ptr [edi + 0x6e], 0x2e6e6f20
        or     al, byte ptr [eax]
        inc    esp
        popal  
        je     loc_2_04A0
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     0x4b7
loc_2_0445:
        jae    0x4bb
        imul   ebp, dword ptr [edi + 0x6e], 0x66666f20
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [ebp + 0x6e], dh
        imul   ebp, dword ptr [esi + 0x6f], 0x77
        outsb  dx, byte ptr [esi]
        or     al, byte ptr cs:[eax]
        inc    edx
        jns    0x4d4
        jae    loc_2_0483
        jae    loc_2_04CA
        outsb  dx, byte ptr [esi]
        je     loc_2_0488
loc_2_0468:
        je     loc_2_04D9
        and    byte ptr [ebp + 0x6f], ch
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + 0x65], dh
        jb     0x495
        jae    loc_2_04DC
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr cs:[edx + 0x79], al
        je     0x4e6
        jae    0x4a3
loc_2_0483:
        jae    0x4ea
        outsb  dx, byte ptr [esi]
        je     0x4b7
loc_2_0488:
        jae    loc_2_04EF
loc_2_048A:
        arpl   word ptr [eax], ax
        push   ebx
        outsb  dx, byte ptr gs:[esi]
loc_2_048F:
        push   edx
        popal  
        je     0x4f9
        add    byte ptr [edx + 0x79], al
        je     loc_2_04FE
        jae    0x4bb
        jb     loc_2_0502
        arpl   word ptr [ebp + 0x69], sp
loc_2_04A0:
        jbe    loc_2_0507
        and    byte ptr fs:[esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ebp + 0x6f], ch
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + 0x65], dh
        jb     loc_2_04D3
        jae    0x51a
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr cs:[edx + 0x79], al
        je     0x524
        jae    loc_2_04E1
        jb     loc_2_0528
        arpl   word ptr [ebp + 0x69], sp
        jbe    0x52d
        das    
loc_2_04CA:
        jae    0x531
        arpl   word ptr [eax], ax
        push   edx
        arpl   word ptr gs:[ebp + 0x69], sp
loc_2_04D3:
        jbe    0x53a
        push   edx
        popal  
        je     0x53e
loc_2_04D9:
        add    byte ptr [edx + 0x79], al
loc_2_04DC:
        je     loc_2_0543
        jae    loc_2_04E0
loc_2_04E0:
        push   ebx
loc_2_04E1:
        jae    loc_2_0557
        imul   ebp, dword ptr [edi + 0x6e], 0x61745320
        je     0x556
        jae    loc_2_0563
loc_2_04EF:
        imul   esp, dword ptr [ebx + 0x73], 0x20000a3a
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_04FE:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0502:
        and    byte ptr [eax], ah
        push   edx
        popal  
loc_2_0507:
        jae    loc_2_052A
        cmp    ah, byte ptr [eax]
        and    eax, 0x79622064
        je     loc_2_0578
        jae    loc_2_051F
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_051F:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   edi
        jb     loc_2_0591
loc_2_0528:
        je     0x58f
loc_2_052A:
        jae    0x566
        and    byte ptr [0x79622064], ah
        je     loc_2_0599
        jae    loc_2_0540
        add    byte ptr [eax], ah
        and    byte ptr [edi + 0x65], al
        outsb  dx, byte ptr [esi]
        jb     0x5a0
        insb   byte ptr es:[edi], dx
loc_2_0540:
        and    byte ptr [ebx + 0x6f], al
loc_2_0543:
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        and    byte ptr [ebp + 0x72], al
        jb     0x5b9
        jb     loc_2_05BF
        cmp    ah, byte ptr [eax]
        and    eax, 0x20000a64
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0557:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    esi
        jb     loc_2_05BF
        insd   dword ptr es:[edi], dx
        and    byte ptr gs:[ebp + 0x72], al
loc_2_0563:
        jb     0x5d4
        jb     loc_2_05DA
        cmp    ah, byte ptr [eax]
        and    eax, 0x20000a64
        and    byte ptr [ecx + 0x6e], cl
        jo     0x5e8
        je     loc_2_05A4
        dec    edi
        jne    0x5ec
loc_2_0578:
        jo     loc_2_05EF
        je     0x59c
        inc    ebp
        jb     loc_2_05F1
        outsd  dx, dword ptr [esi]
        jb     0x5f5
        cmp    ah, byte ptr [eax]
        and    eax, 0x53000a64
        jb     0x5f5
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [edi + 0x76], cl
loc_2_0591:
        jb     loc_2_0606
        jne    loc_2_0604
        and    byte ptr [ebp + 0x72], al
loc_2_0599:
        jb     loc_2_060A
        jb     0x610
        cmp    ah, byte ptr [eax]
        and    eax, 0x42000a64
loc_2_05A4:
        jne    0x60c
        jb     loc_2_05CA
        dec    edi
        jbe    0x612
        jb     0x621
        jne    loc_2_061F
        and    byte ptr [ebp + 0x72], al
        jb     0x625
        jb     0x62b
        cmp    ah, byte ptr [eax]
        and    eax, 0x20000a64
loc_2_05BF:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax + 0x61], dl
        jb     0x633
loc_2_05CA:
        je     0x645
        and    byte ptr [ebp + 0x72], al
        jb     0x640
        jb     0x646
        cmp    ah, byte ptr [eax]
        and    eax, 0x45000a64
loc_2_05DA:
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [eax + 0x73], 0x6e697274
        and    byte ptr [bp + di + 0x6f], ah
        outsb  dx, byte ptr [esi]
loc_2_05EF:
        jae    loc_2_0665
loc_2_05F1:
        jb     loc_2_0668
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        and    byte ptr [esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x45000a2e
        push   edx
loc_2_0604:
        push   edx
        dec    edi
loc_2_0606:
        push   edx
        cmp    ah, byte ptr [eax]
        dec    ecx
loc_2_060A:
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [eax + 0x66], 0x656c6961
        or     al, byte ptr cs:[eax]
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
loc_2_061F:
        imul   esp, dword ptr [ecx + 0x6c], 0x72747320
        imul   ebp, dword ptr [esi + 0x67], 0x6e6f6320
        jae    0x6a3
        jb     loc_2_06A6
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        and    byte ptr [esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x726f6620
        and    byte ptr [edi], ah
        and    eax, 0xa2e2773
        add    byte ptr [ebp + 0x52], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esp
        imul   esp, dword ptr [ecx + 0x6c], 0x69616620
        insb   byte ptr es:[edi], dx
        or     al, byte ptr cs:[eax]
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
loc_2_0665:
        dec    ebp
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_2_0668:
        imul   esi, dword ptr [edi + ebp*2 + 0x72], 0x69616620
        insb   byte ptr es:[edi], dx
        or     al, byte ptr cs:[eax]
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ecx
        outsb  dx, byte ptr [esi]
        jae    loc_2_06F8
        jb     0x6a4
        popaw  
        imul   ebp, dword ptr [ebp + 0x64], 0x45000a2e
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        dec    eax
        popal  
        outsb  dx, byte ptr [esi]
        jne    loc_2_070A
        and    byte ptr [esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x45000a2e
        push   edx
loc_2_06A6:
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x723
        and    byte ptr [ebx + 0x65], dh
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        je     0x72f
        and    byte ptr gs:[ebp + 0x76], ah
        imul   esp, dword ptr [ebx + 0x65], 0x45000a2e
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x751
        and    byte ptr [edx + 0x65], dh
        arpl   word ptr [edi + 0x76], bp
        jb     loc_2_070B
        jb     loc_2_075D
        insd   dword ptr es:[edi], dx
        and    byte ptr [ebp + 0x6e], dh
        jb     loc_2_0759
        arpl   word ptr [edi + 0x67], bp
        outsb  dx, byte ptr [esi]
loc_2_06F8:
        imul   edi, dword ptr [edx + 0x65], 0x65722064
        jae    loc_2_0771
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jae    loc_2_076A
        or     al, byte ptr cs:[eax]
        inc    ebp
        push   edx
loc_2_070A:
        push   edx
loc_2_070B:
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   esi
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebx + 0x65], 0x736e4120
        ja     0x77f
        jb     0x73c
        popaw  
        imul   ebp, dword ptr [ebp + 0x64], 0x45000a2e
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   esi
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebx + 0x65], 0x61696420
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x65], dh
        je     0x7b0
        jo     loc_2_075D
        popaw  
        imul   ebp, dword ptr [ebp + 0x64], 0x45000a2e
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   esi
        outsd  dx, dword ptr [esi]
        imul   esp, dword ptr [ebx + 0x65], 0x6d6f4320
        insd   dword ptr es:[edi], dx
        popal  
        outsb  dx, byte ptr [esi]
loc_2_0759:
        and    byte ptr fs:[esi + 0x61], ah
loc_2_075D:
        imul   ebp, dword ptr [ebp + 0x64], 0x7325202e
        or     al, byte ptr [eax]
        inc    ebp
        push   edx
        push   edx
loc_2_076A:
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
        je     loc_2_07D2
loc_2_0771:
        jb     0x7e7
        and    byte ptr [edi + 0x61], dh
        jbe    0x7dd
        and    byte ptr [eax + 0x6c], dh
        popal  
        jns    loc_2_07AD
        jb     0x7e5
        arpl   word ptr [edi + 0x72], bp
        and    byte ptr fs:[esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x45000a2e
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
        je     loc_2_0807
        jo     0x7ba
        ja     0x7fd
        jbe    0x803
        and    byte ptr [eax + 0x6c], dh
        popal  
        jns    loc_2_07D3
        jb     0x80b
        arpl   word ptr [edi + 0x72], bp
        and    byte ptr fs:[esi + 0x61], ah
loc_2_07AD:
        imul   ebp, dword ptr [ebp + 0x64], 0x45000a2e
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esi
        popal  
        imul   ebp, dword ptr [ebp + 0x64], 0x206f7420
        jae    loc_2_082C
        je     0x7e9
        ja     loc_2_082C
        jbe    0x832
        and    byte ptr [esi + 0x6f], ah
        jb     0x833
loc_2_07D2:
        insd   dword ptr es:[edi], dx
loc_2_07D3:
        je     0x803
        or     al, byte ptr [eax]
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esi
        popal  
        imul   ebp, dword ptr [ebp + 0x64], 0x76615720
        and    byte ptr gs:[edi + 0x70], cl
        outsb  dx, byte ptr gs:[esi]
        or     al, byte ptr cs:[eax]
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    esi
        popal  
        imul   ebp, dword ptr [ebp + 0x64], 0x76615720
        and    byte ptr gs:[ebx + 0x6c], al
        outsd  dx, dword ptr [esi]
loc_2_0807:
        jae    0x86e
        or     al, byte ptr cs:[eax]
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebx
        jo     0x87b
        popal  
        imul   esp, dword ptr [ebp + 0x72], 0x20
        push   eax
        push   0x656e6f61
        and    byte ptr [ebx + 0x6f], al
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[esi + 0x61], al
loc_2_082C:
        imul   ebp, dword ptr [ebp + 0x64], 0x45000a2e
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    edi
        outsb  dx, byte ptr gs:[esi]
        jb     loc_2_08A1
        je     0x8a7
        and    byte ptr [esp + edx*2 + 0x4d], al
        inc    esi
        and    byte ptr [esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x57000a2e
        inc    ecx
        push   edx
        dec    esi
        dec    ecx
        dec    esi
        inc    edi
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
        jb     0x8c3
        arpl   word ptr [edi + 0x67], bp
        outsb  dx, byte ptr [esi]
        imul   edi, dword ptr [edx + 0x65], 0x65722064
        jae    loc_2_08DB
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jae    0x8d4
        and    byte ptr cs:[eax], ah
        push   edx
        je     loc_2_08E8
        jns    loc_2_08E1
        outsb  dx, byte ptr [esi]
        or     al, byte ptr cs:[bx + si]
        push   edi
        inc    ecx
        push   edx
        dec    esi
        dec    ecx
        dec    esi
        inc    edi
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     loc_2_0900
        and    byte ptr [edi + ebp*2 + 0x61], ch
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [edi], ah
        and    eax, 0x73202773
loc_2_08A1:
        je     0x915
        imul   ebp, dword ptr [esi + 0x67], 0x57000a2e
        inc    ecx
        push   edx
        dec    esi
        dec    ecx
        dec    esi
        inc    edi
        cmp    ah, byte ptr [eax]
        push   esp
        push   0x6f6d2065
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + ebp*2 + 0x64], ah
        and    byte ptr [esi + 0x6f], ch
        je     0x8e4
        jb     loc_2_092B
        jae    loc_2_0938
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        ja     loc_2_0938
        jb     0x93e
        outsb  dx, byte ptr [esi]
        and    byte ptr [si + 0x54], al
        push   edx
loc_2_08DB:
        and    byte ptr cs:[eax], ah
        push   esp
        jb     0x95a
loc_2_08E1:
        imul   ebp, dword ptr [esi + 0x67], 0x666f7320
loc_2_08E8:
        je     0x961
        popal  
        jb     loc_2_0952
        and    byte ptr [eax + 0x61], ch
        outsb  dx, byte ptr [esi]
        jne    loc_2_0964
        or     al, byte ptr cs:[eax]
        push   edi
        inc    ecx
        push   edx
        dec    esi
        dec    ecx
        dec    esi
        inc    edi
loc_2_0900:
        cmp    ah, byte ptr [eax]
        inc    ecx
        and    byte ptr [eax + 0x72], dh
        jbe    0x972
        outsd  dx, dword ptr [esi]
        jne    0x97f
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        jb     0x974
        insd   dword ptr es:[edi], dx
        and    byte ptr [ebp + 0x66], ch
        je     0x93a
        popal  
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        arpl   word ptr gs:[ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x73], ah
        je     0x98c
loc_2_092B:
        bound  ebp, qword ptr [ecx + ebp*2 + 0x73]
        push   0x202e6465
        and    byte ptr [eax + 0x61], cl
        outsb  dx, byte ptr [esi]
loc_2_0938:
        imul   ebp, dword ptr [bp + 0x67], 0x20707520
        jo     0x9b4
        jbe    0x9ae
        outsd  dx, dword ptr [esi]
        jne    0x9bb
        and    byte ptr [ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        arpl   word ptr gs:[ecx + ebp*2 + 0x6f], si
loc_2_0952:
        outsb  dx, byte ptr [esi]
        or     al, byte ptr cs:[eax]
        and    eax, 0x6e692073
        and    byte ptr [ebp + 0x73], dh
        or     al, byte ptr cs:[eax]
loc_2_0964:
        dec    ebp
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + edi*2 + 0x70], dh
        cmp    ah, byte ptr gs:[eax]
        and    eax, 0x4d000a73
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [eax + 0x61], dh
        je     0x9ea
        cmp    ah, byte ptr [eax]
        and    eax, 0x4d000a73
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ebx + 0x65], dh
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        cmp    ah, byte ptr [eax]
        and    eax, 0x25000a73
        jae    0x9c2
        arpl   word ptr [edi + ebp*2 + 0x73], bp
        or     al, byte ptr cs:[eax]
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x80c4  R-X
loc_3_0000:
        VxDCall  BIOS.Service_0000h              ; cd2000002b00
        jae    9
        ret    
        db  068h
        dd  loc_3_0024               ; fixup
        db  0CDh, 020h, 001h, 000h, 02Bh, 000h, 083h, 0C4h, 004h, 080h, 03Dh
        dd  obj4:0x0004              ; fixup
        db  000h, 074h, 002h, 0F8h, 0C3h, 0F9h, 0C3h
loc_3_0024:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     loc_3_002D
        ret    
loc_3_002D:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        push   dword ptr [ebp + 0x1c]
        call   sub_3_42CC
        add    esp, 4
        or     eax, eax
        jne    loc_3_008D
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0xc]
        call   sub_3_44E8
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     loc_3_008D
        mov    word ptr [esi], 0x94
        mov    word ptr [esi + 2], 0x10a
        mov    dword ptr [esi + 4], 0x650    ; -> loc_1_0650
        mov    dword ptr [esi + 8], 0x17
        inc    byte ptr [4]                  ; -> obj4:0x0004
        lea    ebx, [esi + 0x9c]
        push   ebx
        push   0xd4                          ; -> loc_3_00D4
        push   dword ptr [ebp + 0x10]
        VxDCall  BIOS.Service_0019h              ; cd2019002b00
        add    esp, 0xc
loc_3_008D:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_0092:
        push   edx
        call   sub_3_1E80
        pop    edx
        ret    
loc_3_009A:
        push   edx
        call   sub_3_2C5B
        pop    edx
        ret    
sub_3_00A2:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        xor    esi, esi
        xchg   dword ptr [edx + 0x4f4], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        leave  
        ret    
sub_3_00BA:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        xor    esi, esi
        xchg   dword ptr [edx + 0x620], esi
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        pop    esi
        leave  
        ret    
        db  000h, 000h
loc_3_00D4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_3_42CC
        add    esp, 4
        or     eax, eax
        je     loc_3_0129
        push   eax
        call   sub_3_4303
        add    esp, 4
        or     eax, eax
        je     loc_3_0130
        mov    esi, eax
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_3_0137
        add    esp, 8
        cmp    eax, -1
        jne    loc_3_0129
        sub    eax, eax
        mov    dword ptr [esi + 0xc], eax
        push   eax
        push   esi
        call   sub_3_0214
        add    esp, 4
        pop    eax
loc_3_0118:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx], eax
        or     eax, eax
        mov    eax, esi
        je     loc_3_0125
        xor    eax, eax
loc_3_0125:
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_0129:
        mov    eax, 0xfffffffb
        jmp    loc_3_0118
loc_3_0130:
        mov    eax, 0xfffffffe
        jmp    loc_3_0118
sub_3_0137:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x29c]
        or     eax, eax
        jne    loc_3_020D
        lea    eax, [esi + 0x19c]
        push   dword ptr [ebp + 0xc]
        push   eax
        VxDCall  BIOS.Service_0004h              ; cd2004002b00
        add    esp, 8
        cmp    eax, -0x14
        jae    loc_3_020D
        mov    dword ptr [esi + 0x29c], eax
        mov    dword ptr [esi + 0x2d0], 0xffffffff
        mov    dword ptr [esi + 0x2dc], 0xffffffff
        mov    ebx, 0x172                    ; -> sub_1_0172
        push   esi
        push   ebx
        push   eax
        VxDCall  BIOS.Service_0014h              ; cd2014002b00
        add    esp, 0xc
        lea    edi, [esi + 0x88]
        mov    eax, dword ptr [esi + 0x29c]
        push   0
        push   edi
        push   0x13
        push   eax
        VxDCall  BIOS.Service_000Eh              ; cd200e002b00
        add    esp, 0x10
        mov    eax, dword ptr [esi + 0x29c]
        mov    ebx, dword ptr [eax + 4]
        cmp    dword ptr [ebx + 0x1c], 0
        je     loc_3_01E9
        lea    edi, [esi + 0x38]
        mov    dword ptr [esi + 0x2ac], edi
        push   edi
        push   eax
        call   dword ptr [ebx + 0x1c]
        add    esp, 8
        push   esi
        lea    eax, [esi + 0x7a2]
        push   eax
        push   dword ptr [esi + 0x29c]
        lea    eax, [esi + 0x640]
        push   eax
        call   sub_3_64C0
loc_3_01E9:
        mov    eax, 0x34
        add    eax, 0x2d
        push   eax
        lea    eax, [esi + 0x54d]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_3_54CC
        add    esp, 0xc
        mov    eax, 0xffffffff
loc_3_0209:
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_020D:
        mov    eax, 0
        jmp    loc_3_0209
sub_3_0214:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    dword ptr [esi + 0x2b8], eax
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_030E
        VxDCall  Dev_0048.Service_0000h          ; cd2000004800
        or     eax, eax
        je     loc_3_030E
        push   0
        lea    ebx, [esi + 0x9c]
        push   ebx
        push   ebx
        push   0
        push   0
        push   esp
        VxDCall  Dev_0048.Service_0001h          ; cd2001004800
        add    esp, 0x18
        or     eax, eax
        je     loc_3_030E
        mov    dword ptr [esi + 0x2b8], eax
        lea    ebx, [esi + 0x2b4]
        push   ebx
        movzx  ebx, word ptr [0x7e]          ; -> loc_2_007E
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        push   ebx
        movzx  ebx, word ptr [0x8a]          ; -> obj2:0x008A
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        push   ebx
        movzx  ebx, word ptr [0x82]          ; -> obj2:0x0082
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        push   ebx
        movzx  ebx, word ptr [0x80]          ; -> loc_2_0080
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        push   ebx
        push   2
        push   0
        push   esp
        mov    eax, dword ptr [esi + 0x2b8]
        push   eax
        VxDCall  Dev_0048.Service_0003h          ; cd2003004800
        add    esp, 0x24
        lea    ebx, [esi + 0x2b0]
        push   ebx
        movzx  ebx, word ptr [0x84]          ; -> obj2:0x0084
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        push   ebx
        movzx  ebx, word ptr [0x8a]          ; -> obj2:0x008A
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        push   ebx
        movzx  ebx, word ptr [0x88]          ; -> loc_2_0088
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        push   ebx
        movzx  ebx, word ptr [0x86]          ; -> obj2:0x0086
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        push   ebx
        push   2
        push   0
        push   esp
        mov    eax, dword ptr [esi + 0x2b8]
        push   eax
        VxDCall  Dev_0048.Service_0003h          ; cd2003004800
        add    esp, 0x24
loc_3_030E:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_0313:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x2b8]
        cmp    eax, 0
        je     loc_3_0330
        push   eax
        VxDCall  Dev_0048.Service_0002h          ; cd2002004800
        add    esp, 4
loc_3_0330:
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_0334:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x29c]
        or     eax, eax
        je     loc_3_03F0
        push   0
        push   dword ptr [esi + 0x29c]
        VxDCall  BIOS.Service_000Fh              ; cd200f002b00
        add    esp, 8
        push   1
        push   dword ptr [esi + 0x29c]
        VxDCall  BIOS.Service_000Fh              ; cd200f002b00
        add    esp, 8
        mov    eax, dword ptr [esi + 0x2e0]
        cmp    eax, 7
        je     loc_3_03E1
        mov    eax, dword ptr [esi + 0x5ee]
        or     eax, eax
        je     loc_3_03E1
        push   0
        push   4
        push   dword ptr [esi + 0x29c]
        VxDCall  BIOS.Service_0010h              ; cd2010002b00
        add    esp, 0xc
        mov    dword ptr [esi + 0x5f6], 0
        lea    eax, [esi + 0x4f0]
        push   eax
        push   dword ptr [esi + 0x5f2]
        push   dword ptr [esi + 0x5ee]
        push   dword ptr [esi + 0x29c]
        VxDCall  BIOS.Service_0012h              ; cd2012002b00
        add    esp, 0x10
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    ebx, eax
loc_3_03C9:
        cmp    dword ptr [esi + 0x5f6], 1
        je     loc_3_03E1
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        sub    eax, ebx
        cmp    eax, 0x3e8
        jb     loc_3_03C9
loc_3_03E1:
        push   dword ptr [esi + 0x29c]
        VxDCall  BIOS.Service_0009h              ; cd2009002b00
        add    esp, 4
loc_3_03F0:
        push   esi
        call   sub_3_0313
        add    esp, 4
        push   esi
        call   sub_3_3F80
        add    esp, 4
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_0407:
        mov    ecx, 4
        jmp    loc_1_035B
loc_3_0411:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x2ac], ecx
        mov    ecx, 0x1c
        jmp    loc_1_035B
loc_3_0429:
        mov    ecx, 0x28
        jmp    sub_1_033F
loc_3_0433:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x5b6], ecx
        cmp    dword ptr [eax + 0x2e0], 7
        jne    loc_3_0454
        mov    ecx, dword ptr [eax + 0x5ba]
        or     dword ptr [esp + 8], ecx
loc_3_0454:
        mov    ecx, 0x2c
        jmp    loc_1_035B
loc_3_045E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x10]
        cli    
        mov    dword ptr [edx + 0x2c0], eax
        mov    dword ptr [edx + 0x2bc], ecx
        and    dword ptr [edx + 0x2c4], 0xfffffffb
        jecxz  loc_3_04A9
        or     dword ptr [edx + 0x2c4], 4
        mov    eax, dword ptr [edx + 0x30]
        and    eax, dword ptr [edx + 0x10]
        je     loc_3_04A9
        sti    
        push   edx
        push   dword ptr [edx + 0x30]
        push   4
        push   edx
        push   dword ptr [edx + 0x29c]
        call   sub_1_0172
        add    esp, 0x10
        pop    edx
loc_3_04A9:
        sti    
        mov    dword ptr [edx + 0xc], 0
        mov    eax, 0xffffffff
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_04BB:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x29c]
        or     eax, eax
        jne    loc_3_04D5
        mov    eax, 0
        jmp    loc_3_053B
loc_3_04D5:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x2d0], ecx
        inc    ecx
        jecxz  loc_3_04F3
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx + 0x2c8], ecx
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [edx + 0x2cc], ecx
loc_3_04F3:
        cmp    dword ptr [edx + 0x2e0], 7
        je     loc_3_050C
        cmp    dword ptr [edx + 0x2e0], 0x1a
        je     loc_3_050C
        mov    eax, 0
        jmp    loc_3_053B
loc_3_050C:
        mov    esi, dword ptr [edx + 0x29c]
        mov    edi, dword ptr [esi + 4]
        mov    eax, 0x40
        mov    eax, dword ptr [eax + edi]
        push   edx
        push   0x172                         ; -> sub_1_0172
        push   dword ptr [ebp + 0xc]
        push   esi
        call   eax
        add    esp, 0x10
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x29c]
        mov    ecx, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 0xc], ecx
loc_3_053B:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_0540:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x29c]
        or     eax, eax
        jne    loc_3_055A
        mov    eax, 0
        jmp    loc_3_05C0
loc_3_055A:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x2dc], ecx
        inc    ecx
        jecxz  loc_3_0578
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx + 0x2d4], ecx
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [edx + 0x2d8], ecx
loc_3_0578:
        cmp    dword ptr [edx + 0x2e0], 7
        je     loc_3_0591
        cmp    dword ptr [edx + 0x2e0], 0x1a
        je     loc_3_0591
        mov    eax, 0
        jmp    loc_3_05C0
loc_3_0591:
        mov    esi, dword ptr [edx + 0x29c]
        mov    edi, dword ptr [esi + 4]
        mov    eax, 0x44
        mov    eax, dword ptr [eax + edi]
        push   edx
        push   0x172                         ; -> sub_1_0172
        push   dword ptr [ebp + 0xc]
        push   esi
        call   eax
        add    esp, 0x10
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edx + 0x29c]
        mov    ecx, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 0xc], ecx
loc_3_05C0:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_05C5:
        mov    ecx, 0x48
        jmp    loc_1_035B
loc_3_05CF:
        mov    ecx, 0x54
        jmp    loc_1_035B
loc_3_05D9:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 0xa000
        jb     loc_3_0611
        cmp    eax, 0xa00d
        ja     loc_3_0611
        sub    eax, 0xa000
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [edx], ecx
        push   edx
        call   dword ptr [eax*4 + 0x5e8]     ; -> obj1:0x05E8
        add    esp, 4
        mov    edx, dword ptr [esp + 0x14]
        mov    dword ptr [edx], eax
        mov    eax, 0xffffffff
        ret    
loc_3_0611:
        mov    ecx, 0x58
        jmp    loc_1_035B
        db  000h
sub_3_061C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld    
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        movzx  eax, byte ptr [esi - 1]
        movzx  edx, byte ptr [edi - 1]
        sub    eax, edx
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_063B:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0x2a0]
        or     eax, eax
        je     loc_3_0683
        mov    eax, dword ptr [edx + 0x61c]
        mov    dword ptr [edx + 0x61c], 0xffffffff
        cmp    eax, -1
        jne    loc_3_0663
        mov    eax, dword ptr [ebp + 0xc]
loc_3_0663:
        push   0
        push   0
        push   dword ptr [edx + 0x2a8]
        push   eax
        push   dword ptr [edx + 0x2a4]
        push   dword ptr [edx + 0x2a0]
        VxDCall  SHELL._SHELL_PostMessage        ; cd2006001700
        add    esp, 0x18
loc_3_0683:
        cld    
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    al, 0x27
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_3_0695
        mov    al, 7
loc_3_0695:
        mov    byte ptr [ebp + 0x18], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x10]
loc_3_06A4:
        xor    edx, edx
        div    ecx
        xchg   edx, eax
        add    al, 0x30
        cmp    al, 0x39
        jbe    loc_3_06B2
        add    al, byte ptr [ebp + 0x18]
loc_3_06B2:
        dec    esi
        je     loc_3_06BC
        stosb  byte ptr es:[edi], al
        mov    eax, edx
        or     eax, eax
        jne    loc_3_06A4
loc_3_06BC:
        mov    eax, edi
        sub    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, edi
        sub    ecx, esi
        inc    ecx
        shr    ecx, 1
        jcxz   loc_3_06E6
loc_3_06DA:
        mov    ah, byte ptr [edi]
        mov    al, byte ptr [esi]
        mov    byte ptr [esi], ah
        mov    byte ptr [edi], al
        inc    esi
        dec    edi
        loop   loc_3_06DA
loc_3_06E6:
        pop    edi
        pop    esi
        leave  
        ret    
        db  0CDh, 020h, 032h, 080h, 040h, 000h, 0CDh, 020h, 006h, 080h, 040h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_3_0700:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x18]
        mov    esi, 2
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ecx + 0x2e0]
        cmp    eax, 3
        je     loc_3_0735
        cmp    eax, 0x18
        je     loc_3_0735
        cmp    eax, 0x13
        je     loc_3_0735
        cmp    eax, 4
        je     loc_3_0735
        cmp    eax, 8
        jne    loc_3_07F3
loc_3_0735:
        push   0
        push   ecx
        call   sub_3_3E17
        add    esp, 8
        lea    ebx, [edi + 0x10]
        lea    eax, [esp + 0x10]
        push   eax
        push   ebx
        push   dword ptr [edi]
        call   sub_3_3523
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        je     loc_3_07C5
        mov    esi, dword ptr [edi]
        mov    eax, dword ptr [esi + 0x2e0]
        cmp    eax, 0x18
        je     loc_3_077A
        cmp    eax, 0x13
        je     loc_3_077A
        cmp    dword ptr [esp + 0x10], 1
        sbb    eax, eax
        and    eax, 0xfffffffc
        add    eax, 9
        jmp    loc_3_0787
loc_3_077A:
        cmp    dword ptr [esp + 0x10], 1
        sbb    eax, eax
        and    eax, 2
        add    eax, 0x10
loc_3_0787:
        mov    dword ptr [esi + 0x2e0], eax
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_07AE
        movzx  eax, word ptr [0xc]           ; -> loc_2_000C
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_07AE:
        push   ebp
        push   dword ptr [edi + 0xc]
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        push   esi
        call   sub_3_11F4
        add    esp, 0x14
        mov    esi, eax
        jmp    loc_3_07F3
loc_3_07C5:
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 0x2e0], 1
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_07F9
        push   ebx
        movzx  eax, word ptr [0xa2]          ; -> obj2:0x00A2
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 0xc
loc_3_07F3:
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
loc_3_07F9:
        cmp    esi, 2
        jne    loc_3_0819
        or     ecx, ecx
        je     loc_3_0819
        movzx  eax, word ptr [0xa4]          ; -> loc_2_00A4
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_0819:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_3_0823:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   0x80
        push   dword ptr [esi]
        call   sub_3_3E17
        add    esp, 8
        push   esi
        call   sub_3_0842
        add    esp, 4
        pop    esi
        ret    
sub_3_0842:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 0x18]
        mov    ebx, 2
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 0x7d3], eax
        mov    eax, dword ptr [esi + 8]
        mov    ebp, 0x830                    ; -> loc_1_0830
        mov    dword ptr [ecx + 0x7d7], eax
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ecx + 0x7db], eax
        mov    eax, dword ptr [ecx + 0x2e0]
        cmp    eax, 3
        je     loc_3_088A
        cmp    eax, 4
        jne    loc_3_0957
loc_3_088A:
        cmp    word ptr [ecx + 0x899], 0
        je     loc_3_08B8
        cmp    word ptr [ecx + 0x89b], 0x80
        jne    loc_3_08A6
        mov    ebp, 0x808                    ; -> loc_1_0808
        jmp    loc_3_08FA
loc_3_08A6:
        mov    ebp, 0x81c                    ; -> loc_1_081C
        push   9
        push   ecx
        call   sub_3_3DE5
        add    esp, 8
        jmp    loc_3_08FA
loc_3_08B8:
        test   byte ptr [esi + 0x14], 2
        je     loc_3_08D6
        push   0
        push   0x838                         ; -> obj1:0x0838
        push   ecx
        call   sub_3_31F4
        add    esp, 0xc
        mov    edi, eax
        or     edi, edi
        jne    loc_3_08D6
        sub    edi, edi
loc_3_08D6:
        test   byte ptr [esi + 0x14], 1
        je     loc_3_08FA
        mov    dword ptr [esp + 0x10], edi
        push   edi
        push   0x848                         ; -> loc_1_0848
        push   dword ptr [esi]
        call   sub_3_31F4
        add    esp, 0xc
        mov    edi, eax
        or     edi, edi
        jne    loc_3_08FA
        mov    edi, dword ptr [esp + 0x10]
loc_3_08FA:
        push   edi
        push   ebp
        push   dword ptr [esi]
        call   sub_3_31F4
        add    esp, 0xc
        mov    edi, eax
        or     edi, edi
        je     loc_3_0957
        mov    ebx, dword ptr [esi]
        mov    dword ptr [ebx + 0x2e0], 4
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_0939
        movzx  eax, word ptr [0xe]           ; -> loc_2_000E
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_0939:
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebx + 0x5be], eax
        push   edi
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   ebx
        call   sub_3_11F4
        add    esp, 0x14
        mov    ebx, eax
loc_3_0957:
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        cmp    ebx, 2
        jne    loc_3_097D
        or     ecx, ecx
        je     loc_3_097D
        movzx  eax, word ptr [0xa6]          ; -> loc_2_00A6
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_097D:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_3_0987:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, 2
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [ecx + 0x2e0]
        cmp    eax, 3
        je     loc_3_09AF
        cmp    eax, 0xf
        je     loc_3_09AF
        cmp    eax, 4
        je     loc_3_09AF
        cmp    eax, 8
        jne    loc_3_0A24
loc_3_09AF:
        push   0
        push   ecx
        call   sub_3_3E17
        add    esp, 8
        mov    eax, dword ptr [esi]
        push   0
        cmp    dword ptr [eax + 0x2e0], 0xf
        jne    loc_3_09CE
        push   0x8e4                         ; -> obj1:0x08E4
        jmp    loc_3_09D3
loc_3_09CE:
        push   0x860                         ; -> obj1:0x0860
loc_3_09D3:
        push   eax
        call   sub_3_31F4
        add    esp, 0xc
        mov    ebx, eax
        or     ebx, ebx
        je     loc_3_0A24
        mov    edi, dword ptr [esi]
        mov    dword ptr [edi + 0x2e0], 6
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_0A0F
        movzx  eax, word ptr [0x10]          ; -> loc_2_0010
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_0A0F:
        push   ebx
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   edi
        call   sub_3_11F4
        add    esp, 0x14
        mov    edi, eax
loc_3_0A24:
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        cmp    edi, 2
        jne    loc_3_0A4A
        or     ecx, ecx
        je     loc_3_0A4A
        movzx  eax, word ptr [0xa8]          ; -> obj2:0x00A8
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_0A4A:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_0A50:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    ebx, 2
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [ecx + 0x2e0]
        cmp    eax, 1
        je     loc_3_0A78
        cmp    eax, 3
        je     loc_3_0A78
        cmp    eax, 4
        jne    loc_3_0C09
loc_3_0A78:
        push   0
        push   ecx
        call   sub_3_3E17
        add    esp, 8
        mov    eax, dword ptr [esi]
        push   eax
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 4]
        add    eax, 0x7a2
        push   0
        push   dword ptr [eax + 0xa7]
        push   eax
        call   sub_3_7ED0
        push   1
        mov    eax, dword ptr [esi]
        add    eax, 0x640
        push   eax
        call   sub_3_64E3
        mov    edi, dword ptr [esi]
        add    edi, 0x6fa
        sub    eax, eax
        mov    ecx, 0x26
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [esi]
        push   0
        mov    byte ptr [eax + 0x7fb], 0
        mov    dword ptr [eax + 0x7f3], 0x10000
        mov    dword ptr [eax + 0x7f7], 0x10000
        mov    dword ptr [eax + 0x6fa], 0x10
        mov    dword ptr [eax + 0x7c7], 0
        push   0x868                         ; -> obj1:0x0868
        push   eax
        call   sub_3_31F4
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        je     loc_3_0C09
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 0x5c2], 0
        jne    loc_3_0B69
        push   eax
        call   sub_3_5651
        add    esp, 4
        or     eax, eax
        jne    loc_3_0B5D
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 0x618], 0
        je     loc_3_0B5D
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_0B4D
        movzx  ecx, word ptr [0x9e]          ; -> obj2:0x009E
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_0B4D:
        push   0
        push   ebp
        call   sub_1_04D0
        add    esp, 8
        jmp    loc_3_0C0E
loc_3_0B5D:
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 0x5c2], 1
loc_3_0B69:
        push   ebp
        push   0xab8                         ; -> loc_1_0AB8
        push   dword ptr [esi]
        call   sub_3_31F4
        add    esp, 0xc
        mov    edi, eax
        or     edi, edi
        je     loc_3_0BFE
        mov    eax, dword ptr [esi]
        push   0
        mov    dword ptr [eax + 0x5ba], 0
        push   0
        push   dword ptr [eax + 0x29c]
        call   sub_1_0490
        add    esp, 0xc
        mov    ebx, dword ptr [esi]
        mov    dword ptr [ebx + 0x2e0], 2
        mov    dword ptr [ebx + 0x5a5], 0
        mov    dword ptr [ebx + 0x5a9], 0
        mov    dword ptr [ebx + 0x5ca], 0
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_0BE7
        movzx  ecx, word ptr [0x12]          ; -> obj2:0x0012
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_0BE7:
        push   edi
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   ebx
        call   sub_3_11F4
        add    esp, 0x14
        mov    ebx, eax
        jmp    loc_3_0C09
loc_3_0BFE:
        push   0
        push   ebp
        call   sub_1_04D0
        add    esp, 8
loc_3_0C09:
        cmp    ebx, 2
        jne    loc_3_0C2E
loc_3_0C0E:
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_0C2E
        movzx  ecx, word ptr [0xa0]          ; -> obj2:0x00A0
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_0C2E:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_0C35:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, 2
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_0C61
        movzx  ecx, word ptr [0x14]          ; -> loc_2_0014
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_0C61:
        mov    edi, dword ptr [esp + 0x30]
        mov    ecx, dword ptr [edi]
        mov    dword ptr [ecx + 0x61c], 0
        mov    eax, dword ptr [ecx + 0x2e0]
        lea    edx, [ecx + 0x2e0]
        cmp    eax, 0xa
        je     loc_3_0FDB
        cmp    eax, 0xb
        je     loc_3_0FDB
        cmp    eax, 0xc
        je     loc_3_0FDB
        cmp    eax, 0xd
        je     loc_3_0FDB
        cmp    eax, 0x1a
        je     loc_3_0FC1
        cmp    eax, 0x17
        je     loc_3_0FBC
        cmp    eax, 0x10
        je     loc_3_0F5F
        cmp    eax, 0xe
        je     loc_3_0F5F
        push   ecx
        call   sub_3_00A2
        add    esp, 4
        mov    eax, dword ptr [edi]
        push   0
        mov    dword ptr [eax + 0x5ba], 0
        push   0
        push   dword ptr [eax + 0x29c]
        call   sub_1_0490
        add    esp, 0xc
        push   dword ptr [edi]
        call   sub_3_00BA
        add    esp, 4
        push   1
        mov    eax, dword ptr [edi]
        add    eax, 0x640
        push   eax
        call   sub_3_64E3
        push   0
        push   0
        mov    eax, dword ptr [edi]
        push   -1
        push   dword ptr [eax + 0x29c]
        call   sub_1_04C0
        add    esp, 0x10
        mov    ebx, dword ptr [edi]
        push   0
        push   dword ptr [ebx + 0x29c]
        call   sub_1_0488
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x5e2]
        or     eax, eax
        je     loc_3_0D56
        cmp    dword ptr [ebx + 0x5de], eax
        je     loc_3_0D4A
        push   0
        push   eax
        call   sub_1_04D0
        add    esp, 8
loc_3_0D4A:
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 0x5e2], 0
loc_3_0D56:
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        jne    loc_3_0D6B
        mov    eax, 2
        jmp    loc_3_0FE0
loc_3_0D6B:
        mov    ebx, dword ptr [edi]
        mov    dword ptr [ebx + 0x5d2], 1
        mov    eax, dword ptr [ebx + 0x2e0]
        lea    ebp, [ebx + 0x2e0]
        dec    eax
        cmp    eax, 0x12
        ja     loc_3_0D90
        jmp    dword ptr [eax*4 + 0xfe8]     ; -> obj3:0x0FE8
loc_3_0D90:
        mov    dword ptr [ebp], 1
        sub    esi, esi
        jmp    loc_3_0F1C
loc_3_0D9E:
        sub    esi, esi
        jmp    loc_3_0F1C
loc_3_0DA5:
        cmp    dword ptr [ebx + 0x618], 0
        je     loc_3_0E59
        push   0
        push   1
        push   0x8fc                         ; -> obj1:0x08FC
        call   sub_3_33D7
        lea    eax, [esp + 0x20]
        add    esp, 0xc
        push   eax
        push   1
        mov    eax, dword ptr [edi]
        push   0x900                         ; -> obj1:0x0900
        push   dword ptr [eax + 0x29c]
        call   sub_1_04A0
        add    esp, 0x10
        or     eax, eax
        jne    loc_3_0DF4
        mov    eax, dword ptr [edi]
        mov    dword ptr [eax + 0x2e0], 1
        jmp    loc_3_0F1C
loc_3_0DF4:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_0E09
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [esp + 0x14]
        add    dword ptr [eax + 0x2b4], ecx
        jmp    loc_3_0E15
loc_3_0E09:
        push   0
        push   dword ptr [edi]
        call   sub_3_3349
        add    esp, 8
loc_3_0E15:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 0x2a0], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ecx + 0x2a4], eax
        mov    eax, dword ptr [edi + 0xc]
        mov    esi, 1
        mov    dword ptr [ecx + 0x2a8], eax
        mov    dword ptr [ecx + 0x4ec], 1
        mov    dword ptr [ecx + 0x2e0], 0xc
        push   ecx
        call   sub_3_1E80
        add    esp, 4
        jmp    loc_3_0F1C
loc_3_0E59:
        mov    dword ptr [ebp], 3
        sub    esi, esi
        jmp    loc_3_0F1C
loc_3_0E67:
        cmp    dword ptr [ebx + 0x618], 0
        jne    loc_3_0EFA
        sub    esi, esi
        jmp    loc_3_0F1C
loc_3_0E7B:
        mov    dword ptr [ebp], 3
        sub    esi, esi
        jmp    loc_3_0F1C
loc_3_0E89:
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_0EA9
        movzx  ecx, word ptr [0x2c]          ; -> obj2:0x002C
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_0EA9:
        push   0
        push   0
        push   6
        push   dword ptr [ebx + 0x29c]
        call   sub_1_0480
        mov    eax, dword ptr [esp + 0x20]
        add    esp, 0x10
        mov    dword ptr [ebx + 0x2a0], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 0x2a4], eax
        mov    eax, dword ptr [edi + 0xc]
        mov    esi, 1
        mov    dword ptr [ebx + 0x2a8], eax
        mov    dword ptr [ebp], 0xb
        mov    dword ptr [ebx + 0x4ec], 0xf
        push   ebx
        call   sub_3_1E80
        add    esp, 4
        jmp    loc_3_0F1C
loc_3_0EFA:
        mov    dword ptr [ebp], 0xd
        push   dword ptr [ebx + 0x5de]
        push   dword ptr [edi + 0xc]
        push   dword ptr [edi + 8]
        push   dword ptr [esp + 0x1c]
        push   ebx
        call   sub_3_11F4
        add    esp, 0x14
        mov    esi, eax
loc_3_0F1C:
        lea    eax, [esp + 0x18]
        push   eax
        mov    eax, dword ptr [edi]
        push   0
        push   dword ptr [eax + 0x29c]
        call   sub_1_0470
        add    esp, 0xc
        cmp    esi, 2
        jne    loc_3_0F58
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_0F58
        movzx  ecx, word ptr [0xaa]          ; -> obj2:0x00AA
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_0F58:
        mov    eax, esi
        jmp    loc_3_0FE0
loc_3_0F5F:
        mov    dword ptr [edx], 0x19
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ecx + 0x7c7], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ecx + 0x7cb], eax
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ecx + 0x7cf], eax
        lea    eax, [esp + 0x20]
        push   eax
        push   dword ptr [ecx + 0x29c]
        call   sub_1_0468
        add    esp, 8
        or     eax, eax
        je     loc_3_0FDB
        cmp    dword ptr [esp + 0x28], 0
        jne    loc_3_0FDB
        lea    eax, [esp + 0x1c]
        push   eax
        push   1
        mov    eax, dword ptr [edi]
        push   0x8f8                         ; -> loc_1_08F8
        push   dword ptr [eax + 0x29c]
        call   sub_1_04A0
        add    esp, 0x10
        jmp    loc_3_0FDB
loc_3_0FBC:
        cmp    eax, 0x1a
        jne    loc_3_0FCE
loc_3_0FC1:
        cmp    dword ptr [ecx + 0x7fc], 1
        jne    loc_3_0FCE
        sub    eax, eax
        jmp    loc_3_0FE0
loc_3_0FCE:
        push   4
        push   edi
        call   sub_3_7341
        add    esp, 8
        jmp    loc_3_0FE0
loc_3_0FDB:
        mov    eax, 1
loc_3_0FE0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        ; ---- dispatch table (19 entries) ----
        dd  loc_3_0D9E
        dd  loc_3_0DA5
        dd  loc_3_0E67
        dd  loc_3_0E7B
        dd  loc_3_0DA5
        dd  loc_3_0DA5
        dd  loc_3_0E89
        dd  loc_3_0EFA
        dd  loc_3_0DA5
        dd  loc_3_0D90
        dd  loc_3_0D90
        dd  loc_3_0D90
        dd  loc_3_0D90
        dd  loc_3_0EFA
        dd  loc_3_0EFA
        dd  loc_3_0DA5
        dd  loc_3_0EFA
        dd  loc_3_0DA5
        dd  loc_3_0EFA
sub_3_1034:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 0x2e0], 0x1a
        jne    loc_3_1052
        mov    eax, 2
        jmp    loc_3_118E
loc_3_1052:
        push   2
        push   eax
        call   sub_3_75C1
        add    esp, 8
        push   dword ptr [esi]
        call   sub_3_00A2
        add    esp, 4
        mov    eax, dword ptr [esi + 0xc]
        cmp    eax, 1
        je     loc_3_1082
        cmp    eax, 2
        je     loc_3_10DC
        cmp    eax, 3
        je     loc_3_1163
        jmp    loc_3_118C
loc_3_1082:
        mov    esi, dword ptr [esi]
        push   esi
        mov    dword ptr [esi + 0x2e0], 7
        mov    edi, dword ptr [esi + 0x29c]
        push   0x172                         ; -> sub_1_0172
        push   dword ptr [esi + 0x2d0]
        push   edi
        call   sub_1_04B8
        add    esp, 0x10
        push   esi
        push   0x172                         ; -> sub_1_0172
        push   dword ptr [esi + 0x2dc]
        push   edi
        call   sub_1_04C0
        add    esp, 0x10
        mov    dword ptr [esi + 0x5ba], 0x80
        push   0
        mov    eax, dword ptr [esi + 0x5b6]
        or     eax, 0x80
        push   eax
        push   edi
        jmp    loc_3_1184
loc_3_10DC:
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 0x2e0], 7
        je     loc_3_10F1
        mov    eax, 2
        jmp    loc_3_118E
loc_3_10F1:
        push   1
        add    eax, 0x640
        push   eax
        call   sub_3_64E3
        push   0
        push   0
        mov    eax, dword ptr [esi]
        push   -1
        push   dword ptr [eax + 0x29c]
        call   sub_1_04C0
        add    esp, 0x10
        mov    eax, dword ptr [esi]
        push   0
        mov    dword ptr [eax + 0x5ba], 0
        push   0
        push   dword ptr [eax + 0x29c]
        call   sub_1_0490
        add    esp, 0xc
        mov    eax, dword ptr [esi]
        push   0
        mov    edi, dword ptr [eax + 0x29c]
        push   edi
        call   sub_1_0488
        lea    eax, [esp + 0x10]
        add    esp, 8
        push   eax
        push   0
        push   edi
        call   sub_1_0470
        add    esp, 0xc
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 0x2e0], 1
        jmp    loc_3_118C
loc_3_1163:
        mov    eax, dword ptr [esi]
        push   0
        mov    dword ptr [eax + 0x5ba], 0xb0
        mov    ecx, dword ptr [eax + 0x5b6]
        or     ecx, 0xb0
        push   ecx
        push   dword ptr [eax + 0x29c]
loc_3_1184:
        call   sub_1_0490
        add    esp, 0xc
loc_3_118C:
        sub    eax, eax
loc_3_118E:
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_3_1194:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx]
        cmp    dword ptr [eax + 0x2e0], 7
        jne    loc_3_11B0
        mov    eax, dword ptr [eax + 0x5a9]
        mov    dword ptr [ecx + 0xc], eax
        sub    eax, eax
        jmp    loc_3_11BC
loc_3_11B0:
        mov    dword ptr [ecx + 0xc], 0
        mov    eax, 2
loc_3_11BC:
        ret    
loc_3_11BD:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [ecx + 0x2e0]
        sub    eax, 7
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 0x5a9]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx + 0x2b0]
        mov    dword ptr [edx + 0xc], eax
        mov    eax, dword ptr [ecx + 0x2b4]
        mov    dword ptr [edx + 0x10], eax
        sub    eax, eax
        ret    
sub_3_11F4:
        sub    esp, 4
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x14]
        or     eax, eax
        mov    esi, dword ptr [esp + 0x10]
        jne    loc_3_1214
        cmp    word ptr [esi + 0x899], 0
        jne    loc_3_1214
        jmp    loc_3_1302
loc_3_1214:
        mov    dword ptr [esi + 0x2a0], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x2a4], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [esi + 0x2a8], eax
        mov    eax, dword ptr [esp + 0x20]
        or     eax, eax
        je     loc_3_1302
        mov    dword ptr [esi + 0x5e2], eax
        mov    dword ptr [esi + 0x5e6], eax
        push   esi
        call   sub_3_00A2
        add    esp, 4
        lea    edi, [esi + 0x640]
        push   0
        push   0
        push   -1
        push   edi
        call   sub_3_687B
        push   0
        push   0
        push   -1
        push   dword ptr [esi + 0x29c]
        call   sub_1_04C0
        lea    eax, [esp + 0x18]
        add    esp, 0x10
        push   eax
        push   0
        push   dword ptr [esi + 0x29c]
        call   sub_1_0470
        add    esp, 0xc
        push   0
        push   dword ptr [esi + 0x29c]
        call   sub_1_0488
        add    esp, 8
        push   edi
        call   sub_3_6847
        cmp    dword ptr [esi + 0x2e0], 0xd
        je     loc_3_12BD
        push   0
        push   0
        push   5
        push   dword ptr [esi + 0x29c]
        call   sub_1_0480
        add    esp, 0x10
loc_3_12BD:
        mov    dword ptr [esi + 0x5ce], 1
        push   esi
        call   sub_3_130D
        add    esp, 4
        mov    edi, eax
        cmp    edi, 1
        je     loc_3_12FE
        mov    eax, dword ptr [esi + 0x5e2]
        or     eax, eax
        je     loc_3_12FE
        cmp    dword ptr [esi + 0x5de], eax
        je     loc_3_12F4
        push   0
        push   eax
        call   sub_1_04D0
        add    esp, 8
loc_3_12F4:
        mov    dword ptr [esi + 0x5e2], 0
loc_3_12FE:
        mov    eax, edi
        jmp    loc_3_1307
loc_3_1302:
        mov    eax, 2
loc_3_1307:
        pop    edi
        pop    esi
        add    esp, 4
        ret    
sub_3_130D:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   1
        push   0x100
        call   sub_1_04C8
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_3_1330
        mov    eax, 2
        jmp    loc_3_151F
loc_3_1330:
        mov    ebx, dword ptr [esp + 0x14]
        cmp    dword ptr [ebx + 0x5e2], 0
        je     loc_3_1512
        mov    edi, dword ptr [ebx + 0x5e6]
        cmp    byte ptr [edi], 0
        je     loc_3_1512
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 0x3e8], ecx
        mov    eax, dword ptr [ebx + 0x5e6]
        mov    dword ptr [ebx + 0x5ea], eax
        add    ecx, eax
        inc    ecx
        mov    dword ptr [ebx + 0x5e6], ecx
        lea    esi, [ebx + 0x2e8]
        push   0
        push   0
        lea    eax, [ebx + 0x3e8]
        push   eax
        push   esi
        push   ebp
        call   sub_3_2F1C
        add    esp, 0x14
        or     eax, eax
        jne    loc_3_13D6
        mov    dword ptr [ebx + 0x2e0], 1
        push   0
        push   ebp
        call   sub_1_04D0
        add    esp, 8
        mov    eax, 2
        jmp    loc_3_151F
loc_3_13D6:
        mov    ecx, esi
        mov    edx, 0x87c                    ; -> loc_1_087C
loc_3_13DD:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    loc_3_13FD
        or     al, al
        je     loc_3_13F9
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    loc_3_13FD
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    loc_3_13DD
loc_3_13F9:
        sub    eax, eax
        jmp    loc_3_1402
loc_3_13FD:
        sbb    eax, eax
        sbb    eax, -1
loc_3_1402:
        or     eax, eax
        je     loc_3_149C
        mov    eax, dword ptr [ebx + 0x2e0]
        cmp    eax, 5
        je     loc_3_141F
        cmp    eax, 9
        mov    eax, 0
        jne    loc_3_1424
loc_3_141F:
        mov    eax, 1
loc_3_1424:
        push   eax
        push   dword ptr [ebx + 0x3e8]
        push   esi
        call   sub_3_33D7
        add    esp, 0xc
        lea    eax, [ebx + 0x640]
        push   eax
        call   sub_3_6847
        lea    eax, [ebx + 0x3e8]
        push   eax
        push   dword ptr [ebx + 0x3e8]
        push   esi
        push   dword ptr [ebx + 0x29c]
        call   sub_1_04A0
        add    esp, 0x10
        or     eax, eax
        jne    loc_3_147F
        mov    dword ptr [ebx + 0x2e0], 1
        push   0
        push   ebp
        call   sub_1_04D0
        add    esp, 8
        mov    eax, 2
        jmp    loc_3_151F
loc_3_147F:
        mov    eax, dword ptr [ebx + 0x3e8]
        or     eax, eax
        je     loc_3_1491
        add    dword ptr [ebx + 0x2b4], eax
        jmp    loc_3_149C
loc_3_1491:
        push   0
        push   ebx
        call   sub_3_3349
        add    esp, 8
loc_3_149C:
        mov    eax, dword ptr [ebx + 0x5e6]
        mov    edx, 0x870                    ; -> obj1:0x0870
loc_3_14A7:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    loc_3_14C7
        or     cl, cl
        je     loc_3_14C3
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    loc_3_14C7
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    loc_3_14A7
loc_3_14C3:
        sub    eax, eax
        jmp    loc_3_14CC
loc_3_14C7:
        sbb    eax, eax
        sbb    eax, -1
loc_3_14CC:
        or     eax, eax
        jne    loc_3_14ED
        push   0
        push   ebx
        call   sub_3_1524
        add    esp, 8
        push   0
        push   ebp
        call   sub_1_04D0
        add    esp, 8
        mov    eax, 1
        jmp    loc_3_151F
loc_3_14ED:
        mov    dword ptr [ebx + 0x4ec], 1
        push   ebx
        call   sub_3_1E80
        add    esp, 4
        push   0
        push   ebp
        call   sub_1_04D0
        add    esp, 8
        mov    eax, 1
        jmp    loc_3_151F
loc_3_1512:
        push   0
        push   ebp
        call   sub_1_04D0
        add    esp, 8
        sub    eax, eax
loc_3_151F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_1524:
        sub    esp, 0x38
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x48]
        mov    dword ptr [esi + 0x4ec], 0xe
        mov    eax, dword ptr [esi + 0x2e0]
        sub    eax, 2
        cmp    eax, 0x17
        ja     loc_3_154D
        jmp    dword ptr [eax*4 + 0x1dec]    ; -> obj3:0x1DEC
loc_3_154D:
        mov    edi, 2
        mov    dword ptr [esi + 0x2e0], 1
        jmp    loc_3_1CBA
loc_3_1561:
        mov    edi, dword ptr [esp + 0x4c]
        or     edi, edi
        je     loc_3_1587
        cmp    edi, 4
        jb     loc_3_1573
        cmp    edi, 7
        jbe    loc_3_1596
loc_3_1573:
        mov    edi, 2
        mov    dword ptr [esi + 0x2e0], 1
        jmp    loc_3_1CBA
loc_3_1587:
        mov    dword ptr [esi + 0x2e0], 3
        jmp    loc_3_1CBA
loc_3_1596:
        mov    eax, dword ptr [esi + 0x5ce]
        cmp    eax, 5
        jae    loc_3_1573
        inc    eax
        mov    dword ptr [esi + 0x5ce], eax
        mov    eax, dword ptr [esi + 0x5ea]
        mov    dword ptr [esi + 0x5e6], eax
        push   esi
        call   sub_3_130D
        add    esp, 4
        or     eax, eax
        je     loc_3_15D4
        cmp    eax, 1
        je     loc_3_1DE5
        cmp    eax, 2
        je     loc_3_15E3
        jmp    loc_3_1CBA
loc_3_15D4:
        mov    dword ptr [esi + 0x2e0], 3
        jmp    loc_3_1CBA
loc_3_15E3:
        mov    dword ptr [esi + 0x2e0], 1
        jmp    loc_3_1CBA
loc_3_15F2:
        mov    edi, dword ptr [esp + 0x4c]
        or     edi, edi
        jne    loc_3_1634
        cmp    dword ptr [esi + 0x5be], 0
        je     loc_3_1625
        push   edi
        push   esi
        call   sub_3_063B
        add    esp, 8
        mov    dword ptr [esi + 0x4ec], 2
        push   esi
        call   sub_3_1E80
        add    esp, 4
        jmp    loc_3_1DE5
loc_3_1625:
        mov    dword ptr [esi + 0x2e0], 3
        jmp    loc_3_1CBA
loc_3_1634:
        mov    edi, 2
        mov    dword ptr [esi + 0x2e0], 1
        jmp    loc_3_1CBA
loc_3_1648:
        mov    edi, dword ptr [esp + 0x4c]
        or     edi, edi
        je     loc_3_166E
        cmp    edi, 4
        jb     loc_3_165A
        cmp    edi, 7
        jbe    loc_3_167D
loc_3_165A:
        mov    edi, 2
        mov    dword ptr [esi + 0x2e0], 1
        jmp    loc_3_1CBA
loc_3_166E:
        mov    dword ptr [esi + 0x2e0], 8
        jmp    loc_3_1CBA
loc_3_167D:
        mov    dword ptr [esi + 0x2e0], 3
        jmp    loc_3_1CBA
loc_3_168C:
        mov    edi, dword ptr [esp + 0x4c]
        or     edi, edi
        je     loc_3_16B6
        cmp    edi, 4
        jb     loc_3_16A2
        cmp    edi, 7
        jbe    loc_3_1895
loc_3_16A2:
        mov    edi, 2
        mov    dword ptr [esi + 0x2e0], 1
        jmp    loc_3_1CBA
loc_3_16B6:
        mov    dword ptr [esi + 0x2e0], 7
        cmp    dword ptr [esi + 0x5a9], 0
        jne    loc_3_1756
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esi + 0x29c]
        call   sub_1_0460
        add    esp, 8
        or     eax, eax
        jne    loc_3_16F8
        mov    dword ptr [esi + 0x2e0], 1
        mov    edi, 2
        jmp    loc_3_1CBA
loc_3_16F8:
        mov    eax, dword ptr [esi + 0x5ca]
        or     eax, eax
        je     loc_3_174C
        mov    dword ptr [esi + 0x5a9], eax
        cmp    dword ptr [esp + 0x18], eax
        je     loc_3_1756
        mov    dword ptr [esp + 0x18], eax
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_3_34E7
        add    esp, 4
        lea    eax, [esp + 0x14]
        push   -1
        push   eax
        push   dword ptr [esi + 0x29c]
        call   sub_1_0458
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_1756
        mov    dword ptr [esi + 0x2e0], 1
        mov    edi, 2
        jmp    loc_3_1CBA
loc_3_174C:
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x5a9], eax
loc_3_1756:
        mov    dword ptr [esi + 0x5ba], 0xb0
        push   0
        mov    eax, dword ptr [esi + 0x5b6]
        or     eax, 0xb0
        push   eax
        push   dword ptr [esi + 0x29c]
        call   sub_1_0490
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x5a9]
        or     eax, eax
        je     loc_3_17AA
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_17F9
        push   eax
        movzx  eax, word ptr [0x72]          ; -> obj2:0x0072
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 0xc
        jmp    loc_3_17F3
loc_3_17AA:
        mov    eax, dword ptr [esi + 0x5ca]
        or     eax, eax
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        je     loc_3_17D8
        or     ecx, ecx
        je     loc_3_17F9
        push   eax
        movzx  eax, word ptr [0x72]          ; -> obj2:0x0072
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 0xc
        jmp    loc_3_17F3
loc_3_17D8:
        or     ecx, ecx
        je     loc_3_17F9
        movzx  eax, word ptr [0x70]          ; -> obj2:0x0070
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_17F3:
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
loc_3_17F9:
        mov    ebx, dword ptr [esi + 0x5a5]
        test   bl, 2
        je     loc_3_1823
        test   bl, 8
        je     loc_3_1816
        or     ecx, ecx
        je     loc_3_1844
        movzx  eax, word ptr [0x76]          ; -> obj2:0x0076
        jmp    loc_3_182E
loc_3_1816:
        or     ecx, ecx
        je     loc_3_1844
        movzx  eax, word ptr [0x78]          ; -> obj2:0x0078
        jmp    loc_3_182E
loc_3_1823:
        or     ecx, ecx
        je     loc_3_1844
        movzx  eax, word ptr [0x74]          ; -> obj2:0x0074
loc_3_182E:
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
loc_3_1844:
        test   bl, 1
        je     loc_3_185A
        or     ecx, ecx
        je     loc_3_1CBA
        movzx  eax, word ptr [0x7a]          ; -> obj2:0x007A
        jmp    loc_3_1869
loc_3_185A:
        or     ecx, ecx
        je     loc_3_1CBA
        movzx  eax, word ptr [0x7c]          ; -> loc_2_007C
loc_3_1869:
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_1CBA
        push   ecx
        call   sub_1_0590
        add    esp, 4
        jmp    loc_3_1CBA
loc_3_1895:
        mov    dword ptr [esi + 0x2e0], 3
        jmp    loc_3_1CBA
loc_3_18A4:
        mov    edi, dword ptr [esp + 0x4c]
        or     edi, edi
        je     loc_3_18C2
        cmp    edi, 4
        jb     loc_3_18B6
        cmp    edi, 7
        jbe    loc_3_18C2
loc_3_18B6:
        mov    dword ptr [esi + 0x2e0], 1
        jmp    loc_3_18CC
loc_3_18C2:
        mov    dword ptr [esi + 0x2e0], 3
loc_3_18CC:
        mov    eax, dword ptr [esi + 0x2a8]
        and    eax, 0xffff
        mov    dword ptr [esi + 0x2a8], eax
        or     eax, 0xffff0000
        mov    dword ptr [esi + 0x2a8], eax
        mov    edi, 3
        jmp    loc_3_1CBA
loc_3_18F2:
        mov    eax, dword ptr [esi + 0x2ac]
        mov    al, byte ptr [eax]
        test   al, 0x80
        je     loc_3_19CF
        test   al, 0x20
        je     loc_3_19CF
        cmp    dword ptr [esi + 0x618], 0
        je     loc_3_19BB
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_1938
        movzx  eax, word ptr [0xc8]          ; -> loc_2_00C8
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_1938:
        push   0
        push   3
        push   0x884                         ; -> loc_1_0884
        call   sub_3_33D7
        lea    eax, [esp + 0x18]
        add    esp, 0xc
        push   eax
        push   3
        push   0x884                         ; -> loc_1_0884
        push   dword ptr [esi + 0x29c]
        call   sub_1_04A0
        add    esp, 0x10
        or     eax, eax
        jne    loc_3_197B
        mov    dword ptr [esi + 0x2e0], 1
        mov    edi, 2
        jmp    loc_3_1CBA
loc_3_197B:
        cmp    dword ptr [esp + 0xc], 0
        je     loc_3_198E
        mov    eax, dword ptr [esp + 0xc]
        add    dword ptr [esi + 0x2b4], eax
        jmp    loc_3_1999
loc_3_198E:
        push   0
        push   esi
        call   sub_3_3349
        add    esp, 8
loc_3_1999:
        mov    dword ptr [esi + 0x4ec], 1
        mov    dword ptr [esi + 0x2e0], 0xc
        push   esi
        call   sub_3_1E80
        add    esp, 4
        jmp    loc_3_1DE5
loc_3_19BB:
        mov    dword ptr [esi + 0x2e0], 1
        mov    edi, 2
        jmp    loc_3_1CBA
loc_3_19CF:
        push   0
        push   0
        push   5
        push   dword ptr [esi + 0x29c]
        call   sub_1_0480
        add    esp, 0x10
        cmp    dword ptr [esi + 0x618], 0
        je     loc_3_1A0E
        mov    dword ptr [esi + 0x4ec], 2
        mov    dword ptr [esi + 0x2e0], 0xc
        push   esi
        call   sub_3_1E80
        add    esp, 4
        jmp    loc_3_1DE5
loc_3_1A0E:
        mov    dword ptr [esi + 0x2e0], 3
        sub    edi, edi
        jmp    loc_3_1CBA
loc_3_1A1F:
        mov    edi, dword ptr [esp + 0x4c]
        cmp    edi, 2
        je     loc_3_1A85
        mov    dword ptr [esi + 0x2e0], 0xd
        mov    eax, dword ptr [esi + 0x5e2]
        or     eax, eax
        je     loc_3_1A59
        cmp    dword ptr [esi + 0x5de], eax
        je     loc_3_1A4F
        push   0
        push   eax
        call   sub_1_04D0
        add    esp, 8
loc_3_1A4F:
        mov    dword ptr [esi + 0x5e2], 0
loc_3_1A59:
        push   dword ptr [esi + 0x5de]
        push   dword ptr [esi + 0x2a8]
        push   dword ptr [esi + 0x2a4]
        push   dword ptr [esi + 0x2a0]
        push   esi
        call   sub_3_11F4
        add    esp, 0x14
        mov    edi, eax
        cmp    edi, 1
        je     loc_3_1DE5
loc_3_1A85:
        or     edi, edi
        jne    loc_3_1A98
        mov    dword ptr [esi + 0x2e0], 3
        jmp    loc_3_1CBA
loc_3_1A98:
        mov    dword ptr [esi + 0x2e0], 1
        jmp    loc_3_1CBA
loc_3_1AA7:
        mov    edi, dword ptr [esp + 0x4c]
        cmp    edi, 2
        je     loc_3_1AEC
        push   0
        push   0
        push   5
        push   dword ptr [esi + 0x29c]
        call   sub_1_0480
        add    esp, 0x10
        mov    dword ptr [esi + 0x2e0], 3
        push   0
        push   edi
        push   esi
        call   sub_3_7475
        add    esp, 0xc
        push   2
        push   esi
        call   sub_3_75C1
        add    esp, 8
        sub    edi, edi
        jmp    loc_3_1CBA
loc_3_1AEC:
        mov    eax, dword ptr [esi + 0x5d2]
        cmp    eax, 3
        jae    loc_3_1B55
        inc    eax
        mov    dword ptr [esi + 0x5d2], eax
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_1B1F
        movzx  eax, word ptr [0x2c]          ; -> obj2:0x002C
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_1B1F:
        push   0
        push   0
        push   6
        push   dword ptr [esi + 0x29c]
        call   sub_1_0480
        add    esp, 0x10
        mov    dword ptr [esi + 0x2e0], 0xb
        mov    dword ptr [esi + 0x4ec], 0xf
        push   esi
        call   sub_3_1E80
        add    esp, 4
        jmp    loc_3_1DE5
loc_3_1B55:
        mov    dword ptr [esi + 0x2e0], 1
        jmp    loc_3_1CBA
loc_3_1B64:
        mov    edi, dword ptr [esp + 0x4c]
        or     edi, edi
        je     loc_3_1B8A
        cmp    edi, 4
        jb     loc_3_1B76
        cmp    edi, 7
        jbe    loc_3_1B99
loc_3_1B76:
        mov    edi, 2
        mov    dword ptr [esi + 0x2e0], 1
        jmp    loc_3_1CBA
loc_3_1B8A:
        mov    dword ptr [esi + 0x2e0], 0xf
        jmp    loc_3_1CBA
loc_3_1B99:
        mov    dword ptr [esi + 0x2e0], 3
        jmp    loc_3_1CBA
loc_3_1BA8:
        mov    edi, dword ptr [esp + 0x4c]
        or     edi, edi
        je     loc_3_1BC4
        mov    edi, 2
        mov    dword ptr [esi + 0x2e0], 1
        jmp    loc_3_1CBA
loc_3_1BC4:
        mov    dword ptr [esi + 0x2e0], 0x18
        jmp    loc_3_1CBA
loc_3_1BD3:
        mov    edi, dword ptr [esp + 0x4c]
        or     edi, edi
        je     loc_3_1BF9
        cmp    edi, 4
        jb     loc_3_1BE5
        cmp    edi, 7
        jbe    loc_3_1C08
loc_3_1BE5:
        mov    edi, 2
        mov    dword ptr [esi + 0x2e0], 1
        jmp    loc_3_1CBA
loc_3_1BF9:
        mov    dword ptr [esi + 0x2e0], 0x13
        jmp    loc_3_1CBA
loc_3_1C08:
        mov    dword ptr [esi + 0x2e0], 3
        jmp    loc_3_1CBA
loc_3_1C17:
        mov    dword ptr [esi + 0x2e0], 0x15
        mov    edi, dword ptr [esp + 0x4c]
        jmp    loc_3_1CBA
loc_3_1C2A:
        push   1
        push   dword ptr [esp + 0x50]
        push   esi
        call   sub_3_7475
        add    esp, 0xc
        jmp    loc_3_1DE5
loc_3_1C3E:
        push   dword ptr [esi + 0x810]
        call   sub_1_0544
        mov    dword ptr [esi + 0x810], 0
        mov    dword ptr [esi + 0x2e0], 0xd
        mov    eax, dword ptr [esi + 0x5e2]
        or     eax, eax
        je     loc_3_1C84
        cmp    dword ptr [esi + 0x5de], eax
        je     loc_3_1C7A
        push   0
        push   eax
        call   sub_1_04D0
        add    esp, 8
loc_3_1C7A:
        mov    dword ptr [esi + 0x5e2], 0
loc_3_1C84:
        push   dword ptr [esi + 0x5de]
        push   dword ptr [esi + 0x7cf]
        push   dword ptr [esi + 0x7cb]
        push   dword ptr [esi + 0x7c7]
        push   esi
        call   sub_3_11F4
        add    esp, 0x14
        mov    edi, eax
        mov    dword ptr [esi + 0x7c7], 0
        cmp    edi, 1
        je     loc_3_1DE5
loc_3_1CBA:
        mov    ecx, dword ptr [esi + 0x5d6]
        or     ecx, ecx
        je     loc_3_1D10
        mov    eax, dword ptr [esi + 0x5da]
        or     eax, eax
        je     loc_3_1D10
        mov    dword ptr [esi + 0x2e0], ecx
        mov    dword ptr [esi + 0x5e2], 0
        push   eax
        push   dword ptr [esi + 0x2a8]
        push   dword ptr [esi + 0x2a4]
        push   dword ptr [esi + 0x2a0]
        push   esi
        call   sub_3_11F4
        add    esp, 0x14
        mov    edi, eax
        mov    dword ptr [esi + 0x5da], 0
        cmp    edi, 1
        jne    loc_3_1D24
        jmp    loc_3_1DE5
loc_3_1D10:
        mov    dword ptr [esi + 0x5da], 0
        mov    dword ptr [esi + 0x5d6], 0
loc_3_1D24:
        mov    eax, dword ptr [esi + 0x5e2]
        or     eax, eax
        je     loc_3_1D4B
        cmp    dword ptr [esi + 0x5de], eax
        je     loc_3_1D41
        push   0
        push   eax
        call   sub_1_04D0
        add    esp, 8
loc_3_1D41:
        mov    dword ptr [esi + 0x5e2], 0
loc_3_1D4B:
        lea    eax, [esp + 0x10]
        push   eax
        push   0
        push   dword ptr [esi + 0x29c]
        call   sub_1_0470
        add    esp, 0xc
        cmp    dword ptr [esi + 0x2e0], 7
        jne    loc_3_1D93
        mov    ebx, dword ptr [esi + 0x29c]
        push   esi
        push   0x172                         ; -> sub_1_0172
        push   dword ptr [esi + 0x2d0]
        push   ebx
        call   sub_1_04B8
        add    esp, 0x10
        push   esi
        push   0x172                         ; -> sub_1_0172
        push   dword ptr [esi + 0x2dc]
        push   ebx
        jmp    loc_3_1DB1
loc_3_1D93:
        push   0
        push   0
        push   -1
        lea    eax, [esi + 0x640]
        push   eax
        call   sub_3_687B
        push   0
        push   0
        push   -1
        push   dword ptr [esi + 0x29c]
loc_3_1DB1:
        call   sub_1_04C0
        add    esp, 0x10
        cmp    dword ptr [esi + 0x2e0], 0x15
        jne    loc_3_1DDB
        mov    dword ptr [esi + 0x2e0], 0x16
        push   edi
        add    esi, 0x7df
        push   esi
        call   sub_1_076C
        jmp    loc_3_1DE5
loc_3_1DDB:
        push   edi
        push   esi
        call   sub_3_063B
        add    esp, 8
loc_3_1DE5:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x38
        ret    
        ; ---- dispatch table (24 entries) ----
        dd  loc_3_1561
        dd  loc_3_154D
        dd  loc_3_15F2
        dd  loc_3_1648
        dd  loc_3_168C
        dd  loc_3_154D
        dd  loc_3_154D
        dd  loc_3_168C
        dd  loc_3_18A4
        dd  loc_3_18F2
        dd  loc_3_1A1F
        dd  loc_3_1AA7
        dd  loc_3_1B64
        dd  loc_3_154D
        dd  loc_3_1B64
        dd  loc_3_1BA8
        dd  loc_3_1BD3
        dd  loc_3_154D
        dd  loc_3_1C17
        dd  loc_3_154D
        dd  loc_3_154D
        dd  loc_3_1C2A
        dd  loc_3_154D
        dd  loc_3_1C3E
loc_3_1E4C:
        sub    esp, 8
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x810], 0
        lea    eax, [esp + 4]
        push   eax
        push   1
        push   0x904                         ; -> obj1:0x0904
        mov    eax, dword ptr [esp + 0xc]
        push   dword ptr [eax + 0x29c]
        call   sub_1_04A0
        add    esp, 0x18
        ret    
sub_3_1E80:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x34]
loc_3_1E8B:
        cmp    dword ptr [ebx + 0x620], 0
        je     loc_3_1ECD
        push   ebx
        call   sub_3_00A2
        add    esp, 4
        lea    eax, [ebx + 0x640]
        push   0
        push   0
        push   -1
        push   eax
        call   sub_3_687B
        push   0
        push   0
        push   -1
        push   dword ptr [ebx + 0x29c]
        call   sub_1_04C0
        add    esp, 0x10
        mov    dword ptr [ebx + 0x620], 0
loc_3_1ECD:
        mov    eax, dword ptr [ebx + 0x4ec]
        dec    eax
        cmp    eax, 0xf
        ja     loc_3_2B38
        jmp    dword ptr [eax*4 + 0x2b4b]    ; -> obj3:0x2B4B
loc_3_1EE4:
        mov    dword ptr [ebx + 0x4ec], 2
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [ebx + 0x29c]
        call   sub_1_0468
        add    esp, 8
        or     eax, eax
        je     loc_3_2B38
        cmp    dword ptr [esp + 0x2c], 0
        je     loc_3_25D2
        push   ebx
        push   0x54                          ; -> loc_1_0054
        push   1
        push   dword ptr [ebx + 0x29c]
        call   sub_1_04C0
        add    esp, 0x10
        or     eax, eax
        jne    loc_3_2729
        jmp    loc_3_2B38
loc_3_1F37:
        mov    eax, dword ptr [ebx + 0x4f0]
        cmp    eax, 0x100
        jb     loc_3_1F53
        mov    dword ptr [ebx + 0x4ec], 0xa
        jmp    loc_3_1ECD
loc_3_1F53:
        lea    edx, [ebx + 0x640]
        lea    ecx, [esp + 0x14]
        push   ecx
        push   1
        add    eax, ebx
        add    eax, 0x3ec
        push   eax
        push   edx
        call   sub_3_67E7
        or     eax, eax
        je     loc_3_2B38
        mov    eax, dword ptr [esp + 0x14]
        add    dword ptr [ebx + 0x2b0], eax
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_1FA0
        mov    eax, dword ptr [esp + 0x14]
        add    dword ptr [ebx + 0x4f0], eax
        mov    dword ptr [ebx + 0x4ec], 7
        jmp    loc_3_1ECD
loc_3_1FA0:
        mov    dword ptr [ebx + 0x4ec], 5
        jmp    loc_3_1ECD
loc_3_1FAF:
        lea    esi, [ebx + 0x640]
        push   0
        push   0
        push   -1
        push   esi
        call   sub_3_687B
        or     eax, eax
        je     loc_3_2B38
        lea    eax, [esp + 0x10]
        push   eax
        push   1
        mov    eax, dword ptr [ebx + 0x4f0]
        add    eax, ebx
        add    eax, 0x3ec
        push   eax
        push   esi
        call   sub_3_67E7
        or     eax, eax
        je     loc_3_2B38
        mov    eax, dword ptr [esp + 0x10]
        add    dword ptr [ebx + 0x2b0], eax
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_2016
        mov    eax, dword ptr [esp + 0x10]
        add    dword ptr [ebx + 0x4f0], eax
        mov    dword ptr [ebx + 0x4ec], 7
        jmp    loc_3_1ECD
loc_3_2016:
        cmp    dword ptr [ebx + 0x608], 0
        je     loc_3_202E
        mov    dword ptr [ebx + 0x4ec], 0xc
        jmp    loc_3_1ECD
loc_3_202E:
        cmp    dword ptr [ebx + 0x2e0], 0xc
        je     loc_3_2811
        mov    dword ptr [ebx + 0x4ec], 0xa
        jmp    loc_3_1ECD
loc_3_204A:
        cmp    dword ptr [ebx + 0x614], 0
        je     loc_3_2062
        mov    dword ptr [ebx + 0x4ec], 4
        jmp    loc_3_1ECD
loc_3_2062:
        cmp    byte ptr [ebx + 0x840], 0
        je     loc_3_20FA
        mov    dword ptr [ebx + 0x4ec], 4
        mov    esi, dword ptr [ebx + 0x4f0]
        mov    edx, dword ptr [ebx + 0x796]
        mov    eax, esi
        sub    eax, dword ptr [ebx + 0x841]
        cmp    eax, edx
        jb     loc_3_1ECD
        mov    eax, ebx
        sub    eax, edx
        mov    ecx, edx
        shr    ecx, 2
        lea    esi, [eax + esi + 0x3ec]
        mov    edi, dword ptr [ebx + 0x792]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_1ECD
        mov    ecx, edx
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_3_1ECD
        mov    dword ptr [ebx + 0x4ec], 0xb
        mov    al, byte ptr [ebx + 0x840]
        and    al, 0x7f
        mov    byte ptr [esp + 0x18], al
        mov    byte ptr [ebx + 0x840], 0
        mov    eax, dword ptr [ebx + 0x4f0]
        sub    eax, dword ptr [ebx + 0x841]
        sub    eax, dword ptr [ebx + 0x796]
        mov    dword ptr [ebx + 0x845], eax
        jmp    loc_3_1ECD
loc_3_20FA:
        lea    eax, [esp + 0x18]
        push   eax
        push   ebx
        call   sub_3_2E0A
        add    esp, 8
        or     eax, eax
        je     loc_3_2129
        cmp    eax, 1
        je     loc_3_2164
        cmp    eax, 2
        je     loc_3_218A
        cmp    eax, 3
        je     loc_3_2199
        cmp    eax, 4
        je     loc_3_21EF
        jmp    loc_3_2B38
loc_3_2129:
        test   byte ptr [esp + 0x18], 0x80
        je     loc_3_2155
        mov    al, byte ptr [esp + 0x18]
        mov    byte ptr [ebx + 0x840], al
        mov    eax, dword ptr [ebx + 0x4f0]
        mov    dword ptr [ebx + 0x841], eax
        mov    dword ptr [ebx + 0x4ec], 4
        jmp    loc_3_1ECD
loc_3_2155:
        mov    dword ptr [ebx + 0x4ec], 0xb
        jmp    loc_3_1ECD
loc_3_2164:
        push   2
        push   dword ptr [ebx + 0x4f0]
        lea    eax, [ebx + 0x3ec]
        push   eax
        call   sub_3_33D7
        add    esp, 0xc
        mov    dword ptr [ebx + 0x4ec], 2
        jmp    loc_3_1ECD
loc_3_218A:
        mov    dword ptr [ebx + 0x4ec], 4
        jmp    loc_3_1ECD
loc_3_2199:
        cmp    dword ptr [ebx + 0x608], 0
        je     loc_3_21B1
        mov    dword ptr [ebx + 0x4ec], 0xc
        jmp    loc_3_1ECD
loc_3_21B1:
        cmp    dword ptr [ebx + 0x2e0], 4
        jne    loc_3_21E0
        push   2
        push   dword ptr [ebx + 0x4f0]
        lea    eax, [ebx + 0x3ec]
        push   eax
        call   sub_3_33D7
        add    esp, 0xc
        mov    dword ptr [ebx + 0x4ec], 2
        jmp    loc_3_1ECD
loc_3_21E0:
        mov    dword ptr [ebx + 0x4ec], 9
        jmp    loc_3_1ECD
loc_3_21EF:
        mov    dword ptr [ebx + 0x4ec], 4
        lea    esi, [esp + 0x18]
        lea    edi, [ebx + 0x5fe]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    eax, dword ptr [ebx + 0x4f0]
        mov    dword ptr [ebx + 0x608], eax
        jmp    loc_3_1ECD
loc_3_2218:
        mov    dword ptr [ebx + 0x608], 0
        mov    dword ptr [ebx + 0x614], 1
        mov    dword ptr [ebx + 0x4ec], 4
        jmp    loc_3_1ECD
loc_3_223B:
        cmp    dword ptr [ebx + 0x2e0], 2
        jne    loc_3_2253
        mov    eax, dword ptr [ebx + 0x2ac]
        test   byte ptr [eax], 0x80
        jne    loc_3_267B
loc_3_2253:
        cmp    dword ptr [ebx + 0x5ce], 5
        push   2
        push   dword ptr [ebx + 0x4f0]
        lea    eax, [ebx + 0x3ec]
        push   eax
        jae    loc_3_2901
        call   sub_3_33D7
        add    esp, 0xc
        cmp    dword ptr [0xc00], 0          ; -> loc_1_0C00
        je     loc_3_229C
        movzx  eax, word ptr [0xc4]          ; -> loc_2_00C4
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   dword ptr [0xc00]             ; -> loc_1_0C00
        call   sub_1_0588
        add    esp, 8
loc_3_229C:
        inc    dword ptr [ebx + 0x5ce]
        mov    dword ptr [ebx + 0x4ec], 8
        jmp    loc_3_1ECD
loc_3_22B1:
        push   2
        push   dword ptr [ebx + 0x4f0]
        lea    eax, [ebx + 0x3ec]
        push   eax
        call   sub_3_33D7
        add    esp, 0xc
        mov    dword ptr [ebx + 0x4ec], 0xd
        jmp    loc_3_1ECD
loc_3_22D7:
        lea    esi, [ebx + 0x5fe]
        lea    edi, [esp + 0x18]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    eax, dword ptr [ebx + 0x608]
        lea    edi, [ebx + eax + 0x3ec]
        mov    eax, dword ptr [ebx + 0x4f0]
        lea    ebp, [ebx + eax + 0x3eb]
        cmp    ebp, edi
        jb     loc_3_2318
        lea    esi, [ebx + 0x640]
loc_3_2309:
        mov    al, byte ptr [ebp]
        push   eax
        push   esi
        call   sub_3_67B3
        dec    ebp
        cmp    ebp, edi
        jae    loc_3_2309
loc_3_2318:
        push   2
        push   dword ptr [ebx + 0x608]
        lea    eax, [ebx + 0x3ec]
        push   eax
        call   sub_3_33D7
        add    esp, 0xc
        mov    dword ptr [ebx + 0x4ec], 0xd
        jmp    loc_3_1ECD
loc_3_233E:
        cmp    dword ptr [0xc00], 0          ; -> loc_1_0C00
        je     loc_3_238E
        cmp    byte ptr [esp + 0x18], 0
        jae    loc_3_235E
        cmp    byte ptr [esp + 0x18], 0x1a
        jbe    loc_3_235E
        movzx  eax, word ptr [0x38]          ; -> obj2:0x0038
        jmp    loc_3_236B
loc_3_235E:
        movzx  eax, byte ptr [esp + 0x18]
        movzx  eax, word ptr [eax*2 + 0x3a]  ; -> obj2:0x003A
loc_3_236B:
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        movzx  eax, word ptr [0x36]          ; -> obj2:0x0036
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   dword ptr [0xc00]             ; -> loc_1_0C00
        call   sub_1_0588
        add    esp, 0xc
loc_3_238E:
        mov    al, byte ptr [esp + 0x19]
        and    eax, 0xb
        or     dword ptr [ebx + 0x5a5], eax
        cmp    dword ptr [esp + 0x1a], 0
        je     loc_3_23AC
        mov    eax, dword ptr [esp + 0x1a]
        mov    dword ptr [ebx + 0x5a9], eax
loc_3_23AC:
        cmp    dword ptr [esp + 0x1e], 0
        je     loc_3_23BD
        mov    eax, dword ptr [esp + 0x1e]
        mov    dword ptr [ebx + 0x5ca], eax
loc_3_23BD:
        movzx  eax, byte ptr [esp + 0x18]
        cmp    eax, 0x1a
        ja     loc_3_2B38
        jmp    dword ptr [eax*4 + 0x2b8b]    ; -> obj3:0x2B8B
loc_3_23D2:
        sub    edx, edx
        sub    esi, esi
        mov    ecx, dword ptr [ebx + 0x845]
        cmp    ecx, edx
        je     loc_3_23FE
        mov    edi, dword ptr [ebx + 0x841]
        add    edi, ebx
loc_3_23E8:
        mov    al, byte ptr [edi + esi + 0x3ec]
        sub    al, 0x30
        imul   edx, edx, 0xa
        movsx  eax, al
        add    edx, eax
        inc    esi
        cmp    esi, ecx
        jb     loc_3_23E8
loc_3_23FE:
        cmp    byte ptr [esp + 0x18], 0x12
        jne    loc_3_2407
        neg    edx
loc_3_2407:
        mov    eax, dword ptr [ebx + 0x702]
        cmp    eax, 0x10
        jae    loc_3_241F
        mov    dword ptr [ebx + eax*4 + 0x706], edx
        inc    dword ptr [ebx + 0x702]
loc_3_241F:
        mov    dword ptr [ebx + 0x4ec], 3
        jmp    loc_3_1ECD
loc_3_242E:
        mov    dword ptr [ebx + 0x4ec], 3
        jmp    loc_3_1ECD
loc_3_243D:
        mov    ebp, dword ptr [ebx + 0x845]
        cmp    ebp, 0xa
        jb     loc_3_244D
        mov    ebp, 0xa
loc_3_244D:
        lea    edx, [ebx + 0x746]
        mov    eax, dword ptr [ebx + 0x841]
        mov    edi, edx
        lea    esi, [eax + ebx + 0x3ec]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebx + 0x845]
        mov    byte ptr [ebx + eax + 0x746], 0
        mov    eax, dword ptr [ebx + 0x79a]
        or     eax, eax
        je     loc_3_24BF
        mov    esi, edx
loc_3_248A:
        mov    cl, byte ptr [esi]
        cmp    cl, byte ptr [eax]
        jne    loc_3_24AA
        or     cl, cl
        je     loc_3_24A6
        mov    cl, byte ptr [esi + 1]
        cmp    cl, byte ptr [eax + 1]
        jne    loc_3_24AA
        add    esi, 2
        add    eax, 2
        or     cl, cl
        jne    loc_3_248A
loc_3_24A6:
        sub    eax, eax
        jmp    loc_3_24AF
loc_3_24AA:
        sbb    eax, eax
        sbb    eax, -1
loc_3_24AF:
        or     eax, eax
        jne    loc_3_24BF
        mov    dword ptr [ebx + 0x6fa], 1
        jmp    loc_3_2516
loc_3_24BF:
        mov    eax, dword ptr [ebx + 0x79e]
        or     eax, eax
        je     loc_3_24FE
loc_3_24C9:
        mov    cl, byte ptr [edx]
        cmp    cl, byte ptr [eax]
        jne    loc_3_24E9
        or     cl, cl
        je     loc_3_24E5
        mov    cl, byte ptr [edx + 1]
        cmp    cl, byte ptr [eax + 1]
        jne    loc_3_24E9
        add    edx, 2
        add    eax, 2
        or     cl, cl
        jne    loc_3_24C9
loc_3_24E5:
        sub    eax, eax
        jmp    loc_3_24EE
loc_3_24E9:
        sbb    eax, eax
        sbb    eax, -1
loc_3_24EE:
        or     eax, eax
        jne    loc_3_24FE
        mov    dword ptr [ebx + 0x6fa], 2
        jmp    loc_3_2516
loc_3_24FE:
        mov    eax, dword ptr [ebx + 0x6fa]
        and    eax, 0xffffffef
        mov    dword ptr [ebx + 0x6fa], eax
        or     eax, 4
        mov    dword ptr [ebx + 0x6fa], eax
loc_3_2516:
        mov    dword ptr [ebx + 0x4ec], 3
        jmp    loc_3_1ECD
loc_3_2525:
        mov    edx, dword ptr [ebx + 0x845]
        cmp    edx, 0x40
        jb     loc_3_2535
        mov    edx, 0x40
loc_3_2535:
        mov    eax, dword ptr [ebx + 0x841]
        lea    edi, [ebx + 0x751]
        lea    esi, [eax + ebx + 0x3ec]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebx + 0x845]
        mov    byte ptr [ebx + eax + 0x751], 0
        mov    eax, dword ptr [ebx + 0x6fa]
        and    eax, 0xffffffef
        mov    dword ptr [ebx + 0x6fa], eax
        or     eax, 8
        mov    dword ptr [ebx + 0x6fa], eax
        mov    dword ptr [ebx + 0x4ec], 3
        jmp    loc_3_1ECD
loc_3_258B:
        or     dword ptr [ebx + 0x6fa], 0x20
        movzx  eax, byte ptr [esp + 0x18]
        sub    eax, 0x18
        mov    dword ptr [ebx + 0x706], eax
loc_3_25A0:
        inc    dword ptr [ebx + 0x6fe]
        mov    eax, dword ptr [ebx + 0x2e0]
        cmp    eax, 6
        je     loc_3_25C3
        cmp    eax, 9
        je     loc_3_25C3
        cmp    dword ptr [ebx + 0x5e2], 0
        jne    loc_3_2A50
loc_3_25C3:
        mov    dword ptr [ebx + 0x4ec], 3
        jmp    loc_3_1ECD
loc_3_25D2:
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [ebx + 0x29c]
        call   sub_1_0468
        add    esp, 8
        or     eax, eax
        je     loc_3_2B38
        cmp    dword ptr [esp + 0x2c], 1
        ja     loc_3_2B18
        push   0
        push   0
        push   -1
        push   dword ptr [ebx + 0x29c]
        call   sub_1_04C0
        add    esp, 0x10
        or     eax, eax
        je     loc_3_2B38
        cmp    dword ptr [ebx + 0x2e0], 0x19
        jne    loc_3_2633
        push   ebx
        push   0x12c
        push   0x1e4c                        ; -> loc_3_1E4C
        call   sub_1_0528
        mov    dword ptr [ebx + 0x810], eax
loc_3_2633:
        mov    dword ptr [ebx + 0x4f0], 0
        mov    dword ptr [ebx + 0x608], 0
        mov    dword ptr [ebx + 0x4ec], 4
        mov    dword ptr [ebx + 0x610], 0
        mov    dword ptr [ebx + 0x614], 0
        mov    byte ptr [ebx + 0x840], 0
        mov    dword ptr [ebx + 0x841], 0
        jmp    loc_3_1ECD
loc_3_267B:
        cmp    dword ptr [ebx + 0x5d6], 0
        jne    loc_3_2B38
        cmp    dword ptr [0xc00], 0          ; -> loc_1_0C00
        je     loc_3_26AD
        movzx  eax, word ptr [0xca]          ; -> loc_2_00CA
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   dword ptr [0xc00]             ; -> loc_1_0C00
        call   sub_1_0588
        add    esp, 8
loc_3_26AD:
        mov    dword ptr [ebx + 0x5d6], 2
        mov    eax, dword ptr [ebx + 0x5e2]
        mov    dword ptr [ebx + 0x5da], eax
        mov    dword ptr [ebx + 0x5e2], 0
        mov    dword ptr [ebx + 0x5d2], 1
        cmp    dword ptr [0xc00], 0          ; -> loc_1_0C00
        je     loc_3_26FC
        movzx  eax, word ptr [0x2c]          ; -> obj2:0x002C
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   dword ptr [0xc00]             ; -> loc_1_0C00
        call   sub_1_0588
        add    esp, 8
loc_3_26FC:
        push   0
        push   0
        push   6
        push   dword ptr [ebx + 0x29c]
        call   sub_1_0480
        add    esp, 0x10
        mov    dword ptr [ebx + 0x2e0], 0xb
        mov    dword ptr [ebx + 0x4ec], 0xf
        jmp    loc_3_1E8B
loc_3_2729:
        push   ebx
        push   0x7d0
        push   0xb                           ; -> loc_1_000B
        call   sub_1_0528
        mov    dword ptr [ebx + 0x4f4], eax
        or     eax, eax
        jne    loc_3_2B43
        jmp    loc_3_2B38
loc_3_274C:
        cmp    dword ptr [ebx + 0x4f0], 0
        jne    loc_3_27BC
        mov    eax, dword ptr [ebx + 0x2e0]
        sub    eax, 2
        cmp    eax, 0x18
        ja     loc_3_2B38
        jmp    dword ptr [eax*4 + 0x2bf7]    ; -> obj3:0x2BF7
loc_3_276E:
        mov    esi, 0x7d0
        jmp    loc_3_27C1
loc_3_2775:
        sub    esi, esi
        jmp    loc_3_27C1
loc_3_2779:
        cmp    dword ptr [ebx + 0x528], 0
        je     loc_3_2797
        mov    esi, dword ptr [ebx + 0x591]
        or     esi, esi
        je     loc_3_2797
        add    esi, 0xa
        imul   esi, esi, 0x3e8
        jmp    loc_3_27C1
loc_3_2797:
        cmp    dword ptr [ebx + 0x618], 1
        sbb    esi, esi
        and    esi, 0xffff1d70
        add    esi, 0xea60
        jmp    loc_3_27C1
loc_3_27AE:
        mov    esi, 0xbb8
        jmp    loc_3_27C1
loc_3_27B5:
        mov    esi, 0x1388
        jmp    loc_3_27C1
loc_3_27BC:
        mov    esi, 0x3e8
loc_3_27C1:
        push   ebx
        push   0x1e80                        ; -> sub_3_1E80
        push   1
        lea    eax, [ebx + 0x640]
        push   eax
        call   sub_3_687B
        or     eax, eax
        je     loc_3_2B38
        or     esi, esi
        je     loc_3_2802
        push   ebx
        push   esi
        push   0xb                           ; -> loc_1_000B
        call   sub_1_0528
        mov    dword ptr [ebx + 0x4f4], eax
        or     eax, eax
        jne    loc_3_2802
        push   2
        push   ebx
        call   sub_3_1524
        add    esp, 8
loc_3_2802:
        mov    dword ptr [ebx + 0x4ec], 6
        jmp    loc_3_2B43
loc_3_2811:
        push   0
        jmp    loc_3_2B3A
loc_3_2818:
        cmp    dword ptr [ebx + 0x618], 0
        je     loc_3_28CD
        mov    eax, dword ptr [ebx + 0x2e0]
        cmp    eax, 6
        je     loc_3_28B7
        cmp    eax, 9
        je     loc_3_28B7
        cmp    eax, 5
        jne    loc_3_28CD
        mov    dword ptr [ebx + 0x61c], 2
        mov    dword ptr [ebx + 0x2e0], 0xd
        mov    dword ptr [ebx + 0x5d2], 1
        mov    eax, dword ptr [ebx + 0x5e2]
        or     eax, eax
        je     loc_3_2887
        cmp    dword ptr [ebx + 0x5de], eax
        je     loc_3_287D
        push   0
        push   eax
        call   sub_1_04D0
        add    esp, 8
loc_3_287D:
        mov    dword ptr [ebx + 0x5e2], 0
loc_3_2887:
        push   dword ptr [ebx + 0x5de]
        push   dword ptr [ebx + 0x2a8]
        push   dword ptr [ebx + 0x2a4]
        push   dword ptr [ebx + 0x2a0]
        push   ebx
        call   sub_3_11F4
        add    esp, 0x14
        cmp    eax, 1
        je     loc_3_2B43
        push   eax
        jmp    loc_3_2B3A
loc_3_28B7:
        mov    eax, dword ptr [ebx + 0x2ac]
        test   byte ptr [eax], 0x80
        je     loc_3_2B38
        push   0
        jmp    loc_3_2B3A
loc_3_28CD:
        cmp    dword ptr [ebx + 0x5e2], 0
        je     loc_3_28FA
        mov    eax, dword ptr [ebx + 0x5ea]
        mov    dword ptr [ebx + 0x5e6], eax
        push   ebx
        call   sub_3_130D
        add    esp, 4
        cmp    eax, 1
        je     loc_3_2B43
        push   eax
        jmp    loc_3_2B3A
loc_3_28FA:
        push   0
        jmp    loc_3_2B3A
loc_3_2901:
        call   sub_3_33D7
        add    esp, 0xc
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_2B38
        movzx  ecx, word ptr [0xae]          ; -> loc_2_00AE
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
        jmp    loc_3_2B38
loc_3_2932:
        cmp    dword ptr [ebx + 0x2e0], 0xc
        jne    loc_3_29F2
        push   0
        jmp    loc_3_2B3A
loc_3_2946:
        cmp    dword ptr [ebx + 0x2e0], 0xc
        jne    loc_3_2953
        push   0
        jmp    loc_3_2955
loc_3_2953:
        push   2
loc_3_2955:
        push   ebx
        call   sub_3_1524
        add    esp, 8
        cmp    dword ptr [ebx + 0x2e0], 4
        jne    loc_3_2B43
        cmp    word ptr [ebx + 0x89b], 0x80
        jne    loc_3_298C
        push   0xb
        push   ebx
        call   sub_3_3DE5
        add    esp, 8
        push   0
        push   ebx
        call   sub_3_3E17
        add    esp, 8
loc_3_298C:
        mov    eax, dword ptr [ebx + 0x8a9]
        or     eax, eax
        je     loc_3_2B43
        push   eax
        call   sub_1_0510
        jmp    loc_3_2B43
loc_3_29A5:
        push   6
        jmp    loc_3_2B3A
loc_3_29AC:
        push   7
        jmp    loc_3_2B3A
loc_3_29B3:
        push   4
        jmp    loc_3_2B3A
loc_3_29BA:
        push   5
        jmp    loc_3_2B3A
loc_3_29C1:
        movzx  ax, byte ptr [esp + 0x18]
        sub    ax, 8
        push   eax
        push   ebx
        call   sub_3_3DE5
        jmp    loc_3_2B40
loc_3_29D7:
        movzx  ax, byte ptr [esp + 0x18]
        sub    ax, 8
        push   eax
        push   ebx
        call   sub_3_3DE5
        add    esp, 8
        push   0
        jmp    loc_3_2B3A
loc_3_29F2:
        mov    dword ptr [ebx + 0x5ce], 1
        push   ebx
        call   sub_3_130D
        add    esp, 4
        cmp    eax, 1
        je     loc_3_2A14
        push   eax
        push   ebx
        call   sub_3_1524
        add    esp, 8
loc_3_2A14:
        cmp    dword ptr [ebx + 0x2e0], 4
        jne    loc_3_2B43
        cmp    word ptr [ebx + 0x89b], 0x80
        jne    loc_3_2A37
        push   0xa
        push   ebx
        call   sub_3_3DE5
        add    esp, 8
loc_3_2A37:
        mov    eax, dword ptr [ebx + 0x8a9]
        or     eax, eax
        je     loc_3_2B43
        push   eax
        call   sub_1_0510
        jmp    loc_3_2B43
loc_3_2A50:
        cmp    dword ptr [ebx + 0x2e0], 4
        je     loc_3_2A65
        mov    eax, dword ptr [ebx + 0x5ea]
        mov    dword ptr [ebx + 0x5e6], eax
loc_3_2A65:
        mov    dword ptr [ebx + 0x5ce], 1
        push   ebx
        call   sub_3_130D
        add    esp, 4
        cmp    eax, 1
        je     loc_3_2B43
        push   eax
        jmp    loc_3_2B3A
loc_3_2A87:
        cmp    dword ptr [ebx + 0x2e0], 0xb
        je     loc_3_2A9F
        push   2
        push   ebx
        call   sub_3_1524
        add    esp, 8
        sub    eax, eax
        jmp    loc_3_2AC7
loc_3_2A9F:
        mov    dword ptr [ebx + 0x5ba], 0x30
        push   0
        push   0x30
        push   dword ptr [ebx + 0x29c]
        call   sub_1_0490
        add    esp, 0xc
        mov    eax, dword ptr [ebx + 0x5d2]
        imul   eax, eax, 0x4b0
loc_3_2AC7:
        or     eax, eax
        je     loc_3_2B43
        mov    dword ptr [ebx + 0x4ec], 0x10
        push   ebx
        push   eax
        push   0xb                           ; -> loc_1_000B
        call   sub_1_0528
        mov    dword ptr [ebx + 0x4f4], eax
        or     eax, eax
        jne    loc_3_2B43
        jmp    loc_3_2B38
loc_3_2AED:
        cmp    dword ptr [ebx + 0x2e0], 0xb
        je     loc_3_2AF8
        jmp    loc_3_2B38
loc_3_2AF8:
        mov    dword ptr [ebx + 0x5ba], 0
        push   0
        push   0
        push   dword ptr [ebx + 0x29c]
        call   sub_1_0490
        add    esp, 0xc
        push   0
        jmp    loc_3_2B3A
loc_3_2B18:
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_2B38
        movzx  ecx, word ptr [0xac]          ; -> loc_2_00AC
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_2B38:
        push   2
loc_3_2B3A:
        push   ebx
        call   sub_3_1524
loc_3_2B40:
        add    esp, 8
loc_3_2B43:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
        ; ---- dispatch table (68 entries) ----
        dd  loc_3_1EE4
        dd  loc_3_25D2
        dd  loc_3_2633
        dd  loc_3_1F37
        dd  loc_3_274C
        dd  loc_3_1FAF
        dd  loc_3_204A
        dd  loc_3_2818
        dd  loc_3_2218
        dd  loc_3_223B
        dd  loc_3_22B1
        dd  loc_3_22D7
        dd  loc_3_233E
        dd  loc_3_2B43
        dd  loc_3_2A87
        dd  loc_3_2AED
        dd  loc_3_2932
        dd  loc_3_25C3
        dd  loc_3_29F2
        dd  loc_3_2946
        dd  loc_3_29A5
        dd  loc_3_29AC
        dd  loc_3_29B3
        dd  loc_3_29BA
        dd  loc_3_25A0
        dd  loc_3_29C1
        dd  loc_3_29D7
        dd  loc_3_29D7
        dd  loc_3_29D7
        dd  loc_3_29D7
        dd  loc_3_29D7
        dd  loc_3_29D7
        dd  loc_3_29D7
        dd  loc_3_23D2
        dd  loc_3_23D2
        dd  loc_3_242E
        dd  loc_3_242E
        dd  loc_3_243D
        dd  loc_3_2525
        dd  loc_3_242E
        dd  loc_3_258B
        dd  loc_3_258B
        dd  loc_3_258B
        dd  loc_3_276E
        dd  loc_3_2B38
        dd  loc_3_2775
        dd  loc_3_2779
        dd  loc_3_2779
        dd  loc_3_2B38
        dd  loc_3_2B38
        dd  loc_3_2779
        dd  loc_3_276E
        dd  loc_3_2B38
        dd  loc_3_276E
        dd  loc_3_276E
        dd  loc_3_2779
        dd  loc_3_2B38
        dd  loc_3_2779
        dd  loc_3_2779
        dd  loc_3_2779
        dd  loc_3_2B38
        dd  loc_3_27AE
        dd  loc_3_2B38
        dd  loc_3_2B38
        dd  loc_3_27AE
        dd  loc_3_2B38
        dd  loc_3_2779
        dd  loc_3_27B5
sub_3_2C5B:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, 1
        mov    esi, dword ptr [esp + 0x14]
        lea    eax, [esp + 0xc]
        push   eax
        mov    eax, dword ptr [esi + 0x5b6]
        not    eax
        push   eax
        push   dword ptr [esi + 0x29c]
        call   sub_1_0498
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x5b2]
        test   dword ptr [esi + 0x5ba], eax
        je     loc_3_2DE1
        mov    ecx, dword ptr [esi + 0x2e0]
        cmp    ecx, 7
        je     loc_3_2CB1
        cmp    ecx, 0xb
        je     loc_3_2DCE
        jmp    loc_3_2DE1
loc_3_2CB1:
        test   al, 0x80
        je     loc_3_2CC9
        mov    eax, dword ptr [esi + 0x60c]
        or     eax, eax
        je     loc_3_2CC9
        push   eax
        push   esi
        call   sub_3_3349
        add    esp, 8
loc_3_2CC9:
        mov    eax, dword ptr [esi + 0x5b2]
        test   al, 0x20
        je     loc_3_2CDE
        mov    ecx, dword ptr [esi + 0x2ac]
        test   byte ptr [ecx], 0x80
        je     loc_3_2CF5
loc_3_2CDE:
        test   al, 0x10
        je     loc_3_2DE1
        mov    eax, dword ptr [esi + 0x2ac]
        test   byte ptr [eax], 0x20
        jne    loc_3_2DE1
loc_3_2CF5:
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_2D15
        movzx  ecx, word ptr [0x2e]          ; -> obj2:0x002E
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_2D15:
        push   esi
        call   sub_3_00A2
        add    esp, 4
        push   0
        lea    eax, [esi + 0x640]
        push   eax
        call   sub_3_64E3
        push   0
        push   0
        push   -1
        push   dword ptr [esi + 0x29c]
        call   sub_1_04C0
        add    esp, 0x10
        mov    ebx, dword ptr [esi + 0x29c]
        push   0
        push   ebx
        call   sub_1_0488
        add    esp, 8
        mov    dword ptr [esi + 0x5ba], 0
        push   0
        push   0
        push   ebx
        call   sub_1_0490
        add    esp, 0xc
        cmp    dword ptr [esi + 0x618], 0
        je     loc_3_2D87
        mov    dword ptr [esi + 0x2e0], 0xa
        mov    dword ptr [esi + 0x4ec], 2
        jmp    loc_3_2DD6
loc_3_2D87:
        push   0
        push   0
        push   6
        push   dword ptr [esi + 0x29c]
        call   sub_1_0480
        add    esp, 0x10
        mov    dword ptr [esi + 0x2e0], 3
        mov    eax, dword ptr [esi + 0x2a8]
        and    eax, 0xffff
        mov    dword ptr [esi + 0x2a8], eax
        or     eax, 0xffff0000
        mov    dword ptr [esi + 0x2a8], eax
        push   3
        push   esi
        call   sub_3_063B
        add    esp, 8
        jmp    loc_3_2DE1
loc_3_2DCE:
        test   al, 0x20
        jne    loc_3_2DD6
        test   al, 0x10
        je     loc_3_2DE1
loc_3_2DD6:
        push   esi
        call   sub_3_1E80
        add    esp, 4
        sub    edi, edi
loc_3_2DE1:
        mov    dword ptr [esi + 0x5b2], 0
        mov    dword ptr [esi + 0x60c], 0
        or     edi, edi
        je     loc_3_2E03
        mov    dword ptr [esi + 0x620], 0
loc_3_2E03:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_3_2E0A:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_3_2E61
        add    esp, 8
        mov    edi, eax
        cmp    edi, 3
        jne    loc_3_2E5C
        push   dword ptr [esi + 0x4f0]
        lea    eax, [esi + 0x3ec]
        push   eax
        lea    eax, [esi + 0x2e8]
        push   eax
        call   sub_3_061C
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_2E5C
        mov    eax, dword ptr [esi + 0x4f0]
        cmp    dword ptr [esi + 0x3e8], eax
        mov    edi, 1
        je     loc_3_2E5C
        mov    edi, 2
loc_3_2E5C:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
sub_3_2E61:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 0x10], 0
        mov    ebx, dword ptr [eax + 0x5fa]
        add    eax, 0x3ec
        mov    dword ptr [esp + 0x14], 0
        mov    ebp, dword ptr [eax + 0x104]
        mov    dword ptr [esp + 0x18], eax
        or     ebx, ebx
        je     loc_3_2ED8
loc_3_2E95:
        push   ebp
        lea    eax, [ebx + 0xf]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_3_3146
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_2ED2
        movzx  eax, byte ptr [ebx + 0xe]
        sub    eax, ebp
        cmp    eax, -1
        jne    loc_3_2ECA
        mov    dword ptr [esp + 0x10], 1
        lea    esi, [ebx + 4]
        mov    edi, dword ptr [esp + 0x24]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        jmp    loc_3_2ED2
loc_3_2ECA:
        mov    dword ptr [esp + 0x14], 1
loc_3_2ED2:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_3_2E95
loc_3_2ED8:
        mov    ecx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esp + 0x10]
        lea    eax, [ecx + eax*2]
        or     eax, eax
        je     loc_3_2EFD
        cmp    eax, 1
        je     loc_3_2F04
        cmp    eax, 2
        je     loc_3_2F0B
        cmp    eax, 3
        je     loc_3_2F0F
        mov    eax, 3
        jmp    loc_3_2F14
loc_3_2EFD:
        mov    eax, 3
        jmp    loc_3_2F14
loc_3_2F04:
        mov    eax, 2
        jmp    loc_3_2F14
loc_3_2F0B:
        sub    eax, eax
        jmp    loc_3_2F14
loc_3_2F0F:
        mov    eax, 4
loc_3_2F14:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_3_2F1C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x1c]
        mov    ebx, dword ptr [esp + 0x20]
        cmp    byte ptr [ebp], 0
        je     loc_3_30D7
loc_3_2F35:
        cmp    byte ptr [ebp], 0x3c
        jne    loc_3_30C6
        push   4
        push   0x908                         ; -> obj1:0x0908
        push   ebp
        call   sub_3_3146
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_2F5F
        mov    byte ptr [ebx], 0xd
        inc    ebx
        add    ebp, 4
        jmp    loc_3_30CD
loc_3_2F5F:
        push   4
        push   0x910                         ; -> loc_1_0910
        push   ebp
        call   sub_3_3146
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_2F7F
        mov    byte ptr [ebx], 0xa
        inc    ebx
        add    ebp, 4
        jmp    loc_3_30CD
loc_3_2F7F:
        mov    al, byte ptr [ebp + 1]
        cmp    al, 0x68
        je     loc_3_2F8E
        cmp    al, 0x48
        jne    loc_3_3038
loc_3_2F8E:
        mov    cl, byte ptr [ebp + 2]
        cmp    cl, 0x61
        jl     loc_3_2F9B
        cmp    cl, 0x66
        jle    loc_3_2FB7
loc_3_2F9B:
        cmp    cl, 0x41
        jl     loc_3_2FA5
        cmp    cl, 0x46
        jle    loc_3_2FB7
loc_3_2FA5:
        cmp    cl, 0x30
        jl     loc_3_3038
        cmp    cl, 0x39
        jg     loc_3_3038
loc_3_2FB7:
        mov    al, byte ptr [ebp + 3]
        cmp    al, 0x61
        jl     loc_3_2FC2
        cmp    al, 0x66
        jle    loc_3_2FD2
loc_3_2FC2:
        cmp    al, 0x41
        jl     loc_3_2FCA
        cmp    al, 0x46
        jle    loc_3_2FD2
loc_3_2FCA:
        cmp    al, 0x30
        jl     loc_3_3038
        cmp    al, 0x39
        jg     loc_3_3038
loc_3_2FD2:
        cmp    byte ptr [ebp + 4], 0x3e
        jne    loc_3_3038
        cmp    cl, 0x30
        jl     loc_3_2FEA
        cmp    cl, 0x39
        jg     loc_3_2FEA
        movsx  ecx, cl
        sub    ecx, 0x30
        jmp    loc_3_3002
loc_3_2FEA:
        cmp    cl, 0x61
        jl     loc_3_2FFC
        cmp    cl, 0x7a
        jg     loc_3_2FFC
        movsx  ecx, cl
        sub    ecx, 0x20
        jmp    loc_3_2FFF
loc_3_2FFC:
        movsx  ecx, cl
loc_3_2FFF:
        sub    ecx, 0x37
loc_3_3002:
        cmp    al, 0x30
        jl     loc_3_3012
        cmp    al, 0x39
        jg     loc_3_3012
        movsx  eax, al
        sub    eax, 0x30
        jmp    loc_3_3028
loc_3_3012:
        cmp    al, 0x61
        jl     loc_3_3022
        cmp    al, 0x7a
        jg     loc_3_3022
        movsx  eax, al
        sub    eax, 0x20
        jmp    loc_3_3025
loc_3_3022:
        movsx  eax, al
loc_3_3025:
        sub    eax, 0x37
loc_3_3028:
        shl    cl, 4
        add    cl, al
        mov    byte ptr [ebx], cl
        inc    ebx
        add    ebp, 5
        jmp    loc_3_30CD
loc_3_3038:
        cmp    dword ptr [esp + 0x28], 0
        je     loc_3_30C6
        mov    dword ptr [esp + 0x14], 0
        mov    dword ptr [esp + 0x10], 0
        cmp    dword ptr [esp + 0x2c], 0
        je     loc_3_30BF
        mov    esi, dword ptr [esp + 0x28]
loc_3_305E:
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    edi, [ecx - 1]
        push   edi
        push   esi
        push   ebp
        call   sub_3_3146
        add    esp, 0xc
        or     eax, eax
        je     loc_3_3093
        add    esi, 0x200
        inc    dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x10]
        cmp    eax, dword ptr [esp + 0x2c]
        jb     loc_3_305E
        jmp    loc_3_30BF
loc_3_3093:
        mov    ecx, dword ptr [esp + 0x10]
        shl    ecx, 9
        mov    eax, dword ptr [esp + 0x28]
        lea    ecx, [ecx + eax + 0x100]
        cmp    byte ptr [ecx], 0
        je     loc_3_30B5
loc_3_30AA:
        mov    al, byte ptr [ecx]
        mov    byte ptr [ebx], al
        inc    ecx
        inc    ebx
        cmp    byte ptr [ecx], 0
        jne    loc_3_30AA
loc_3_30B5:
        add    ebp, edi
        mov    dword ptr [esp + 0x14], 1
loc_3_30BF:
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_3_30CD
loc_3_30C6:
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebx], al
        inc    ebp
        inc    ebx
loc_3_30CD:
        cmp    byte ptr [ebp], 0
        jne    loc_3_2F35
loc_3_30D7:
        mov    byte ptr [ebx], 0
        mov    eax, dword ptr [esp + 0x24]
        or     eax, eax
        je     loc_3_30E8
        sub    ebx, dword ptr [esp + 0x20]
        mov    dword ptr [eax], ebx
loc_3_30E8:
        mov    eax, 1
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_3_30F5:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        mov    esi, edi
        mov    ebx, dword ptr [esp + 0x10]
        or     ebx, ebx
        jne    loc_3_310F
        mov    byte ptr [edi], 0x30
        mov    byte ptr [edi + 1], 0
        jmp    loc_3_3142
loc_3_310F:
        mov    ecx, 0xa
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        add    dl, 0x30
        mov    byte ptr [esi], dl
        inc    esi
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_3_310F
        mov    byte ptr [esi], 0
        dec    esi
        cmp    esi, edi
        jbe    loc_3_3142
loc_3_3134:
        mov    al, byte ptr [esi]
        mov    cl, byte ptr [edi]
        mov    byte ptr [esi], cl
        mov    byte ptr [edi], al
        inc    edi
        dec    esi
        cmp    esi, edi
        ja     loc_3_3134
loc_3_3142:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_3146:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x18]
        or     edx, edx
        je     loc_3_3194
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
loc_3_3159:
        mov    bl, byte ptr [eax]
        cmp    bl, 0x61
        jl     loc_3_316D
        cmp    bl, 0x7a
        jg     loc_3_316D
        movsx  edi, bl
        sub    edi, 0x20
        jmp    loc_3_3170
loc_3_316D:
        movsx  edi, bl
loc_3_3170:
        mov    bl, byte ptr [ecx]
        cmp    bl, 0x61
        jl     loc_3_3184
        cmp    bl, 0x7a
        jg     loc_3_3184
        movsx  esi, bl
        sub    esi, 0x20
        jmp    loc_3_3187
loc_3_3184:
        movsx  esi, bl
loc_3_3187:
        cmp    edi, esi
        jne    loc_3_319A
        or     bl, bl
        je     loc_3_3194
        inc    eax
        inc    ecx
        dec    edx
        jne    loc_3_3159
loc_3_3194:
        sub    eax, eax
loc_3_3196:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_319A:
        mov    eax, 1
        jmp    loc_3_3196
sub_3_31A1:
        push   ebx
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 0x10]
loc_3_31AB:
        mov    cl, byte ptr [eax]
        cmp    cl, 0x61
        jl     loc_3_31BF
        cmp    cl, 0x7a
        jg     loc_3_31BF
        movsx  edx, cl
        sub    edx, 0x20
        jmp    loc_3_31C2
loc_3_31BF:
        movsx  edx, cl
loc_3_31C2:
        mov    bl, byte ptr [esi]
        cmp    bl, 0x61
        jl     loc_3_31D6
        cmp    bl, 0x7a
        jg     loc_3_31D6
        movsx  ecx, bl
        sub    ecx, 0x20
        jmp    loc_3_31D9
loc_3_31D6:
        movsx  ecx, bl
loc_3_31D9:
        cmp    edx, ecx
        jne    loc_3_31E9
        or     bl, bl
        je     loc_3_31E5
        inc    eax
        inc    esi
        jmp    loc_3_31AB
loc_3_31E5:
        sub    eax, eax
        jmp    loc_3_31F1
loc_3_31E9:
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [esi]
        sub    eax, ecx
loc_3_31F1:
        pop    esi
        pop    ebx
        ret    
sub_3_31F4:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 0x14]
        push   eax
        sub    esi, esi
        mov    eax, dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x34]
        push   dword ptr [eax + 0x2e4]
        call   sub_1_0428
        add    esp, 0xc
        cmp    eax, esi
        je     loc_3_3223
        sub    eax, eax
        jmp    loc_3_3341
loc_3_3223:
        mov    edi, 1
        lea    eax, [esp + 0x1c]
        push   eax
        push   edi
        call   sub_3_30F5
        add    esp, 8
        mov    ebx, edi
loc_3_3238:
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x1c]
        push   0
        push   eax
        lea    eax, [esp + 0x28]
        push   0
        push   eax
        push   dword ptr [esp + 0x28]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_327A
        cmp    dword ptr [esp + 0x18], ebx
        jne    loc_3_3331
        add    esi, dword ptr [esp + 0x10]
        inc    edi
        lea    eax, [esp + 0x1c]
        push   eax
        push   edi
        call   sub_3_30F5
        add    esp, 8
        jmp    loc_3_3238
loc_3_327A:
        cmp    eax, 2
        je     loc_3_3284
        jmp    loc_3_3331
loc_3_3284:
        mov    edi, dword ptr [esp + 0x34]
        or     edi, edi
        je     loc_3_32B0
        push   0
        push   edi
        call   sub_1_04E0
        add    esp, 8
        or     eax, eax
        jne    loc_3_32A0
        jmp    loc_3_3331
loc_3_32A0:
        add    esi, eax
        push   1
        push   esi
        push   edi
        call   sub_1_04D8
        add    esp, 0xc
        jmp    loc_3_32BC
loc_3_32B0:
        inc    esi
        push   1
        push   esi
        call   sub_1_04C8
        add    esp, 8
loc_3_32BC:
        mov    ebp, eax
        or     ebp, ebp
        je     loc_3_3333
        mov    ebx, ebp
        cmp    byte ptr [ebp], 0
        je     loc_3_32DE
loc_3_32CA:
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ebx, ecx
        cmp    byte ptr [ebx], 0
        jne    loc_3_32CA
loc_3_32DE:
        mov    edi, 1
loc_3_32E3:
        lea    eax, [esp + 0x1c]
        push   eax
        push   edi
        call   sub_3_30F5
        add    esp, 8
        mov    eax, ebp
        sub    eax, ebx
        add    eax, esi
        mov    dword ptr [esp + 0x10], eax
        lea    eax, [esp + 0x10]
        push   eax
        push   ebx
        push   0
        lea    eax, [esp + 0x28]
        push   0
        push   eax
        push   dword ptr [esp + 0x28]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_3321
        add    ebx, dword ptr [esp + 0x10]
        inc    edi
        jmp    loc_3_32E3
loc_3_3321:
        cmp    eax, 2
        je     loc_3_3333
        push   0
        push   ebp
        call   sub_1_04D0
        add    esp, 8
loc_3_3331:
        sub    ebp, ebp
loc_3_3333:
        push   dword ptr [esp + 0x14]
        call   sub_1_0430
        add    esp, 4
        mov    eax, ebp
loc_3_3341:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
sub_3_3349:
        push   esi
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_3_3375
        lea    eax, [esp + 0xc]
        push   eax
        mov    esi, dword ptr [esp + 0xc]
        push   0
        push   dword ptr [esi + 0x29c]
        call   sub_1_0470
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_3379
        mov    eax, 0xffffffff
        jmp    loc_3_33D5
loc_3_3375:
        mov    esi, dword ptr [esp + 8]
loc_3_3379:
        cmp    dword ptr [esp + 0xc], 0
        je     loc_3_33D1
        test   dword ptr [esp + 0xc], 0x9bf0
        je     loc_3_3390
        inc    dword ptr [esi + 0x628]
loc_3_3390:
        test   byte ptr [esp + 0xc], 8
        je     loc_3_339D
        inc    dword ptr [esi + 0x62c]
loc_3_339D:
        test   byte ptr [esp + 0xd], 4
        je     loc_3_33AA
        inc    dword ptr [esi + 0x630]
loc_3_33AA:
        test   byte ptr [esp + 0xc], 2
        je     loc_3_33B7
        inc    dword ptr [esi + 0x634]
loc_3_33B7:
        test   byte ptr [esp + 0xc], 1
        je     loc_3_33C4
        inc    dword ptr [esi + 0x638]
loc_3_33C4:
        test   byte ptr [esp + 0xc], 4
        je     loc_3_33D1
        inc    dword ptr [esi + 0x63c]
loc_3_33D1:
        mov    eax, dword ptr [esp + 0xc]
loc_3_33D5:
        pop    esi
        ret    
sub_3_33D7:
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x4c]
        mov    edi, ebp
        mov    ecx, dword ptr [esp + 0x48]
        lea    esi, [esp + 0x10]
        mov    eax, edi
        dec    edi
        or     eax, eax
        mov    eax, dword ptr [esp + 0x50]
        je     loc_3_3481
loc_3_33FB:
        mov    edx, esi
        lea    ebx, [esp + 0x10]
        sub    edx, ebx
        cmp    edx, 0x2e
        jge    loc_3_3481
        mov    dl, byte ptr [ecx]
        cmp    dl, 0x20
        jl     loc_3_342C
        cmp    dl, 0x7e
        jg     loc_3_342C
        cmp    eax, 1
        jne    loc_3_3428
        cmp    dl, 0x30
        jl     loc_3_3428
        cmp    dl, 0x39
        jg     loc_3_3428
        mov    byte ptr [esi], 0x23
        jmp    loc_3_3474
loc_3_3428:
        mov    byte ptr [esi], dl
        jmp    loc_3_3474
loc_3_342C:
        movsx  ebx, dl
        cmp    ebx, 0xa
        je     loc_3_345B
        cmp    ebx, 0xd
        mov    byte ptr [esi], 0x3c
        je     loc_3_3468
        mov    bl, dl
        shr    bl, 4
        and    ebx, 0xf
        inc    esi
        mov    bl, byte ptr [ebx + 0x888]    ; -> loc_1_0888
        mov    byte ptr [esi], bl
        and    edx, 0xf
        inc    esi
        mov    dl, byte ptr [edx + 0x888]    ; -> loc_1_0888
        mov    byte ptr [esi], dl
        jmp    loc_3_3470
loc_3_345B:
        mov    byte ptr [esi], 0x3c
        inc    esi
        mov    byte ptr [esi], 0x6c
        inc    esi
        mov    byte ptr [esi], 0x66
        jmp    loc_3_3470
loc_3_3468:
        inc    esi
        mov    byte ptr [esi], 0x63
        inc    esi
        mov    byte ptr [esi], 0x72
loc_3_3470:
        inc    esi
        mov    byte ptr [esi], 0x3e
loc_3_3474:
        inc    esi
        inc    ecx
        mov    edx, edi
        dec    edi
        or     edx, edx
        jne    loc_3_33FB
loc_3_3481:
        mov    byte ptr [esi], 0
        cmp    eax, 1
        jbe    loc_3_3490
        cmp    eax, 2
        je     loc_3_34A8
        jmp    loc_3_34DF
loc_3_3490:
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_34DF
        lea    eax, [esp + 0x10]
        push   eax
        movzx  eax, word ptr [0x30]          ; -> obj2:0x0030
        jmp    loc_3_34CF
loc_3_34A8:
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_34DF
        or     ebp, ebp
        lea    eax, [esp + 0x10]
        jne    loc_3_34C7
        movzx  eax, word ptr [0x34]          ; -> obj2:0x0034
        add    eax, dword ptr [4]            ; -> loc_2_0004
loc_3_34C7:
        push   eax
        movzx  eax, word ptr [0x32]          ; -> obj2:0x0032
loc_3_34CF:
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 0xc
loc_3_34DF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
sub_3_34E7:
        mov    edx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     edx, edx
        je     loc_3_3522
        mov    ecx, dword ptr [esp + 4]
        movzx  eax, byte ptr [ecx + 0x18]
        push   dword ptr [eax*4 + 0x924]     ; -> obj1:0x0924
        movzx  eax, byte ptr [ecx + 0x16]
        push   eax
        movzx  eax, byte ptr [ecx + 0x17]
        movsx  eax, byte ptr [eax + 0x89c]   ; -> loc_1_089C
        push   eax
        push   dword ptr [ecx + 4]
        push   0x930                         ; -> loc_1_0930
        push   edx
        call   sub_1_0588
        add    esp, 0x18
loc_3_3522:
        ret    
sub_3_3523:
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x40]
        mov    dword ptr [esp + 0x28], 0
        mov    eax, dword ptr [ebx + 0x2e0]
        cmp    eax, 0x18
        je     loc_3_3546
        cmp    eax, 0x13
        jne    loc_3_3560
loc_3_3546:
        test   byte ptr [ebx + 0x825], 2
        mov    ebp, dword ptr [esp + 0x44]
        je     loc_3_3564
        cmp    byte ptr [ebp], 0x3b
        je     loc_3_3564
        mov    esi, 1
        jmp    loc_3_3566
loc_3_3560:
        mov    ebp, dword ptr [esp + 0x44]
loc_3_3564:
        sub    esi, esi
loc_3_3566:
        cmp    eax, 0x18
        je     loc_3_3570
        cmp    eax, 0x13
        jne    loc_3_3581
loc_3_3570:
        test   byte ptr [ebx + 0x824], 0x40
        mov    dword ptr [esp + 0x34], 1
        jne    loc_3_3589
loc_3_3581:
        mov    dword ptr [esp + 0x34], 0
loc_3_3589:
        mov    ecx, dword ptr [esp + 0x48]
        mov    eax, ebp
        mov    dword ptr [ecx], 1
        cmp    byte ptr [ebp], 0
        je     loc_3_35B0
loc_3_359B:
        cmp    byte ptr [eax], 0x3b
        jne    loc_3_35AA
        mov    ecx, dword ptr [esp + 0x48]
        mov    dword ptr [ecx], 0
loc_3_35AA:
        inc    eax
        cmp    byte ptr [eax], 0
        jne    loc_3_359B
loc_3_35B0:
        mov    ecx, dword ptr [esp + 0x48]
        cmp    dword ptr [ecx], 0
        jne    loc_3_35BD
        mov    byte ptr [eax - 1], 0
loc_3_35BD:
        push   1
        push   0xa4
        call   sub_1_04C8
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 8
        or     eax, eax
        jne    loc_3_35DB
        sub    eax, eax
        jmp    loc_3_3B94
loc_3_35DB:
        mov    eax, dword ptr [esp + 0x14]
        add    eax, 0x29
        mov    dword ptr [esp + 0x18], eax
        add    eax, 0x29
        mov    dword ptr [esp + 0x1c], eax
        add    eax, 0x29
        mov    dword ptr [esp + 0x30], eax
        lea    eax, [esp + 0x2c]
        push   eax
        push   0xaac                         ; -> obj1:0x0AAC
        push   dword ptr [ebx + 0x2e4]
        call   sub_1_0428
        add    esp, 0xc
        or     eax, eax
        je     loc_3_3625
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_1_04D0
        add    esp, 8
        sub    eax, eax
        jmp    loc_3_3B94
loc_3_3625:
        mov    dword ptr [esp + 0x20], 0x28
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x28]
        push   dword ptr [esp + 0x18]
        push   eax
        push   0
        push   0xac8                         ; -> obj1:0x0AC8
        push   dword ptr [esp + 0x40]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_3673
        cmp    dword ptr [esp + 0x24], 1
        jne    loc_3_3B59
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        call   sub_3_2F1C
        add    esp, 0x14
loc_3_3673:
        or     esi, esi
        je     loc_3_3696
        mov    dword ptr [esp + 0x20], 0x28
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x28]
        push   dword ptr [esp + 0x18]
        push   eax
        push   0
        push   0x8a4                         ; -> loc_1_08A4
        jmp    loc_3_36D4
loc_3_3696:
        test   byte ptr [ebx + 0x539], 2
        je     loc_3_371D
        cmp    dword ptr [ebx + 0x5c2], 0
        jne    loc_3_371D
        mov    dword ptr [esp + 0x20], 0x28
        lea    eax, [esp + 0x20]
        push   eax
        test   byte ptr [ebx + 0x5a2], 2
        lea    eax, [esp + 0x28]
        push   dword ptr [esp + 0x18]
        push   eax
        mov    eax, 0x8a4                    ; -> loc_1_08A4
        push   0
        jne    loc_3_36D3
        mov    eax, 0x8b0                    ; -> loc_1_08B0
loc_3_36D3:
        push   eax
loc_3_36D4:
        push   dword ptr [esp + 0x40]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_3B59
        cmp    dword ptr [esp + 0x24], 1
        jne    loc_3_3B59
        push   0
        push   0
        mov    edi, dword ptr [esp + 0x20]
        push   0
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [esp + 0x24]
        lea    eax, [ecx + eax - 1]
        push   eax
        push   dword ptr [esp + 0x24]
        call   sub_3_2F1C
        add    esp, 0x14
loc_3_371D:
        mov    dword ptr [esp + 0x20], 0x28
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x28]
        push   dword ptr [esp + 0x18]
        push   eax
        push   0
        push   0x8bc                         ; -> obj1:0x08BC
        push   dword ptr [esp + 0x40]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_3B59
        cmp    dword ptr [esp + 0x24], 1
        jne    loc_3_3B59
        push   0
        push   0
        mov    edi, dword ptr [esp + 0x20]
        push   0
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [esp + 0x24]
        lea    eax, [ecx + eax - 1]
        push   eax
        push   dword ptr [esp + 0x24]
        call   sub_3_2F1C
        add    esp, 0x14
        test   byte ptr [ebx + 0x539], 1
        je     loc_3_3801
        mov    dword ptr [esp + 0x20], 0x28
        lea    eax, [esp + 0x20]
        push   eax
        test   byte ptr [ebx + 0x5a2], 1
        lea    eax, [esp + 0x28]
        push   dword ptr [esp + 0x18]
        push   eax
        mov    eax, 0x8d4                    ; -> obj1:0x08D4
        push   0
        jne    loc_3_37B7
        mov    eax, 0x8dc                    ; -> loc_1_08DC
loc_3_37B7:
        push   eax
        push   dword ptr [esp + 0x40]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_3B59
        cmp    dword ptr [esp + 0x24], 1
        jne    loc_3_3B59
        push   0
        push   0
        mov    edi, dword ptr [esp + 0x20]
        push   0
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [esp + 0x24]
        lea    eax, [ecx + eax - 1]
        push   eax
        push   dword ptr [esp + 0x24]
        call   sub_3_2F1C
        add    esp, 0x14
loc_3_3801:
        mov    dword ptr [esp + 0x20], 0x28
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x28]
        push   dword ptr [esp + 0x18]
        push   eax
        push   0
        push   0x8c8                         ; -> obj1:0x08C8
        push   dword ptr [esp + 0x40]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_3864
        cmp    dword ptr [esp + 0x20], 1
        jbe    loc_3_3864
        add    ebx, 0x5c6
        mov    dword ptr [ebx], 1
        cmp    dword ptr [esp + 0x24], 1
        jne    loc_3_3B59
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        call   sub_3_2F1C
        add    esp, 0x14
        jmp    loc_3_387A
loc_3_3864:
        mov    esi, 0x940                    ; -> loc_1_0940
        mov    edi, dword ptr [esp + 0x1c]
        add    ebx, 0x5c6
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [ebx], 0
loc_3_387A:
        mov    dword ptr [esp + 0x20], 0x28
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x28]
        push   dword ptr [esp + 0x18]
        push   eax
        push   0
        push   0xad0                         ; -> obj1:0x0AD0
        push   dword ptr [esp + 0x40]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_38F9
        cmp    dword ptr [esp + 0x24], 1
        jne    loc_3_3B59
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x3c]
        push   dword ptr [esp + 0x24]
        call   sub_3_2F1C
        mov    edi, dword ptr [esp + 0x44]
        add    esp, 0x14
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_38F9:
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, dword ptr [esp + 0x18]
        lea    edx, [ecx - 1]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    esi, [edx + ecx - 1]
        mov    eax, 0x28
        sub    eax, esi
        mov    dword ptr [esp + 0x38], eax
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    eax, 1
        je     loc_3_3957
        mov    eax, ecx
        sub    edx, edx
        div    dword ptr [esp + 0x38]
        cmp    edx, 1
        sbb    edi, edi
        mov    eax, ecx
        sub    edx, edx
        div    dword ptr [esp + 0x38]
        lea    eax, [edi + eax + 1]
loc_3_3957:
        inc    esi
        imul   esi, eax
        add    esi, ecx
        inc    esi
        mov    dword ptr [esp + 0x20], esi
        push   1
        push   dword ptr [esp + 0x24]
        call   sub_1_04C8
        mov    dword ptr [esp + 0x30], eax
        add    esp, 8
        or     eax, eax
        je     loc_3_3B59
        mov    edx, eax
        cmp    dword ptr [ebx], 0
        jne    loc_3_3A11
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 0x28]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, dword ptr [esp + 0x28]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, dword ptr [esp + 0x28]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_3_3B76
loc_3_3A11:
        mov    byte ptr [esp + 0x13], 0
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 0x28]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    byte ptr [ebp], 0
        je     loc_3_3B12
loc_3_3A45:
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, dword ptr [esp + 0x1c]
        lea    esi, [ecx - 1]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    esi, [esi + ecx - 1]
        inc    esi
        cmp    esi, 0x28
        jbe    loc_3_3AD1
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    edx, ecx
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_3_3B08
loc_3_3AD1:
        mov    al, byte ptr [ebp]
        mov    byte ptr [esp + 0x12], al
        lea    edi, [esp + 0x12]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        inc    ebp
loc_3_3B08:
        cmp    byte ptr [ebp], 0
        jne    loc_3_3A45
loc_3_3B12:
        mov    ecx, dword ptr [esp + 0x48]
        cmp    dword ptr [ecx], 0
        je     loc_3_3B28
        cmp    dword ptr [esp + 0x34], 0
        jne    loc_3_3B28
        mov    edi, dword ptr [esp + 0x30]
        jmp    loc_3_3B2C
loc_3_3B28:
        mov    edi, dword ptr [esp + 0x1c]
loc_3_3B2C:
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_3_3B76
loc_3_3B59:
        cmp    dword ptr [esp + 0x28], 0
        je     loc_3_3B76
        push   0
        push   dword ptr [esp + 0x2c]
        call   sub_1_04D0
        mov    dword ptr [esp + 0x30], 0
        add    esp, 8
loc_3_3B76:
        push   dword ptr [esp + 0x2c]
        call   sub_1_0430
        add    esp, 4
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_1_04D0
        mov    eax, dword ptr [esp + 0x30]
        add    esp, 8
loc_3_3B94:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x2c
        ret    
sub_3_3B9C:
        sub    esp, 0xc4
        push   esi
        push   edi
        mov    esi, 1
        mov    edi, dword ptr [esp + 0xd0]
        or     edi, edi
        jne    loc_3_3BBB
        sub    eax, eax
        jmp    loc_3_3DC4
loc_3_3BBB:
        mov    eax, dword ptr [esp + 0xd4]
        add    eax, 0x1000000f
        cmp    eax, 5
        ja     loc_3_3BD3
        jmp    loc_3_3DCD
loc_3_3BD3:
        sub    esi, esi
        jmp    loc_3_3DC2
loc_3_3BDA:
        lea    eax, [esp + 8]
        push   eax
        push   0x808                         ; -> loc_1_0808
        push   dword ptr [edi + 0x2e4]
        call   sub_1_0428
        add    esp, 0xc
        or     eax, eax
        je     loc_3_3C00
        mov    esi, 2
        jmp    loc_3_3DC2
loc_3_3C00:
        cmp    word ptr [edi + 0x899], 0
        je     loc_3_3C0F
        jmp    loc_3_3DBD
loc_3_3C0F:
        mov    eax, dword ptr [esp + 0xd8]
        or     eax, eax
        jne    loc_3_3C24
        mov    esi, 2
        jmp    loc_3_3DC2
loc_3_3C24:
        mov    word ptr [edi + 0x899], 1
        mov    dword ptr [edi + 0x8a1], eax
        jmp    loc_3_3DC2
loc_3_3C38:
        cmp    word ptr [edi + 0x899], 1
        je     loc_3_3C47
        jmp    loc_3_3DBD
loc_3_3C47:
        mov    word ptr [edi + 0x899], 0
        mov    word ptr [edi + 0x89b], 0
        mov    dword ptr [edi + 0x89d], 0
        mov    dword ptr [edi + 0x8a1], 0
        jmp    loc_3_3DC2
loc_3_3C72:
        cmp    word ptr [edi + 0x899], 1
        je     loc_3_3C81
        jmp    loc_3_3DBD
loc_3_3C81:
        mov    word ptr [edi + 0x899], 2
        mov    eax, dword ptr [esp + 0xd8]
        mov    dword ptr [edi + 0x89d], eax
        push   0x80
        push   edi
        call   sub_3_3E17
        add    esp, 8
        jmp    loc_3_3DC2
loc_3_3CAA:
        cmp    word ptr [edi + 0x899], 2
        je     loc_3_3CB9
        jmp    loc_3_3DBD
loc_3_3CB9:
        mov    word ptr [edi + 0x899], 1
        mov    eax, dword ptr [edi + 0x5be]
        mov    dword ptr [edi + 0x8a5], eax
        push   0
        push   edi
        call   sub_3_3E17
        add    esp, 8
        jmp    loc_3_3DC2
loc_3_3CDE:
        cmp    word ptr [edi + 0x899], 2
        je     loc_3_3CED
        jmp    loc_3_3DBD
loc_3_3CED:
        cmp    word ptr [edi + 0x89b], 0x80
        je     loc_3_3D02
        mov    esi, 0xf
        jmp    loc_3_3DC2
loc_3_3D02:
        mov    dword ptr [esp + 0xc], edi
        mov    dword ptr [esp + 0x18], 1
        mov    dword ptr [esp + 0x10], 0
        mov    dword ptr [esp + 0x14], 0
        lea    eax, [esp + 0xc]
        mov    byte ptr [esp + 0x1c], 0
        push   eax
        call   sub_3_1034
        add    esp, 4
        jmp    loc_3_3DC2
loc_3_3D35:
        cmp    word ptr [edi + 0x899], 2
        jne    loc_3_3DBD
        cmp    word ptr [edi + 0x89b], 0x80
        jne    loc_3_3DBD
        mov    dword ptr [esp + 0xc], edi
        mov    dword ptr [esp + 0x18], 2
        mov    dword ptr [esp + 0x10], 0
        mov    dword ptr [esp + 0x14], 0
        lea    eax, [esp + 0xc]
        mov    byte ptr [esp + 0x1c], 0
        push   eax
        call   sub_3_1034
        add    esp, 4
        mov    dword ptr [edi + 0x2e0], 4
        mov    dword ptr [esp + 0x6c], edi
        mov    eax, dword ptr [edi + 0x7d3]
        mov    dword ptr [esp + 0x70], eax
        mov    eax, dword ptr [edi + 0x7d7]
        mov    dword ptr [esp + 0x74], eax
        mov    eax, dword ptr [edi + 0x7db]
        mov    dword ptr [esp + 0x78], eax
        mov    eax, dword ptr [edi + 0x5be]
        mov    dword ptr [esp + 0x7c], eax
        lea    eax, [esp + 0x6c]
        push   eax
        call   sub_3_0823
        add    esp, 4
        jmp    loc_3_3DC2
loc_3_3DBD:
        mov    esi, 0xa
loc_3_3DC2:
        mov    eax, esi
loc_3_3DC4:
        pop    edi
        pop    esi
        add    esp, 0xc4
        ret    
loc_3_3DCD:
        fidivr dword ptr [ebx]
        add    byte ptr [eax], al
        cmp    byte ptr [eax + eax], bh
        add    byte ptr [edx + 0x3c], dh     ; -> loc_3_3C72
        add    byte ptr [eax], al
        stosb  byte ptr es:[edi], al
        cmp    al, 0
        add    dh, bl                        ; -> loc_3_3CDE
        cmp    al, 0
        db  000h
        dd  loc_3_3D35               ; fixup
sub_3_3DE5:
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        je     loc_3_3E14
        mov    ax, word ptr [esp + 8]
        cmp    ax, 1
        jb     loc_3_3E14
        cmp    ax, 0xb
        ja     loc_3_3E14
        movzx  eax, ax
        push   eax
        push   dword ptr [ecx + 0x89d]
        push   ecx
        call   dword ptr [ecx + 0x8a1]
        add    esp, 0xc
        jmp    loc_3_3E16
loc_3_3E14:
        sub    eax, eax
loc_3_3E16:
        ret    
sub_3_3E17:
        sub    esp, 0x60
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x70]
        or     esi, esi
        je     loc_3_3F42
        mov    ax, word ptr [esi + 0x899]
        cmp    ax, 1
        je     loc_3_3E40
        cmp    ax, 2
        jne    loc_3_3F42
loc_3_3E40:
        mov    cx, word ptr [esp + 0x74]
        or     cx, cx
        je     loc_3_3E55
        cmp    cx, 0x80
        jne    loc_3_3F42
loc_3_3E55:
        cmp    word ptr [esi + 0x89b], cx
        jne    loc_3_3E68
        mov    eax, 1
        jmp    loc_3_3F44
loc_3_3E68:
        mov    word ptr [esi + 0x89b], cx
        cmp    dword ptr [esi + 0x2e0], 4
        je     loc_3_3E82
        mov    eax, 1
        jmp    loc_3_3F44
loc_3_3E82:
        mov    dword ptr [esp + 0xc], esi
        mov    eax, dword ptr [esi + 0x7db]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esi + 0x7d3]
        cmp    cx, 0x80
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esi + 0x7d7]
        mov    dword ptr [esp + 0x14], eax
        lea    ecx, [esi + 0x5be]
        jne    loc_3_3EC1
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x8a5], eax
        mov    dword ptr [ecx], 1
        jmp    loc_3_3EC9
loc_3_3EC1:
        mov    eax, dword ptr [esi + 0x8a5]
        mov    dword ptr [ecx], eax
loc_3_3EC9:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esp + 0x1c], eax
        lea    eax, [esp + 0xc]
        push   eax
        call   sub_3_0842
        add    esp, 4
        mov    ebx, eax
        cmp    ebx, 1
        jne    loc_3_3F3E
        push   0
        call   sub_1_04F4
        mov    dword ptr [esi + 0x8a9], eax
        push   esi
        push   0x7d0
        push   0x3f4b                        ; -> loc_3_3F4B
        call   sub_1_0528
        mov    dword ptr [esi + 0x8ad], eax
        mov    edi, dword ptr [esi + 0x8a9]
        push   5
        push   edi
        call   sub_1_051C
        mov    eax, dword ptr [esi + 0x8ad]
        or     eax, eax
        je     loc_3_3F2E
        push   eax
        call   sub_1_0544
        mov    dword ptr [esi + 0x8ad], 0
loc_3_3F2E:
        mov    dword ptr [esi + 0x8a9], 0
        push   edi
        call   sub_1_0504
loc_3_3F3E:
        mov    eax, ebx
        jmp    loc_3_3F44
loc_3_3F42:
        sub    eax, eax
loc_3_3F44:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x60
        ret    
loc_3_3F4B:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x8ad], 0
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x8a9]
        or     eax, eax
        je     loc_3_3F74
        push   eax
        call   sub_1_0510
loc_3_3F74:
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_3F80:
        push   esi
        mov    edx, dword ptr [0x944]        ; -> loc_1_0944
        mov    esi, dword ptr [esp + 8]
        cmp    edx, esi
        jne    loc_3_3F9C
        mov    eax, dword ptr [esi + 0x94]
        mov    dword ptr [0x944], eax        ; -> loc_1_0944
        jmp    loc_3_3FC9
loc_3_3F9C:
        cmp    dword ptr [edx + 0x94], 0
        je     loc_3_3FC9
        sub    ecx, ecx
loc_3_3FA7:
        mov    eax, dword ptr [edx + 0x94]
        cmp    eax, esi
        je     loc_3_3FBD
        mov    edx, eax
        cmp    dword ptr [edx + 0x94], ecx
        jne    loc_3_3FA7
        jmp    loc_3_3FC9
loc_3_3FBD:
        mov    eax, dword ptr [esi + 0x94]
        mov    dword ptr [edx + 0x94], eax
loc_3_3FC9:
        push   dword ptr [esi + 0x2e4]
        call   sub_5_0010
        add    esp, 4
        mov    eax, dword ptr [esi + 0x545]
        or     eax, eax
        je     loc_3_3FEC
        push   0
        push   eax
        call   sub_5_0030
        add    esp, 8
loc_3_3FEC:
        mov    eax, dword ptr [esi + 0x549]
        or     eax, eax
        je     loc_3_4001
        push   0
        push   eax
        call   sub_5_0030
        add    esp, 8
loc_3_4001:
        mov    eax, dword ptr [esi + 0x5ee]
        or     eax, eax
        je     loc_3_4016
        push   0
        push   eax
        call   sub_5_0030
        add    esp, 8
loc_3_4016:
        mov    eax, dword ptr [esi + 0x5fa]
        or     eax, eax
        je     loc_3_402B
        push   0
        push   eax
        call   sub_5_0030
        add    esp, 8
loc_3_402B:
        mov    eax, dword ptr [esi + 0x5de]
        or     eax, eax
        je     loc_3_4040
        push   0
        push   eax
        call   sub_5_0030
        add    esp, 8
loc_3_4040:
        mov    eax, dword ptr [esi + 0x792]
        or     eax, eax
        je     loc_3_4055
        push   0
        push   eax
        call   sub_5_0030
        add    esp, 8
loc_3_4055:
        mov    eax, dword ptr [esi + 0x79a]
        or     eax, eax
        je     loc_3_406A
        push   0
        push   eax
        call   sub_5_0030
        add    esp, 8
loc_3_406A:
        mov    eax, dword ptr [esi + 0x79e]
        or     eax, eax
        je     loc_3_407F
        push   0
        push   eax
        call   sub_5_0030
        add    esp, 8
loc_3_407F:
        mov    eax, dword ptr [esi + 0x849]
        or     eax, eax
        je     loc_3_409B
        cmp    eax, 0xd18                    ; -> obj1:0x0D18
        je     loc_3_409B
        push   0
        push   eax
        call   sub_5_0030
        add    esp, 8
loc_3_409B:
        mov    eax, dword ptr [esi + 0x5e2]
        or     eax, eax
        je     loc_3_40B8
        cmp    dword ptr [esi + 0x5de], eax
        je     loc_3_40B8
        push   0
        push   eax
        call   sub_5_0030
        add    esp, 8
loc_3_40B8:
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_4106
        movzx  eax, word ptr [0x8c]          ; -> obj2:0x008C
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_5_0058
        add    esp, 8
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_412D
        push   dword ptr [esi + 0x2b0]
        movzx  eax, word ptr [0x8e]          ; -> obj2:0x008E
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_5_0058
        add    esp, 0xc
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
loc_3_4106:
        or     ecx, ecx
        je     loc_3_412D
        push   dword ptr [esi + 0x2b4]
        movzx  eax, word ptr [0x90]          ; -> obj2:0x0090
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_5_0058
        add    esp, 0xc
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
loc_3_412D:
        mov    eax, dword ptr [esi + 0x628]
        or     eax, eax
        je     loc_3_4159
        or     ecx, ecx
        je     loc_3_4159
        push   eax
        movzx  eax, word ptr [0x92]          ; -> obj2:0x0092
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_5_0058
        add    esp, 0xc
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
loc_3_4159:
        mov    eax, dword ptr [esi + 0x62c]
        or     eax, eax
        je     loc_3_4185
        or     ecx, ecx
        je     loc_3_4185
        push   eax
        movzx  eax, word ptr [0x94]          ; -> obj2:0x0094
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_5_0058
        add    esp, 0xc
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
loc_3_4185:
        mov    eax, dword ptr [esi + 0x630]
        or     eax, eax
        je     loc_3_41B1
        or     ecx, ecx
        je     loc_3_41B1
        push   eax
        movzx  eax, word ptr [0x96]          ; -> obj2:0x0096
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_5_0058
        add    esp, 0xc
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
loc_3_41B1:
        mov    eax, dword ptr [esi + 0x634]
        or     eax, eax
        je     loc_3_41DD
        or     ecx, ecx
        je     loc_3_41DD
        push   eax
        movzx  eax, word ptr [0x98]          ; -> obj2:0x0098
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_5_0058
        add    esp, 0xc
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
loc_3_41DD:
        mov    eax, dword ptr [esi + 0x638]
        or     eax, eax
        je     loc_3_4209
        or     ecx, ecx
        je     loc_3_4209
        push   eax
        movzx  eax, word ptr [0x9a]          ; -> obj2:0x009A
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_5_0058
        add    esp, 0xc
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
loc_3_4209:
        mov    eax, dword ptr [esi + 0x63c]
        or     eax, eax
        je     loc_3_422F
        or     ecx, ecx
        je     loc_3_422F
        push   eax
        movzx  eax, word ptr [0x9c]          ; -> obj2:0x009C
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_5_0058
        add    esp, 0xc
loc_3_422F:
        cmp    dword ptr [esi + 0x4f4], 0
        je     loc_3_4241
        push   esi
        call   sub_3_00A2
        add    esp, 4
loc_3_4241:
        cmp    dword ptr [esi + 0x620], 0
        je     loc_3_4253
        push   esi
        call   sub_3_00BA
        add    esp, 4
loc_3_4253:
        lea    eax, [esi + 0x640]
        push   eax
        call   sub_3_65CD
        mov    eax, dword ptr [esi + 0x804]
        or     eax, eax
        je     loc_3_426F
        push   eax
        call   sub_5_0038
loc_3_426F:
        mov    eax, dword ptr [esi + 0x808]
        or     eax, eax
        je     loc_3_4282
        push   eax
        call   sub_5_0000
        add    esp, 4
loc_3_4282:
        mov    eax, dword ptr [esi + 0x810]
        or     eax, eax
        je     loc_3_4292
        push   eax
        call   sub_5_0038
loc_3_4292:
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_42BA
        lea    eax, [esi + 0x9c]
        push   eax
        movzx  eax, word ptr [0xd4]          ; -> obj2:0x00D4
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_5_0058
        add    esp, 0xc
loc_3_42BA:
        call   sub_3_648B
        push   0
        push   esi
        call   sub_5_0030
        add    esp, 8
        pop    esi
        ret    
sub_3_42CC:
        push   esi
        push   edi
        mov    edi, dword ptr [0x944]        ; -> loc_1_0944
        or     edi, edi
        je     loc_3_42FA
        mov    esi, dword ptr [esp + 0xc]
loc_3_42DC:
        push   esi
        lea    eax, [edi + 0x9c]
        push   eax
        call   sub_3_31A1
        add    esp, 8
        or     eax, eax
        je     loc_3_42FF
        mov    edi, dword ptr [edi + 0x94]
        or     edi, edi
        jne    loc_3_42DC
loc_3_42FA:
        sub    eax, eax
loc_3_42FC:
        pop    edi
        pop    esi
        ret    
loc_3_42FF:
        mov    eax, edi
        jmp    loc_3_42FC
sub_3_4303:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x64], 0x46544d53
        jne    loc_3_4312
        sub    eax, eax
loc_3_4312:
        ret    
sub_3_4313:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x20], 0
        sub    edi, edi
        lea    eax, [esp + 0x1c]
        push   eax
        mov    ecx, dword ptr [esp + 0x2c]
        push   0x948                         ; -> loc_1_0948
        push   dword ptr [ecx + 0x2e4]
        call   sub_5_0008
        add    esp, 0xc
        cmp    eax, edi
        je     loc_3_434B
        sub    eax, eax
        jmp    loc_3_44B6
loc_3_434B:
        push   1
        push   0x200
        call   sub_5_0028
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_3_4373
        push   dword ptr [esp + 0x1c]
        call   sub_5_0010
        add    esp, 4
        sub    eax, eax
        jmp    loc_3_44B6
loc_3_4373:
        lea    ebx, [ebp + 0x100]
        sub    esi, esi
loc_3_437B:
        mov    dword ptr [esp + 0x10], 0x100
        mov    dword ptr [esp + 0x14], 0xa
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   0
        push   eax
        lea    eax, [esp + 0x1c]
        push   0
        push   eax
        push   ebp
        push   esi
        push   dword ptr [esp + 0x38]
        call   sub_5_0018
        add    esp, 0x20
        or     eax, eax
        jne    loc_3_43ED
        cmp    dword ptr [esp + 0x14], 0xa
        jne    loc_3_449B
        cmp    dword ptr [esp + 0x18], 3
        jne    loc_3_449B
        push   0
        lea    eax, [esp + 0x14]
        push   0
        push   eax
        push   ebx
        push   ebp
        call   sub_3_2F1C
        add    esp, 0x14
        or     eax, eax
        je     loc_3_449B
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0xf
        add    edi, eax
        inc    esi
        jmp    loc_3_437B
loc_3_43ED:
        cmp    eax, 0x103
        jne    loc_3_449B
        push   1
        inc    edi
        push   edi
        call   sub_5_0028
        mov    ecx, dword ptr [esp + 0x30]
        add    esp, 8
        mov    dword ptr [ecx + 0x5fa], eax
        mov    ebx, eax
        or     ebx, ebx
        je     loc_3_449B
        sub    edi, edi
        cmp    esi, edi
        je     loc_3_4493
loc_3_441E:
        mov    dword ptr [esp + 0x10], 0x100
        mov    dword ptr [esp + 0x14], 0xa
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [ebx + 4]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x1c]
        push   0
        push   eax
        push   ebp
        push   edi
        push   dword ptr [esp + 0x38]
        call   sub_5_0018
        add    esp, 0x20
        or     eax, eax
        push   0
        jne    loc_3_44BE
        lea    eax, [esp + 0x14]
        push   0
        push   eax
        lea    eax, [ebx + 0xf]
        push   eax
        push   ebp
        call   sub_3_2F1C
        add    esp, 0x14
        or     eax, eax
        je     loc_3_44D2
        mov    eax, dword ptr [esp + 0x10]
        dec    al
        mov    byte ptr [ebx + 0xe], al
        mov    eax, edi
        sub    eax, esi
        cmp    eax, -1
        je     loc_3_448C
        mov    eax, dword ptr [esp + 0x10]
        add    eax, ebx
        add    eax, 0xf
        mov    dword ptr [ebx], eax
loc_3_448C:
        inc    edi
        mov    ebx, dword ptr [ebx]
        cmp    edi, esi
        jb     loc_3_441E
loc_3_4493:
        mov    dword ptr [esp + 0x20], 1
loc_3_449B:
        push   dword ptr [esp + 0x1c]
        call   sub_5_0010
        add    esp, 4
        push   0
        push   ebp
        call   sub_5_0030
        mov    eax, dword ptr [esp + 0x28]
        add    esp, 8
loc_3_44B6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_3_44BE:
        mov    ecx, dword ptr [esp + 0x2c]
        push   dword ptr [ecx + 0x5fa]
        call   sub_5_0030
        add    esp, 8
        jmp    loc_3_449B
loc_3_44D2:
        mov    ecx, dword ptr [esp + 0x28]
        push   0
        push   dword ptr [ecx + 0x5fa]
        call   sub_5_0030
        add    esp, 8
        jmp    loc_3_449B
sub_3_44E8:
        sub    esp, 0x44
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 0x5c]
        call   sub_3_42CC
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_3_451D
        push   1
        push   0x8b1
        call   sub_5_0028
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_3_451D
        sub    eax, eax
        jmp    loc_3_51E3
loc_3_451D:
        push   1
        push   0x400
        call   sub_5_0028
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_3_4544
        push   0
        push   ebx
        call   sub_5_0030
        add    esp, 8
        sub    eax, eax
        jmp    loc_3_51E3
loc_3_4544:
        mov    edi, dword ptr [esp + 0x5c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 0x9c]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [esp + 0x14], 0x100
        lea    edi, [ebx + 0x19c]
        lea    eax, [esp + 0x14]
        push   0
        push   eax
        push   edi
        push   1
        push   0x96c                         ; -> obj1:0x096C
        push   0
        push   dword ptr [esp + 0x70]
        call   sub_5_0050
        add    esp, 0x1c
        or     eax, eax
        je     loc_3_45C7
        mov    dword ptr [esp + 0x14], 0x100
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        push   edi
        push   1
        push   0x954                         ; -> loc_1_0954
        push   0
        push   dword ptr [esp + 0x70]
        call   sub_5_0050
        add    esp, 0x1c
        or     eax, eax
        jne    loc_3_51C4
loc_3_45C7:
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    eax, 0xff
        sub    eax, ecx
        mov    dword ptr [esp + 0x14], eax
        lea    esi, [ecx + ebx]
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        lea    eax, [esi + 0x19d]
        push   eax
        push   1
        push   0x960                         ; -> loc_1_0960
        push   0
        push   dword ptr [esp + 0x70]
        call   sub_5_0050
        add    esp, 0x1c
        or     eax, eax
        jne    loc_3_460F
        mov    byte ptr [esi + 0x19c], 0x5c
loc_3_460F:
        mov    dword ptr [ebx + 0x29c], 0
        mov    dword ptr [ebx + 0x2e0], 1
        mov    eax, dword ptr [esp + 0x58]
        mov    dword ptr [ebx + 0x98], eax
        mov    dword ptr [ebx + 0x5e2], 0
        push   1
        push   0x100
        push   ebp
        push   0
        mov    dword ptr [ebx + 0x61c], 0xffffffff
        push   dword ptr [esp + 0x68]
        call   sub_5_0048
        add    esp, 0x14
        lea    eax, [ebx + 0x2e4]
        push   eax
        push   ebp
        push   0x80000002
        call   sub_5_0008
        mov    dword ptr [esp + 0x20], 1
        add    esp, 0xc
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        lea    eax, [esp + 0x1b]
        push   eax
        push   3
        push   0x990                         ; -> obj1:0x0990
        push   0
        push   dword ptr [esp + 0x70]
        call   sub_5_0050
        add    esp, 0x1c
        cmp    dword ptr [esp + 0x14], 1
        jne    loc_3_470F
        or     eax, eax
        jne    loc_3_470F
        cmp    byte ptr [esp + 0x13], 1
        jne    loc_3_470F
        mov    dword ptr [esp + 0x14], 0x400
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   ebp
        push   eax
        push   0
        push   0x998                         ; -> obj1:0x0998
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_51C4
        cmp    dword ptr [esp + 0x18], 1
        jne    loc_3_51C4
        push   ebp
        call   sub_3_63A0
        add    esp, 4
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_470F
        push   dword ptr [esp + 0x5c]
        movzx  ecx, word ptr [0xcc]          ; -> loc_2_00CC
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_5_0058
        add    esp, 0xc
loc_3_470F:
        mov    dword ptr [esp + 0x14], 4
        lea    esi, [ebx + 0x5ae]
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        push   esi
        push   3
        push   0x9f8                         ; -> loc_1_09F8
        push   0
        push   dword ptr [esp + 0x70]
        call   sub_5_0050
        add    esp, 0x1c
        cmp    dword ptr [esp + 0x14], 4
        jne    loc_3_474A
        or     eax, eax
        jne    loc_3_474A
        cmp    dword ptr [esi], 0
        jne    loc_3_4750
loc_3_474A:
        mov    dword ptr [esi], 0xa
loc_3_4750:
        mov    dword ptr [esp + 0x14], 0x20
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        lea    eax, [esp + 0x3c]
        push   eax
        push   3
        push   0x978                         ; -> loc_1_0978
        push   0
        push   dword ptr [esp + 0x70]
        call   sub_5_0050
        add    esp, 0x1c
        cmp    dword ptr [esp + 0x14], 0x20
        jne    loc_3_51C4
        or     eax, eax
        jne    loc_3_51C4
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [ebx + 0x524], eax
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [ebx + 0x528], eax
        mov    eax, dword ptr [esi]
        imul   eax, dword ptr [esp + 0x3c]
        mov    dword ptr [ebx + 0x52c], eax
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [ebx + 0x530], eax
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [ebx + 0x534], eax
        mov    eax, dword ptr [esp + 0x48]
        mov    dword ptr [ebx + 0x538], eax
        mov    eax, dword ptr [esp + 0x4c]
        mov    dword ptr [ebx + 0x53c], eax
        mov    eax, dword ptr [esp + 0x50]
        mov    dword ptr [ebx + 0x540], eax
        mov    dword ptr [ebx + 0x4f8], 0x4d
        mov    dword ptr [ebx + 0x4fc], 0x4d
        mov    dword ptr [ebx + 0x500], 0
        mov    dword ptr [ebx + 0x504], 0
        mov    dword ptr [ebx + 0x508], 0
        lea    eax, [esp + 0x14]
        mov    dword ptr [ebx + 0x50c], 0
        mov    dword ptr [ebx + 0x510], 0
        mov    dword ptr [ebx + 0x514], 0
        mov    dword ptr [ebx + 0x518], 0
        mov    dword ptr [ebx + 0x51c], 0
        mov    dword ptr [ebx + 0x520], 0
        push   eax
        lea    eax, [esp + 0x1c]
        push   0
        push   eax
        push   0
        push   0x9c4                         ; -> loc_1_09C4
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_48C5
        cmp    dword ptr [esp + 0x18], 1
        jne    loc_3_48C5
        push   1
        push   dword ptr [esp + 0x18]
        call   sub_5_0028
        add    esp, 8
        mov    dword ptr [ebx + 0x545], eax
        or     eax, eax
        je     loc_3_48CF
        lea    ecx, [esp + 0x14]
        push   ecx
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   0
        push   0x9c4                         ; -> loc_1_09C4
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        je     loc_3_48CF
        push   0
        push   dword ptr [ebx + 0x545]
        call   sub_5_0030
        add    esp, 8
loc_3_48C5:
        mov    dword ptr [ebx + 0x545], 0
loc_3_48CF:
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   0
        push   eax
        push   0
        push   0x9d4                         ; -> obj1:0x09D4
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_4947
        cmp    dword ptr [esp + 0x18], 1
        jne    loc_3_4947
        push   1
        push   dword ptr [esp + 0x18]
        call   sub_5_0028
        add    esp, 8
        mov    dword ptr [ebx + 0x549], eax
        or     eax, eax
        je     loc_3_4951
        lea    ecx, [esp + 0x14]
        push   ecx
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   0
        push   0x9d4                         ; -> obj1:0x09D4
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        je     loc_3_4951
        push   0
        push   dword ptr [ebx + 0x549]
        call   sub_5_0030
        add    esp, 8
loc_3_4947:
        mov    dword ptr [ebx + 0x549], 0
loc_3_4951:
        mov    dword ptr [esp + 0x14], 0x14
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        push   3
        push   0x984                         ; -> obj1:0x0984
        push   0
        push   dword ptr [esp + 0x70]
        call   sub_5_0050
        add    esp, 0x1c
        cmp    dword ptr [esp + 0x14], 0x14
        jne    loc_3_51C4
        or     eax, eax
        jne    loc_3_51C4
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ebx + 0x591], eax
        mov    eax, dword ptr [esi]
        imul   eax, dword ptr [esp + 0x24]
        mov    dword ptr [ebx + 0x595], eax
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [ebx + 0x599], eax
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [ebx + 0x59d], eax
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [ebx + 0x5a1], eax
        mov    dword ptr [ebx + 0x581], 0x30
        mov    dword ptr [ebx + 0x585], 0x30
        mov    dword ptr [ebx + 0x589], 0
        mov    dword ptr [ebx + 0x58d], 0
        mov    dword ptr [ebx + 0x5a5], 0
        mov    dword ptr [ebx + 0x5a9], 0
        mov    ecx, dword ptr [ebx + 0x538]
        and    ecx, eax
        mov    dword ptr [ebx + 0x5a1], ecx
        mov    dword ptr [ebx + 0x5c2], 0
        mov    dword ptr [esp + 0x14], 0x1c
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        lea    eax, [ebx + 0x555]
        push   eax
        push   3
        push   0x98c                         ; -> loc_1_098C
        push   0
        push   dword ptr [esp + 0x70]
        call   sub_5_0050
        add    esp, 0x1c
        cmp    dword ptr [esp + 0x14], 0x1a
        je     loc_3_4A53
        cmp    dword ptr [esp + 0x14], 0x1c
        jne    loc_3_51C4
loc_3_4A53:
        or     eax, eax
        jne    loc_3_51C4
        mov    dword ptr [ebx + 0x54d], 0x61
        mov    word ptr [ebx + 0x551], 1
        mov    dword ptr [ebx + 0x571], 6
        mov    dword ptr [ebx + 0x575], 0x34
        lea    eax, [esp + 0x14]
        mov    dword ptr [ebx + 0x579], 0x2d
        mov    dword ptr [esp + 0x14], 0x100
        push   eax
        lea    eax, [esp + 0x1c]
        push   ebp
        push   eax
        push   0
        push   0x9dc                         ; -> obj1:0x09DC
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_4B3F
        cmp    dword ptr [esp + 0x18], 1
        jne    loc_3_4B3F
        cmp    dword ptr [esp + 0x14], 1
        jbe    loc_3_4B3F
        lea    esi, [ebp + 0x100]
        push   0
        push   0
        push   0
        push   esi
        mov    edi, esi
        push   ebp
        call   sub_3_2F1C
        add    esp, 0x14
        push   1
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        call   sub_5_0028
        add    esp, 8
        mov    dword ptr [ebx + 0x5ee], eax
        mov    edx, eax
        or     edx, edx
        je     loc_3_4B49
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [ebx + 0x5ee]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 0x5f2], ecx
        jmp    loc_3_4B49
loc_3_4B3F:
        mov    dword ptr [ebx + 0x5ee], 0
loc_3_4B49:
        lea    eax, [ebp + 0x100]
        mov    dword ptr [esp + 0x1c], eax
        mov    dword ptr [esp + 0x14], 4
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [ebx + 0x824]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   0
        push   0xa3c                         ; -> obj1:0x0A3C
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_501F
        cmp    dword ptr [esp + 0x18], 3
        jne    loc_3_501F
        cmp    dword ptr [esp + 0x14], 4
        jb     loc_3_501F
        mov    dword ptr [esp + 0x14], 0x10
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [ebx + 0x814]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   0
        push   0xa4c                         ; -> obj1:0x0A4C
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        mov    dword ptr [esp + 0x2c], 4
        add    esp, 0x18
        lea    esi, [ebx + 0x80c]
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   esi
        push   eax
        push   0
        push   0xa60                         ; -> loc_1_0A60
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_4C0D
        cmp    dword ptr [esp + 0x18], 3
        jne    loc_3_4C0D
        cmp    dword ptr [esp + 0x14], 4
        jae    loc_3_4C13
loc_3_4C0D:
        mov    dword ptr [esi], 0x1e
loc_3_4C13:
        mov    dword ptr [esp + 0x14], 0x100
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   ebp
        push   eax
        push   0
        push   0xa08                         ; -> obj1:0x0A08
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_4CBA
        cmp    dword ptr [esp + 0x18], 1
        jne    loc_3_4CBA
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x28]
        push   ebp
        call   sub_3_2F1C
        add    esp, 0x14
        mov    edi, dword ptr [esp + 0x1c]
        push   1
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        call   sub_5_0028
        add    esp, 8
        mov    dword ptr [ebx + 0x792], eax
        mov    edx, eax
        or     edx, edx
        je     loc_3_4CBA
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [ebx + 0x792]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 0x796], ecx
loc_3_4CBA:
        mov    dword ptr [esp + 0x14], 0x100
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   ebp
        push   eax
        push   0
        push   0xa1c                         ; -> obj1:0x0A1C
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_4D49
        cmp    dword ptr [esp + 0x18], 1
        jne    loc_3_4D49
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x28]
        push   ebp
        call   sub_3_2F1C
        add    esp, 0x14
        mov    edi, dword ptr [esp + 0x1c]
        push   1
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        call   sub_5_0028
        add    esp, 8
        mov    dword ptr [ebx + 0x79a], eax
        mov    edx, eax
        or     edx, edx
        je     loc_3_4D49
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_4D49:
        mov    dword ptr [esp + 0x14], 0x100
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   ebp
        push   eax
        push   0
        push   0xa2c                         ; -> obj1:0x0A2C
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_4DD8
        cmp    dword ptr [esp + 0x18], 1
        jne    loc_3_4DD8
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x28]
        push   ebp
        call   sub_3_2F1C
        add    esp, 0x14
        mov    edi, dword ptr [esp + 0x1c]
        push   1
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        call   sub_5_0028
        add    esp, 8
        mov    dword ptr [ebx + 0x79e], eax
        mov    edx, eax
        or     edx, edx
        je     loc_3_4DD8
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_4DD8:
        test   byte ptr [ebx + 0x824], 0x20
        je     loc_3_4FAD
        mov    dword ptr [esp + 0x14], 0x14
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   ebp
        push   eax
        push   0
        push   0xa74                         ; -> loc_1_0A74
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_4E7D
        cmp    dword ptr [esp + 0x18], 1
        jne    loc_3_4E7D
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x28]
        push   ebp
        call   sub_3_2F1C
        mov    edi, dword ptr [esp + 0x30]
        add    esp, 0x14
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 0x14
        jae    loc_3_4E7D
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 0x869]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 0x87d], ecx
loc_3_4E7D:
        mov    dword ptr [esp + 0x14], 0x14
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   ebp
        push   eax
        push   0
        push   0xa84                         ; -> loc_1_0A84
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_4F15
        cmp    dword ptr [esp + 0x18], 1
        jne    loc_3_4F15
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x28]
        push   ebp
        call   sub_3_2F1C
        mov    edi, dword ptr [esp + 0x30]
        add    esp, 0x14
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 0x14
        jae    loc_3_4F15
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 0x881]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 0x895], ecx
loc_3_4F15:
        mov    dword ptr [esp + 0x14], 0x14
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   ebp
        push   eax
        push   0
        push   0xa90                         ; -> loc_1_0A90
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_4FAD
        cmp    dword ptr [esp + 0x18], 1
        jne    loc_3_4FAD
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x28]
        push   ebp
        call   sub_3_2F1C
        mov    edi, dword ptr [esp + 0x30]
        add    esp, 0x14
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 0x14
        jae    loc_3_4FAD
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebx + 0x851]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 0x865], ecx
loc_3_4FAD:
        mov    dword ptr [esp + 0x14], 0x80
        push   1
        push   0x80
        call   sub_5_0028
        add    esp, 8
        mov    dword ptr [ebx + 0x849], eax
        or     eax, eax
        je     loc_3_5029
        lea    ecx, [esp + 0x14]
        push   ecx
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        push   0
        push   0xaa0                         ; -> obj1:0x0AA0
        push   dword ptr [ebx + 0x2e4]
        call   sub_5_0020
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_5003
        cmp    dword ptr [esp + 0x18], 3
        jne    loc_3_5003
        cmp    dword ptr [esp + 0x14], 0x80
        jae    loc_3_5029
loc_3_5003:
        push   0
        push   dword ptr [ebx + 0x849]
        call   sub_5_0030
        add    esp, 8
        mov    dword ptr [ebx + 0x849], 0xd18 ; -> obj1:0x0D18
        jmp    loc_3_5029
loc_3_501F:
        mov    dword ptr [ebx + 0x824], 0x1080
loc_3_5029:
        mov    dword ptr [ebx + 0x838], 0
        mov    dword ptr [ebx + 0x83c], 0
        mov    dword ptr [ebx + 0x834], 0
        mov    dword ptr [ebx + 0x5be], 1
        lea    eax, [ebx + 0x7a2]
        push   ebx
        push   0
        push   0
        push   0
        push   dword ptr [ebx + 0x849]
        push   eax
        call   sub_3_7ED0
        push   ebx
        call   sub_3_4313
        add    esp, 4
        or     eax, eax
        je     loc_3_51C4
        mov    dword ptr [esp + 0x14], 0x100
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        push   ebp
        push   1
        push   0x9a4                         ; -> obj1:0x09A4
        push   0
        push   dword ptr [esp + 0x70]
        call   sub_5_0050
        add    esp, 0x1c
        or     eax, eax
        jne    loc_3_50C5
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_50C5
        push   ebp
        movzx  ecx, word ptr [0xce]          ; -> obj2:0x00CE
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_5_0058
        add    esp, 0xc
loc_3_50C5:
        mov    dword ptr [esp + 0x14], 0x100
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        push   ebp
        push   1
        push   0x9b0                         ; -> loc_1_09B0
        push   0
        push   dword ptr [esp + 0x70]
        call   sub_5_0050
        add    esp, 0x1c
        or     eax, eax
        jne    loc_3_510F
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_510F
        push   ebp
        movzx  ecx, word ptr [0xd0]          ; -> obj2:0x00D0
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_5_0058
        add    esp, 0xc
loc_3_510F:
        mov    dword ptr [esp + 0x14], 0x100
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        push   ebp
        push   1
        push   0x9b8                         ; -> loc_1_09B8
        push   0
        push   dword ptr [esp + 0x70]
        call   sub_5_0050
        add    esp, 0x1c
        or     eax, eax
        jne    loc_3_5159
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_5159
        push   ebp
        movzx  ecx, word ptr [0xd2]          ; -> obj2:0x00D2
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_5_0058
        add    esp, 0xc
loc_3_5159:
        mov    dword ptr [esp + 0x14], 1
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        lea    eax, [esp + 0x1a]
        push   eax
        push   3
        push   0x9e4                         ; -> loc_1_09E4
        push   0
        push   dword ptr [esp + 0x70]
        call   sub_5_0050
        add    esp, 0x1c
        or     eax, eax
        jne    loc_3_51C4
        cmp    dword ptr [esp + 0x14], 1
        jne    loc_3_51C4
        movzx  eax, byte ptr [esp + 0x12]
        or     eax, eax
        je     loc_3_51EB
        cmp    eax, 2
        je     loc_3_51A0
        cmp    eax, 4
        je     loc_3_51EB
loc_3_51A0:
        mov    dword ptr [ebx + 0x618], 1
        push   0
        push   0x9f0                         ; -> loc_1_09F0
        push   ebx
        call   sub_3_31F4
        add    esp, 0xc
        mov    dword ptr [ebx + 0x5de], eax
        or     eax, eax
        jne    loc_3_51F5
loc_3_51C4:
        call   sub_3_648B
        push   0
        push   ebx
        call   sub_5_0030
        add    esp, 8
        sub    ebx, ebx
loc_3_51D6:
        push   0
        push   ebp
        call   sub_5_0030
        add    esp, 8
        mov    eax, ebx
loc_3_51E3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x44
        ret    
loc_3_51EB:
        mov    dword ptr [ebx + 0x618], 0
loc_3_51F5:
        mov    eax, dword ptr [0x944]        ; -> loc_1_0944
        or     eax, eax
        mov    dword ptr [ebx + 0x94], eax
        jne    loc_3_520E
        mov    dword ptr [ebx + 0x94], 0
loc_3_520E:
        mov    dword ptr [0x944], ebx        ; -> loc_1_0944
        jmp    loc_3_51D6
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_5220:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 0x12], 0
        mov    ebx, dword ptr [esp + 0x1c]
        or     ebx, ebx
        je     loc_3_53F1
        mov    ebp, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [ebp + 0x29c]
        or     ecx, ecx
        je     loc_3_53F1
        cmp    dword ptr [ebx + 0x34], 0xe73cf52e
        jne    loc_3_5268
        mov    ax, word ptr [ebx]
        cmp    ax, 0x40
        jae    loc_3_5263
        jmp    loc_3_53F1
loc_3_5263:
        mov    word ptr [esp + 0x12], ax
loc_3_5268:
        push   ebx
        push   ecx
        call   sub_1_0478
        add    esp, 8
        or     eax, eax
        jne    loc_3_527B
        jmp    loc_3_53F1
loc_3_527B:
        mov    dword ptr [ebx + 0x18], 6
        or     dword ptr [ebx + 4], 1
        cmp    word ptr [esp + 0x12], 0x8c
        jb     loc_3_53EA
        add    ebx, 0x3c
        lea    esi, [ebp + 0x4f8]
        mov    edi, ebx
        mov    ecx, 0x13
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [ebp + 0x545]
        or     edi, edi
        je     loc_3_533C
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        add    dword ptr [ebx + 4], ecx
        mov    edi, dword ptr [ebp + 0x545]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ecx, 0x8b
        movzx  eax, word ptr [esp + 0x12]
        cmp    ecx, eax
        ja     loc_3_533C
        mov    esi, dword ptr [ebp + 0x545]
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, dword ptr [ebx]
        lea    eax, [ecx - 1]
        add    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edx, dword ptr [ebx]
        mov    dword ptr [ebx + 0x14], edx
        mov    edi, dword ptr [ebp + 0x545]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 0x18], ecx
        mov    edi, dword ptr [ebp + 0x545]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [edx + ecx - 1]
        mov    dword ptr [ebx], eax
loc_3_533C:
        mov    edi, dword ptr [ebp + 0x549]
        or     edi, edi
        je     loc_3_53EA
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        add    dword ptr [ebx + 4], ecx
        mov    edi, dword ptr [ebp + 0x545]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, dword ptr [ebp + 0x549]
        lea    edx, [ecx - 1]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    edx, [edx + ecx - 1]
        add    edx, 0x8c
        movzx  eax, word ptr [esp + 0x12]
        cmp    edx, eax
        ja     loc_3_53EA
        mov    esi, dword ptr [ebp + 0x549]
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, dword ptr [ebx]
        lea    eax, [ecx - 1]
        add    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edx, dword ptr [ebx]
        mov    dword ptr [ebx + 0x1c], edx
        mov    edi, dword ptr [ebp + 0x549]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    dword ptr [ebx + 0x20], ecx
        mov    edi, dword ptr [ebp + 0x549]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [edx + ecx - 1]
        mov    dword ptr [ebx], eax
loc_3_53EA:
        mov    eax, 1
        jmp    loc_3_53F3
loc_3_53F1:
        sub    eax, eax
loc_3_53F3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_3_53FB:
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x44]
        or     edi, edi
        mov    ebx, dword ptr [esp + 0x48]
        je     loc_3_54B9
        or     ebx, ebx
        je     loc_3_54C3
        cmp    dword ptr [ebx], 0x34
        jb     loc_3_54B9
        mov    esi, dword ptr [esp + 0x40]
        mov    eax, dword ptr [esi + 0x29c]
        or     eax, eax
        je     loc_3_54B9
        mov    word ptr [edi + 4], 1
        mov    dword ptr [edi + 0x24], 6
        lea    ecx, [esp + 0xc]
        push   ecx
        push   eax
        call   sub_1_0460
        add    esp, 8
        or     eax, eax
        jne    loc_3_5455
        jmp    loc_3_54C3
loc_3_5455:
        lea    eax, [edi + 8]
        push   eax
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_06C0
        add    esp, 8
        cmp    dword ptr [ebx], 0x60
        jae    loc_3_5487
        mov    dword ptr [ebx], 0x34
        mov    dword ptr [edi], 0x34
        mov    dword ptr [edi + 0x28], 0
        mov    dword ptr [edi + 0x2c], 0
        jmp    loc_3_54B2
loc_3_5487:
        mov    dword ptr [ebx], 0x60
        mov    dword ptr [edi], 0x60
        mov    dword ptr [edi + 0x28], 0x30
        mov    dword ptr [edi + 0x2c], 0x30
        add    esi, 0x581
        add    edi, 0x30
        mov    ecx, 0xb
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_3_54B2:
        mov    eax, 1
        jmp    loc_3_54C5
loc_3_54B9:
        or     ebx, ebx
        je     loc_3_54C3
        mov    dword ptr [ebx], 0x34
loc_3_54C3:
        sub    eax, eax
loc_3_54C5:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
sub_3_54CC:
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x48]
        or     ebp, ebp
        je     loc_3_5647
        cmp    dword ptr [esp + 0x4c], 0x34
        jb     loc_3_5647
        mov    ebx, dword ptr [esp + 0x44]
        mov    eax, dword ptr [ebx + 0x29c]
        or     eax, eax
        je     loc_3_5647
        cmp    word ptr [ebp + 4], 1
        jne    loc_3_5647
        cmp    dword ptr [ebp], 0x34
        jb     loc_3_5647
        lea    ecx, [esp + 0x10]
        push   ecx
        push   eax
        call   sub_1_0460
        add    esp, 8
        or     eax, eax
        jne    loc_3_5528
        jmp    loc_3_5647
loc_3_5528:
        lea    esi, [ebp + 8]
        lea    eax, [esp + 0x10]
        push   eax
        push   esi
        call   sub_1_06D3
        add    esp, 8
        lea    eax, [esp + 0x10]
        push   -1
        push   eax
        push   dword ptr [ebx + 0x29c]
        call   sub_1_0458
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_5557
        jmp    loc_3_5647
loc_3_5557:
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_3_34E7
        add    esp, 4
        mov    edi, dword ptr [ebx + 0x29c]
        push   0
        push   0
        push   5
        push   edi
        call   sub_1_0480
        add    esp, 0x10
        or     eax, eax
        jne    loc_3_5582
        jmp    loc_3_5647
loc_3_5582:
        push   0
        push   0
        push   3
        push   edi
        call   sub_1_0480
        add    esp, 0x10
        or     eax, eax
        jne    loc_3_559A
        jmp    loc_3_5647
loc_3_559A:
        lea    edi, [ebx + 0x555]
        mov    ecx, 7
        rep movsd dword ptr es:[edi], dword ptr [esi]
        cmp    dword ptr [ebp + 0x24], 6
        jne    loc_3_5640
        mov    eax, dword ptr [ebp + 0x2c]
        cmp    eax, 0x2d
        jb     loc_3_5640
        mov    ecx, dword ptr [ebp + 0x28]
        add    eax, ecx
        cmp    eax, dword ptr [ebp]
        ja     loc_3_5640
        lea    edi, [ecx + ebp]
        lea    edx, [ebx + 0x581]
        mov    ecx, dword ptr [ebx + 0x528]
        lea    esi, [ebx + 0x4f8]
        mov    eax, dword ptr [edi + 0x10]
        cmp    ecx, eax
        jae    loc_3_55E8
        mov    dword ptr [edx + 0x10], ecx
        jmp    loc_3_55F7
loc_3_55E8:
        cmp    eax, 1
        mov    dword ptr [edx + 0x10], 1
        jb     loc_3_55F7
        mov    dword ptr [edx + 0x10], eax
loc_3_55F7:
        mov    eax, dword ptr [edi + 0x14]
        mov    ecx, dword ptr [esi + 0x34]
        cmp    eax, ecx
        jbe    loc_3_5606
        mov    dword ptr [edx + 0x14], ecx
        jmp    loc_3_5609
loc_3_5606:
        mov    dword ptr [edx + 0x14], eax
loc_3_5609:
        mov    ecx, dword ptr [edi + 0x18]
        mov    eax, 1
        shl    eax, cl
        test   dword ptr [esi + 0x38], eax
        je     loc_3_561B
        mov    dword ptr [edx + 0x18], ecx
loc_3_561B:
        mov    ecx, dword ptr [edi + 0x1c]
        mov    eax, 1
        shl    eax, cl
        test   dword ptr [esi + 0x3c], eax
        je     loc_3_562D
        mov    dword ptr [edx + 0x1c], ecx
loc_3_562D:
        mov    eax, dword ptr [edi + 0x20]
        and    eax, dword ptr [esi + 0x40]
        mov    dword ptr [edx + 0x20], eax
        mov    dword ptr [ebx + 0x5c2], 0
loc_3_5640:
        mov    eax, 1
        jmp    loc_3_5649
loc_3_5647:
        sub    eax, eax
loc_3_5649:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
sub_3_5651:
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x40]
        mov    dword ptr [esp + 0x38], 0
        mov    eax, dword ptr [ebx + 0x5a1]
        mov    dword ptr [esp + 0x2c], eax
        mov    eax, dword ptr [ebx + 0x538]
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x24], 1
        push   1
        push   0x9a
        call   sub_1_04C8
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
        or     eax, eax
        jne    loc_3_56AC
        push   0
        push   dword ptr [esp + 0x14]
        call   sub_1_04D0
        add    esp, 8
        sub    eax, eax
        jmp    loc_3_5E91
loc_3_56AC:
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0x29
        mov    dword ptr [esp + 0x18], eax
        add    eax, 0x29
        mov    dword ptr [esp + 0x20], eax
        mov    ebp, eax
        add    ebp, 0x29
        push   0xab8                         ; -> loc_1_0AB8
        push   dword ptr [ebx + 0x2e4]
        call   sub_1_0440
        lea    eax, [esp + 0x24]
        add    esp, 8
        push   eax
        push   0xab8                         ; -> loc_1_0AB8
        push   dword ptr [ebx + 0x2e4]
        call   sub_1_0438
        add    esp, 0xc
        or     eax, eax
        je     loc_3_5707
        push   0
        push   dword ptr [esp + 0x14]
        call   sub_1_04D0
        add    esp, 8
        sub    eax, eax
        jmp    loc_3_5E91
loc_3_5707:
        lea    eax, [esp + 0x14]
        push   eax
        push   0xaac                         ; -> obj1:0x0AAC
        push   dword ptr [ebx + 0x2e4]
        call   sub_1_0428
        add    esp, 0xc
        or     eax, eax
        je     loc_3_5744
        push   dword ptr [esp + 0x1c]
        call   sub_1_0430
        add    esp, 4
        push   0
        push   dword ptr [esp + 0x14]
        call   sub_1_04D0
        add    esp, 8
        sub    eax, eax
        jmp    loc_3_5E91
loc_3_5744:
        mov    dword ptr [esp + 0x34], 0x28
        lea    eax, [esp + 0x34]
        push   eax
        lea    eax, [esp + 0x34]
        push   dword ptr [esp + 0x14]
        push   eax
        push   0
        push   0xac8                         ; -> obj1:0x0AC8
        push   dword ptr [esp + 0x28]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_5E67
        cmp    dword ptr [esp + 0x30], 1
        jne    loc_3_5E67
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x20]
        call   sub_3_2F1C
        mov    dword ptr [esp + 0x48], 0x28
        lea    eax, [esp + 0x48]
        add    esp, 0x14
        push   eax
        lea    eax, [esp + 0x34]
        push   dword ptr [esp + 0x14]
        push   eax
        push   0
        push   0xad0                         ; -> obj1:0x0AD0
        push   dword ptr [esp + 0x28]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_5E67
        cmp    dword ptr [esp + 0x30], 1
        jne    loc_3_5E67
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x20]
        call   sub_3_2F1C
        mov    edi, dword ptr [esp + 0x2c]
        add    esp, 0x14
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 0x10]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    dword ptr [ebx + 0x528], 0
        je     loc_3_5850
        lea    eax, [esp + 0x24]
        push   dword ptr [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        push   dword ptr [ebx + 0x591]
        push   0xadc                         ; -> obj1:0x0ADC
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebx + 0x2e4]
        call   sub_3_5E99
        add    esp, 0x24
        or     eax, eax
        je     loc_3_5E67
loc_3_5850:
        cmp    dword ptr [ebx + 0x52c], 0
        je     loc_3_58AF
        mov    ecx, dword ptr [ebx + 0x595]
        mov    esi, dword ptr [ebx + 0x5ae]
        mov    eax, ecx
        sub    edx, edx
        div    esi
        cmp    edx, 1
        sbb    edi, edi
        mov    eax, ecx
        sub    edx, edx
        div    esi
        lea    ecx, [esp + 0x24]
        push   dword ptr [esp + 0x10]
        push   ecx
        lea    eax, [edi + eax + 1]
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        push   eax
        push   0xaf0                         ; -> obj1:0x0AF0
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebx + 0x2e4]
        call   sub_3_5E99
        add    esp, 0x24
        or     eax, eax
        je     loc_3_5E67
loc_3_58AF:
        cmp    dword ptr [ebx + 0x530], 0
        je     loc_3_5966
        mov    edi, 0xb04                    ; -> obj1:0x0B04
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebx + 0x599]
        or     eax, eax
        je     loc_3_58F6
        cmp    eax, 1
        je     loc_3_58FD
        cmp    eax, 2
        je     loc_3_5904
        jmp    loc_3_5934
loc_3_58F6:
        mov    edi, 0xb84                    ; -> loc_1_0B84
        jmp    loc_3_5909
loc_3_58FD:
        mov    edi, 0xb8c                    ; -> obj1:0x0B8C
        jmp    loc_3_5909
loc_3_5904:
        mov    edi, 0xb94                    ; -> loc_1_0B94
loc_3_5909:
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_5934:
        lea    eax, [esp + 0x24]
        push   dword ptr [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        push   0
        push   ebp
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebx + 0x2e4]
        call   sub_3_5E99
        add    esp, 0x24
        or     eax, eax
        je     loc_3_5E67
loc_3_5966:
        cmp    dword ptr [ebx + 0x534], 0
        je     loc_3_5A29
        mov    edi, 0xb14                    ; -> obj1:0x0B14
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebx + 0x59d]
        or     eax, eax
        je     loc_3_59B2
        cmp    eax, 1
        je     loc_3_59B9
        cmp    eax, 2
        je     loc_3_59C0
        cmp    eax, 3
        je     loc_3_59C7
        jmp    loc_3_59F7
loc_3_59B2:
        mov    edi, 0xbd0                    ; -> loc_1_0BD0
        jmp    loc_3_59CC
loc_3_59B9:
        mov    edi, 0xb9c                    ; -> obj1:0x0B9C
        jmp    loc_3_59CC
loc_3_59C0:
        mov    edi, 0xbd8                    ; -> loc_1_0BD8
        jmp    loc_3_59CC
loc_3_59C7:
        mov    edi, 0xba4                    ; -> obj1:0x0BA4
loc_3_59CC:
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_59F7:
        lea    eax, [esp + 0x24]
        push   dword ptr [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        push   0
        push   ebp
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebx + 0x2e4]
        call   sub_3_5E99
        add    esp, 0x24
        or     eax, eax
        je     loc_3_5E67
loc_3_5A29:
        test   byte ptr [esp + 0x28], 2
        je     loc_3_5B1A
        mov    edi, 0xb2c                    ; -> obj1:0x0B2C
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x2c]
        and    eax, 0xe
        cmp    eax, 2
        je     loc_3_5A7A
        cmp    eax, 6
        je     loc_3_5AB8
        cmp    eax, 0xa
        je     loc_3_5A81
        cmp    eax, 0xe
        je     loc_3_5A88
        mov    edi, 0xbd0                    ; -> loc_1_0BD0
        jmp    loc_3_5ABD
loc_3_5A7A:
        mov    edi, 0xbd8                    ; -> loc_1_0BD8
        jmp    loc_3_5ABD
loc_3_5A81:
        mov    edi, 0xbb4                    ; -> obj1:0x0BB4
        jmp    loc_3_5ABD
loc_3_5A88:
        mov    edi, 0xbb4                    ; -> obj1:0x0BB4
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_5AB8:
        mov    edi, 0xbac                    ; -> obj1:0x0BAC
loc_3_5ABD:
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x24]
        push   dword ptr [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        push   0
        push   ebp
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebx + 0x2e4]
        call   sub_3_5E99
        add    esp, 0x24
        or     eax, eax
        je     loc_3_5E67
loc_3_5B1A:
        test   byte ptr [esp + 0x28], 1
        je     loc_3_5BB7
        mov    edi, 0xb3c                    ; -> loc_1_0B3C
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, 0xbd8                    ; -> loc_1_0BD8
        test   byte ptr [esp + 0x2c], 1
        jne    loc_3_5B5A
        mov    edi, 0xbd0                    ; -> loc_1_0BD0
loc_3_5B5A:
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x24]
        push   dword ptr [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        push   0
        push   ebp
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebx + 0x2e4]
        call   sub_3_5E99
        add    esp, 0x24
        or     eax, eax
        je     loc_3_5E67
loc_3_5BB7:
        test   byte ptr [esp + 0x28], 0x30
        je     loc_3_5C7A
        mov    edi, 0xb20                    ; -> obj1:0x0B20
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x2c]
        and    eax, 0x30
        cmp    eax, 0x10
        je     loc_3_5C03
        cmp    eax, 0x20
        je     loc_3_5C18
        cmp    eax, 0x30
        je     loc_3_5C0A
        mov    edi, 0xbd0                    ; -> loc_1_0BD0
        jmp    loc_3_5C1D
loc_3_5C03:
        mov    edi, 0xbc0                    ; -> loc_1_0BC0
        jmp    loc_3_5C1D
loc_3_5C0A:
        test   byte ptr [esp + 0x28], 0x10
        je     loc_3_5C18
        mov    edi, 0xbc0                    ; -> loc_1_0BC0
        jmp    loc_3_5C1D
loc_3_5C18:
        mov    edi, 0xbc8                    ; -> obj1:0x0BC8
loc_3_5C1D:
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x24]
        push   dword ptr [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        push   0
        push   ebp
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebx + 0x2e4]
        call   sub_3_5E99
        add    esp, 0x24
        or     eax, eax
        je     loc_3_5E67
loc_3_5C7A:
        test   byte ptr [esp + 0x28], 0x40
        je     loc_3_5D2E
        mov    edi, 0xb48                    ; -> obj1:0x0B48
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        test   byte ptr [esp + 0x2c], 0x40
        je     loc_3_5CCC
        test   byte ptr [esp + 0x29], 4
        je     loc_3_5CC5
        test   byte ptr [esp + 0x2d], 4
        je     loc_3_5CC5
        mov    edi, 0xb64                    ; -> obj1:0x0B64
        jmp    loc_3_5CD1
loc_3_5CC5:
        mov    edi, 0xb54                    ; -> loc_1_0B54
        jmp    loc_3_5CD1
loc_3_5CCC:
        mov    edi, 0xb5c                    ; -> loc_1_0B5C
loc_3_5CD1:
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x24]
        push   dword ptr [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        push   0
        push   ebp
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebx + 0x2e4]
        call   sub_3_5E99
        add    esp, 0x24
        or     eax, eax
        je     loc_3_5E67
loc_3_5D2E:
        test   byte ptr [esp + 0x28], 0x80
        je     loc_3_5DCB
        mov    edi, 0xb70                    ; -> loc_1_0B70
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, 0xbd8                    ; -> loc_1_0BD8
        test   byte ptr [esp + 0x2c], 0x80
        jne    loc_3_5D6E
        mov    edi, 0xbd0                    ; -> loc_1_0BD0
loc_3_5D6E:
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x24]
        push   dword ptr [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        push   0
        push   ebp
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebx + 0x2e4]
        call   sub_3_5E99
        add    esp, 0x24
        or     eax, eax
        je     loc_3_5E67
loc_3_5DCB:
        test   byte ptr [esp + 0x29], 2
        je     loc_3_5E30
        mov    edi, 0x8a4                    ; -> loc_1_08A4
        test   byte ptr [esp + 0x2d], 2
        jne    loc_3_5DE3
        mov    edi, 0x8b0                    ; -> loc_1_08B0
loc_3_5DE3:
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x24]
        push   dword ptr [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        push   0
        push   ebp
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebx + 0x2e4]
        call   sub_3_5E99
        add    esp, 0x24
        or     eax, eax
        je     loc_3_5E67
loc_3_5E30:
        lea    eax, [esp + 0x24]
        push   dword ptr [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x24]
        push   0
        push   0
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x30]
        push   dword ptr [ebx + 0x2e4]
        call   sub_3_5E99
        add    esp, 0x24
        or     eax, eax
        je     loc_3_5E67
        mov    dword ptr [esp + 0x38], 1
loc_3_5E67:
        push   dword ptr [esp + 0x1c]
        call   sub_1_0430
        add    esp, 4
        push   dword ptr [esp + 0x14]
        call   sub_1_0430
        add    esp, 4
        push   0
        push   dword ptr [esp + 0x14]
        call   sub_1_04D0
        mov    eax, dword ptr [esp + 0x40]
        add    esp, 8
loc_3_5E91:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x2c
        ret    
sub_3_5E99:
        sub    esp, 0x270
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x290]
        or     esi, esi
        je     loc_3_60D2
        mov    dword ptr [esp + 0x10], 0x28
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x58]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   0
        push   esi
        push   dword ptr [esp + 0x29c]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        je     loc_3_5F0A
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_638C
        push   esi
        movzx  eax, word ptr [0xc6]          ; -> loc_2_00C6
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 0xc
        jmp    loc_3_638C
loc_3_5F0A:
        cmp    dword ptr [esp + 0x14], 1
        je     loc_3_5F18
        sub    eax, eax
        jmp    loc_3_6391
loc_3_5F18:
        mov    edi, 0xbe8                    ; -> obj1:0x0BE8
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 0x80]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x1c]
        push   eax
        push   dword ptr [esp + 0x298]
        call   sub_3_30F5
        lea    edi, [esp + 0x24]
        add    esp, 8
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 0x180]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esp + 0x18], eax
        lea    eax, [esp + 0x80]
        push   1
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x34]
        push   eax
        lea    eax, [esp + 0x64]
        push   eax
        call   sub_3_2F1C
        add    esp, 0x14
        or     eax, eax
        jne    loc_3_5FB1
        sub    eax, eax
        jmp    loc_3_6391
loc_3_5FB1:
        mov    ebx, dword ptr [esp + 0x2a4]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, dword ptr [esp + 0x29c]
        lea    edx, [ecx - 1]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    edx, [edx + ecx - 1]
        lea    edi, [esp + 0x28]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    edx, [edx + ecx - 1]
        cmp    edx, 0x28
        jbe    loc_3_609E
        mov    edi, dword ptr [esp + 0x29c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x1c]
        mov    ebp, dword ptr [esp + 0x2a0]
        push   eax
        push   dword ptr [ebp]
        call   sub_3_30F5
        add    esp, 8
        inc    dword ptr [ebp]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        push   ebx
        push   1
        lea    eax, [esp + 0x28]
        push   0
        push   eax
        push   dword ptr [esp + 0x2a0]
        call   sub_1_0450
        add    esp, 0x18
        or     eax, eax
        je     loc_3_6078
        sub    eax, eax
        jmp    loc_3_6391
loc_3_6078:
        mov    edi, dword ptr [esp + 0x298]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_609E:
        lea    edi, [esp + 0x28]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_3_638C
loc_3_60D2:
        mov    edi, dword ptr [esp + 0x29c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    ebx, dword ptr [esp + 0x2a4]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x1c]
        mov    ebp, dword ptr [esp + 0x2a0]
        push   eax
        push   dword ptr [ebp]
        call   sub_3_30F5
        add    esp, 8
        inc    dword ptr [ebp]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        push   ebx
        push   1
        lea    eax, [esp + 0x28]
        push   0
        push   eax
        push   dword ptr [esp + 0x2a0]
        call   sub_1_0450
        add    esp, 0x18
        or     eax, eax
        je     loc_3_6157
        sub    eax, eax
        jmp    loc_3_6391
loc_3_6157:
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   0
        push   eax
        push   0
        push   0xbdc                         ; -> loc_1_0BDC
        push   dword ptr [esp + 0x298]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        je     loc_3_6182
        jmp    loc_3_638C
loc_3_6182:
        cmp    dword ptr [esp + 0x14], 1
        je     loc_3_6190
        sub    eax, eax
        jmp    loc_3_6391
loc_3_6190:
        cmp    dword ptr [esp + 0x10], 1
        jne    loc_3_619C
        jmp    loc_3_638C
loc_3_619C:
        mov    edi, dword ptr [esp + 0x29c]
        push   1
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, dword ptr [esp + 0x29c]
        lea    edx, [ecx - 1]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    edx, [edx + ecx - 1]
        add    edx, dword ptr [esp + 0x14]
        inc    edx
        push   edx
        call   sub_1_04C8
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_3_61E4
        sub    eax, eax
        jmp    loc_3_6391
loc_3_61E4:
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x18]
        push   ebx
        push   eax
        push   0
        push   0xbdc                         ; -> loc_1_0BDC
        push   dword ptr [esp + 0x298]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        je     loc_3_621B
        push   0
        push   ebx
        call   sub_1_04D0
        add    esp, 8
        sub    eax, eax
        jmp    loc_3_6391
loc_3_621B:
        mov    edi, dword ptr [esp + 0x298]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        push   ecx
        push   dword ptr [esp + 0x29c]
        push   ebx
        call   sub_3_3146
        add    esp, 0xc
        or     eax, eax
        je     loc_3_62B6
        mov    edi, dword ptr [esp + 0x298]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x10]
        mov    edi, dword ptr [esp + 0x298]
        push   eax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [ebx + ecx - 1]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        push   0
        push   0xbdc                         ; -> loc_1_0BDC
        push   dword ptr [esp + 0x298]
        call   sub_1_0448
        add    esp, 0x18
        or     eax, eax
        je     loc_3_62B6
        push   0
        push   ebx
        call   sub_1_04D0
        add    esp, 8
        sub    eax, eax
        jmp    loc_3_6391
loc_3_62B6:
        mov    edi, dword ptr [esp + 0x29c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        push   ecx
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        push   dword ptr [esp + 0x2a0]
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, dword ptr [esp + 0x2a4]
        lea    edx, [ecx - 1]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        sub    edx, ecx
        add    edx, ebx
        push   edx
        call   sub_3_3146
        add    esp, 0xc
        or     eax, eax
        je     loc_3_6337
        mov    edi, dword ptr [esp + 0x29c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_6337:
        lea    eax, [esp + 0x1c]
        push   eax
        push   dword ptr [ebp]
        call   sub_3_30F5
        add    esp, 8
        inc    dword ptr [ebp]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        push   ecx
        push   ebx
        push   1
        lea    eax, [esp + 0x28]
        push   0
        push   eax
        push   dword ptr [esp + 0x2a0]
        call   sub_1_0450
        add    esp, 0x18
        or     eax, eax
        push   0
        push   ebx
        je     loc_3_6384
        call   sub_1_04D0
        add    esp, 8
        sub    eax, eax
        jmp    loc_3_6391
loc_3_6384:
        call   sub_1_04D0
        add    esp, 8
loc_3_638C:
        mov    eax, 1
loc_3_6391:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x270
        ret    
        db  000h, 000h, 000h, 000h
sub_3_63A0:
        sub    esp, 0x10c
        push   esi
        push   edi
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        jne    loc_3_646A
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_0554
        mov    edi, eax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 0x14]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [esp + eax + 0x14], 0
        mov    edi, 0xbec                    ; -> loc_1_0BEC
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 0x14]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x14]
        push   1
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_03E0
        or     eax, eax
        je     loc_3_643B
        sub    eax, eax
        jmp    loc_3_6482
loc_3_643B:
        push   0
        push   dword ptr [esp + 0x11c]
        call   sub_1_0578
        add    esp, 8
        mov    dword ptr [0xc00], eax        ; -> loc_1_0C00
        or     eax, eax
        jne    loc_3_646A
        push   0
        push   0x49d
        call   sub_1_0404
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_6477
loc_3_646A:
        mov    ecx, dword ptr [0xc04]        ; -> loc_1_0C04
        inc    ecx
        mov    dword ptr [0xc04], ecx        ; -> loc_1_0C04
loc_3_6477:
        mov    dword ptr [0xc00], eax        ; -> loc_1_0C00
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_3_6482:
        pop    edi
        pop    esi
        add    esp, 0x10c
        ret    
sub_3_648B:
        mov    eax, dword ptr [0xc04]        ; -> loc_1_0C04
        or     eax, eax
        je     loc_3_64B8
        dec    eax
        mov    dword ptr [0xc04], eax        ; -> loc_1_0C04
        jne    loc_3_64B8
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_64B8
        push   eax
        call   sub_1_0580
        add    esp, 4
        mov    dword ptr [0xc00], 0          ; -> loc_1_0C00
loc_3_64B8:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_64C0:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 8], eax
        push   1
        push   ecx
        call   sub_3_64E3
        ret    0x10
sub_3_64E3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 8]
        call   sub_3_65CD
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x10], 0xffffffff
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x2c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x30], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x28], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x24], 0
        push   edx
        mov    edx, dword ptr [ebp + 8]
        call   sub_3_660F
        pop    edx
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_3_6597
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1c], 0
        mov    eax, dword ptr [ebp + 8]
        push   1
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x28], 0
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 4]
        call   sub_1_0488
        add    esp, 8
        or     eax, eax
        je     loc_3_65C7
loc_3_6597:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   dword ptr [eax + 4]
        call   sub_1_0470
        add    esp, 0xc
        or     eax, eax
        je     loc_3_65C7
        push   dword ptr [ebp + 8]
        push   0x390                         ; -> loc_1_0390
        mov    eax, dword ptr [ebp + 8]
        push   1
        push   dword ptr [eax + 4]
        call   sub_1_04B8
        add    esp, 0x10
loc_3_65C7:
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_65CD:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        push   0
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   -1
        push   dword ptr [edi + 4]
        call   sub_1_04B8
        add    esp, 0x10
        mov    eax, dword ptr [edi + 0x18]
        or     eax, eax
        je     loc_3_6607
        mov    dword ptr [ebp - 4], eax
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp - 4]
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        pop    esi
        popfd  
        mov    dword ptr [edi + 0x18], 0
loc_3_6607:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_660F:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], edx
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x18], 0
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [eax + 0x24], 0x80
        jae    loc_3_67AB
        sub    esi, esi
        mov    edi, 1
        mov    ebx, 0x10
loc_3_6642:
        mov    eax, dword ptr [esp + 0x10]
        push   esi
        push   dword ptr [eax + 8]
        call   sub_3_3349
        lea    eax, [esp + 0x1c]
        add    esp, 8
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   edi
        mov    eax, dword ptr [eax + 0x1c]
        add    eax, dword ptr [esp + 0x18]
        add    eax, 0x34
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   dword ptr [eax + 4]
        call   sub_1_04A8
        add    esp, 0x10
        cmp    eax, esi
        je     loc_3_678A
        cmp    dword ptr [esp + 0x14], esi
        je     loc_3_67AB
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0x28
        cmp    dword ptr [eax], esi
        jne    loc_3_66DF
        mov    edx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [edx + 0x1c]
        add    edx, 0x1c
        mov    ebp, dword ptr [esp + 0x10]
        cmp    byte ptr [ecx + ebp + 0x34], bl
        jne    loc_3_66C2
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax + 0xb8], bl
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0xb4], edi
        jmp    loc_3_674A
loc_3_66C2:
        mov    eax, dword ptr [esp + 0x14]
        add    eax, ecx
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 0x10]
        and    dword ptr [eax + 0x1c], 0x7f
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        add    dword ptr [eax + 0x24], ecx
        jmp    loc_3_674A
loc_3_66DF:
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax + 0x1c]
        mov    al, byte ptr [eax + ecx + 0x34]
        mov    ecx, dword ptr [esp + 0x10]
        inc    dword ptr [ecx + 0xb4]
        mov    ecx, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [ecx + 0xb4]
        cmp    ecx, 2
        jbe    loc_3_6705
        jmp    loc_3_6743
loc_3_6705:
        mov    edx, dword ptr [esp + 0x10]
        mov    byte ptr [ecx + edx + 0xb7], al
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax]
        push   dword ptr [eax + 0xb4]
        add    eax, 0xb8
        push   eax
        push   ecx
        call   dword ptr [ecx]
        add    esp, 0xc
        cmp    eax, esi
        jne    loc_3_672E
        jmp    loc_3_6743
loc_3_672E:
        cmp    eax, 3
        jne    loc_3_6735
        jmp    loc_3_6743
loc_3_6735:
        cmp    eax, 2
        jne    loc_3_6743
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x28], edi
        jmp    loc_3_674A
loc_3_6743:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x28], esi
loc_3_674A:
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x10]
        cmp    eax, -1
        je     loc_3_6777
        mov    ecx, dword ptr [esp + 0x10]
        cmp    dword ptr [ecx + 0x24], eax
        jb     loc_3_6777
        mov    eax, ecx
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [eax + 0x620], edi
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax + 0x14]
        call   dword ptr [eax + 0xc]
        add    esp, 4
loc_3_6777:
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [eax + 0x24], 0x80
        jb     loc_3_6642
        jmp    loc_3_67AB
loc_3_678A:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x30], 1
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax + 4]
        call   sub_1_04B0
        mov    ecx, dword ptr [esp + 0x14]
        add    esp, 4
        mov    dword ptr [ecx + 0x2c], eax
loc_3_67AB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_3_67B3:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, 0
        cmp    dword ptr [ecx + 0x24], 0x80
        je     loc_3_67E4
        mov    eax, dword ptr [ecx + 0x20]
        add    eax, 0x7f
        mov    dword ptr [ecx + 0x20], eax
        and    eax, 0x7f
        mov    dword ptr [ecx + 0x20], eax
        mov    dl, byte ptr [esp + 8]
        mov    byte ptr [eax + ecx + 0x34], dl
        inc    dword ptr [ecx + 0x24]
        mov    eax, 1
loc_3_67E4:
        ret    8
sub_3_67E7:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0x30], 1
        jne    loc_3_67FE
        mov    dword ptr [esi + 0x30], 0
        sub    eax, eax
        jmp    loc_3_6842
loc_3_67FE:
        cmp    dword ptr [esp + 0x14], 0
        jle    loc_3_6833
        mov    edx, dword ptr [esi + 0x24]
        or     edx, edx
        je     loc_3_6833
        mov    eax, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [esi + 0x20]
        dec    edx
        mov    bl, byte ptr [eax + esi + 0x34]
        mov    byte ptr [ecx], bl
        inc    eax
        mov    dword ptr [esi + 0x20], eax
        and    eax, 0x7f
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], edx
        jmp    loc_3_683D
loc_3_6833:
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax], 0
loc_3_683D:
        mov    eax, 1
loc_3_6842:
        pop    esi
        pop    ebx
        ret    0x10
sub_3_6847:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        call   sub_3_660F
        pop    edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x1c], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, 1
        pop    ebp
        ret    4
sub_3_687B:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0xc], edx
        mov    ecx, dword ptr [esp + 0xc]
        cmp    ecx, -1
        mov    dword ptr [eax + 0x10], ecx
        mov    esi, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 0x14], esi
        je     loc_3_68A5
        cmp    dword ptr [eax + 0x24], ecx
        jb     loc_3_68A5
        push   esi
        call   edx
        add    esp, 4
loc_3_68A5:
        mov    eax, 1
        pop    esi
        ret    0x10
        db  000h, 000h
loc_3_68B0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   dword ptr [esi + 0xc]
        call   sub_1_071B
        mov    esi, dword ptr [esi]
        mov    edi, eax
        add    esi, 0x6fa
        mov    ecx, 0x26
        rep movsd dword ptr es:[edi], dword ptr [esi]
        sub    ax, ax
        pop    edi
        pop    esi
        ret    
loc_3_68D5:
        push   edi
        mov    edx, dword ptr [esp + 8]
        sub    eax, eax
        mov    edi, dword ptr [edx]
        add    edi, 0x6fa
        mov    ecx, 0x26
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [edx]
        pop    edi
        mov    dword ptr [eax + 0x6fa], 0x10
        sub    ax, ax
        ret    
loc_3_68FC:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 0x2e0]
        cmp    eax, 0x1a
        je     loc_3_6916
        cmp    eax, 0x1c
        mov    ax, 0
        jne    loc_3_691A
loc_3_6916:
        mov    ax, 2
loc_3_691A:
        ret    
loc_3_691B:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 0x82c]
        ret    
sub_3_6929:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x828], eax
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_6959
        movzx  ecx, word ptr [0x22]          ; -> loc_2_0022
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_6959:
        cmp    dword ptr [esi + 0x2e0], 0xf
        mov    ax, 0
        je     loc_3_696A
        mov    ax, 2
loc_3_696A:
        pop    esi
        ret    
loc_3_696C:
        sub    esp, 0x60
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x70]
        mov    ebx, dword ptr [eax]
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_6998
        movzx  ecx, word ptr [0x24]          ; -> obj2:0x0024
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_6998:
        cmp    dword ptr [ebx + 0x2e0], 0x1a
        jne    loc_3_6A41
        mov    dword ptr [ebx + 0x2e0], 0xf
        mov    dword ptr [ebx + 0x7eb], 0
        push   dword ptr [ebx + 0x828]
        call   sub_1_07F6
        add    esp, 4
        mov    dword ptr [ebx + 0x828], 0
        mov    eax, dword ptr [ebx + 0x7ef]
        or     eax, eax
        jne    loc_3_69E4
        test   byte ptr [ebx + 0x825], 8
        jne    loc_3_69F2
loc_3_69E4:
        cmp    eax, 1
        jne    loc_3_6A21
        test   byte ptr [ebx + 0x825], 4
        je     loc_3_6A21
loc_3_69F2:
        lea    esi, [ebx + 0x7f3]
        lea    edi, [esp + 0x1c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [ebx + 0x7fb], 0
        mov    dword ptr [esp + 0x10], 0xffffffff
        mov    dword ptr [esp + 0xc], ebx
        lea    eax, [esp + 0xc]
        push   0x13
        push   eax
        call   sub_3_7341
        add    esp, 8
loc_3_6A21:
        push   ebx
        call   sub_3_73DA
        add    esp, 4
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_6A3C
        push   eax
        call   sub_1_0590
        add    esp, 4
loc_3_6A3C:
        sub    ax, ax
        jmp    loc_3_6A45
loc_3_6A41:
        mov    ax, 2
loc_3_6A45:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x60
        ret    
loc_3_6A4C:
        sub    esp, 0x60
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x6c]
        mov    esi, dword ptr [edi]
        mov    eax, dword ptr [esi + 0x2e0]
        cmp    eax, 4
        jne    loc_3_6B1F
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_6A86
        movzx  ecx, word ptr [0x22]          ; -> loc_2_0022
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_6A86:
        lea    eax, [esi + 0x7df]
        push   eax
        push   0xc68                         ; -> obj1:0x0C68
        push   esi
        call   sub_3_70CA
        or     eax, eax
        jne    loc_3_6AD2
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [esi + 0x828], eax
        mov    dword ptr [esi + 0x2e0], 0x1a
        mov    dword ptr [esi + 0x7eb], 1
        sub    ax, ax
        mov    dword ptr [esi + 0x7fc], 1
        mov    dword ptr [esi + 0x800], 1
        jmp    loc_3_6B19
loc_3_6AD2:
        mov    dword ptr [esi + 0x2e0], 4
        mov    dword ptr [esp + 8], esi
        mov    eax, dword ptr [esi + 0x7d3]
        mov    dword ptr [esp + 0xc], eax
        mov    eax, dword ptr [esi + 0x7d7]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esi + 0x7db]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esi + 0x5be]
        mov    dword ptr [esp + 0x18], eax
        lea    eax, [esp + 8]
        push   eax
        call   sub_3_0823
        add    esp, 4
loc_3_6B15:
        mov    ax, 2
loc_3_6B19:
        pop    edi
        pop    esi
        add    esp, 0x60
        ret    
loc_3_6B1F:
        cmp    eax, 0x1c
        jne    loc_3_6B15
        push   dword ptr [esi + 0x804]
        call   sub_1_0544
        mov    dword ptr [esi + 0x804], 0
        push   dword ptr [esi + 0x808]
        call   sub_1_0420
        add    esp, 4
        mov    dword ptr [esi + 0x808], 0
        mov    dword ptr [esi + 0x2e0], 0x1a
        mov    dword ptr [esi + 0x7eb], 1
        mov    dword ptr [esi + 0x7fc], 1
        sub    ax, ax
        mov    ecx, dword ptr [edi + 0xc]
        mov    dword ptr [esi + 0x828], ecx
        jmp    loc_3_6B19
sub_3_6B7D:
        sub    esp, 0x60
        push   esi
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_6BA1
        movzx  ecx, word ptr [0x24]          ; -> obj2:0x0024
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_6BA1:
        mov    esi, dword ptr [esp + 0x68]
        lea    eax, [esi + 0x7df]
        push   eax
        push   0xc74                         ; -> loc_1_0C74
        push   esi
        call   sub_3_70CA
        mov    dword ptr [esi + 0x2e0], 4
        mov    dword ptr [esi + 0x7eb], 0
        mov    dword ptr [esi + 0x7fc], 0
        mov    dword ptr [esp + 4], esi
        mov    eax, dword ptr [esi + 0x7d3]
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [esi + 0x7d7]
        mov    dword ptr [esp + 0xc], eax
        mov    eax, dword ptr [esi + 0x7db]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esi + 0x5be]
        mov    dword ptr [esp + 0x14], eax
        lea    eax, [esp + 4]
        push   eax
        call   sub_3_0823
        add    esp, 4
        pop    esi
        add    esp, 0x60
        ret    
loc_3_6C13:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    dword ptr [eax + 0x808], 0
        call   sub_3_6B7D
        add    esp, 4
        ret    
loc_3_6C2B:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x804], 0
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x800], 0
        push   0
        push   1
        push   dword ptr [esp + 8]
        push   0x6c13                        ; -> loc_3_6C13
        call   sub_1_0418
        add    esp, 0x10
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 0x808], eax
        add    esp, 4
        ret    
sub_3_6C71:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    dword ptr [esi + 0x800], 0
        cmp    dword ptr [esi + 0x2e0], 0x1c
        jne    loc_3_6CB1
        push   dword ptr [esi + 0x804]
        call   sub_1_0544
        mov    dword ptr [esi + 0x2e0], 0x1a
        push   esi
        push   1
        push   0x6c2b                        ; -> loc_3_6C2B
        call   sub_1_0528
        mov    dword ptr [esi + 0x804], eax
loc_3_6CB1:
        pop    esi
        ret    
loc_3_6CB3:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax]
        cmp    dword ptr [esi + 0x2e0], 0x1a
        jne    loc_3_6D2F
        cmp    dword ptr [esi + 0x828], 0
        je     loc_3_6CE4
        push   dword ptr [esi + 0x828]
        call   sub_1_07F6
        add    esp, 4
        mov    dword ptr [esi + 0x828], 0
loc_3_6CE4:
        cmp    dword ptr [esi + 0x800], 0
        jne    loc_3_6CF8
        push   esi
        call   sub_3_6B7D
        add    esp, 4
        jmp    loc_3_6D2A
loc_3_6CF8:
        mov    dword ptr [esi + 0x2e0], 0x1c
        mov    dword ptr [esi + 0x7eb], 0
        push   esi
        mov    eax, dword ptr [esi + 0x80c]
        imul   eax, eax, 0x3e8
        push   eax
        push   0x6c2b                        ; -> loc_3_6C2B
        call   sub_1_0528
        mov    dword ptr [esi + 0x804], eax
loc_3_6D2A:
        sub    ax, ax
        jmp    loc_3_6D33
loc_3_6D2F:
        mov    ax, 2
loc_3_6D33:
        pop    esi
        ret    
loc_3_6D35:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, 2
        mov    ebx, dword ptr [esi]
        cmp    dword ptr [ebx + 0x2e0], 0x1a
        jne    loc_3_6E55
        cmp    dword ptr [ebx + 0x7eb], 1
        jne    loc_3_6E55
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_6D7D
        movzx  ecx, word ptr [0x20]          ; -> loc_2_0020
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_6D7D:
        mov    dword ptr [ebx + 0x7ef], 0
        mov    dword ptr [ebx + 0x7eb], 2
        lea    eax, [ebx + 0x7df]
        push   eax
        lea    eax, [esi + 0x10]
        push   eax
        push   ebx
        call   sub_3_70CA
        mov    edi, dword ptr [esi]
        mov    ebx, eax
        mov    dword ptr [edi + 0x2e0], 0x1a
        cmp    ebx, 2
        jne    loc_3_6DE5
        mov    dword ptr [edi + 0x7eb], 6
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_6DE0
        movzx  eax, word ptr [0xba]          ; -> loc_2_00BA
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_6DE0:
        mov    ax, bx
        jmp    loc_3_6E58
loc_3_6DE5:
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_6E06
        movzx  eax, word ptr [0x1c]          ; -> loc_2_001C
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_6E06:
        lea    eax, [edi + 0x7df]
        push   eax
        push   0xc08                         ; -> loc_1_0C08
        push   edi
        call   sub_3_70CA
        mov    edi, eax
        mov    eax, dword ptr [esi]
        cmp    edi, 2
        mov    dword ptr [eax + 0x2e0], 0x1a
        jne    loc_3_6E55
        mov    dword ptr [eax + 0x7eb], 6
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_6E55
        movzx  ecx, word ptr [0xb6]          ; -> loc_2_00B6
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_6E55:
        mov    ax, di
loc_3_6E58:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_6E5C:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, 2
        mov    ebx, dword ptr [edi]
        cmp    dword ptr [ebx + 0x2e0], 0x1a
        jne    loc_3_6EF8
        cmp    dword ptr [ebx + 0x7eb], esi
        jne    loc_3_6EF8
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_6E9F
        movzx  ecx, word ptr [0x1e]          ; -> loc_2_001E
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_6E9F:
        lea    eax, [ebx + 0x7df]
        push   eax
        push   0xc14                         ; -> loc_1_0C14
        push   ebx
        call   sub_3_70CA
        mov    esi, eax
        mov    eax, dword ptr [edi]
        cmp    esi, 2
        mov    dword ptr [eax + 0x2e0], 0x1a
        mov    dword ptr [eax + 0x7eb], 1
        jne    loc_3_6EF8
        mov    dword ptr [eax + 0x7eb], 6
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_6EF8
        movzx  ecx, word ptr [0xb8]          ; -> loc_2_00B8
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_6EF8:
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_6EFF:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, 2
        mov    ebx, dword ptr [esi]
        cmp    dword ptr [ebx + 0x2e0], 0x1a
        jne    loc_3_701F
        cmp    dword ptr [ebx + 0x7eb], 1
        jne    loc_3_701F
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_6F47
        movzx  ecx, word ptr [0x20]          ; -> loc_2_0020
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_6F47:
        mov    dword ptr [ebx + 0x7ef], 0
        mov    dword ptr [ebx + 0x7eb], 3
        lea    eax, [ebx + 0x7df]
        push   eax
        lea    eax, [esi + 0x10]
        push   eax
        push   ebx
        call   sub_3_70CA
        mov    edi, dword ptr [esi]
        mov    ebx, eax
        mov    dword ptr [edi + 0x2e0], 0x1a
        cmp    ebx, 2
        jne    loc_3_6FAF
        mov    dword ptr [edi + 0x7eb], 6
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_6FAA
        movzx  eax, word ptr [0xba]          ; -> loc_2_00BA
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_6FAA:
        mov    ax, bx
        jmp    loc_3_7022
loc_3_6FAF:
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_6FD0
        movzx  eax, word ptr [0x1c]          ; -> loc_2_001C
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_6FD0:
        lea    eax, [edi + 0x7df]
        push   eax
        push   0xc20                         ; -> loc_1_0C20
        push   edi
        call   sub_3_70CA
        mov    edi, eax
        mov    eax, dword ptr [esi]
        cmp    edi, 2
        mov    dword ptr [eax + 0x2e0], 0x1a
        jne    loc_3_701F
        mov    dword ptr [eax + 0x7eb], 6
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_701F
        movzx  ecx, word ptr [0xb6]          ; -> loc_2_00B6
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_701F:
        mov    ax, di
loc_3_7022:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_7026:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, 2
        mov    ebx, dword ptr [edi]
        cmp    dword ptr [ebx + 0x2e0], 0x1a
        jne    loc_3_70C3
        cmp    dword ptr [ebx + 0x7eb], 3
        jne    loc_3_70C3
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_706A
        movzx  ecx, word ptr [0x1e]          ; -> loc_2_001E
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_706A:
        lea    eax, [ebx + 0x7df]
        push   eax
        push   0xc2c                         ; -> loc_1_0C2C
        push   ebx
        call   sub_3_70CA
        mov    esi, eax
        mov    eax, dword ptr [edi]
        cmp    esi, 2
        mov    dword ptr [eax + 0x2e0], 0x1a
        mov    dword ptr [eax + 0x7eb], 1
        jne    loc_3_70C3
        mov    dword ptr [eax + 0x7eb], 6
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_70C3
        movzx  ecx, word ptr [0xb8]          ; -> loc_2_00B8
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_70C3:
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_70CA:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        mov    ebx, 2
        mov    esi, dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x1c]
        push   esi
        call   sub_3_31F4
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        je     loc_3_711F
        mov    dword ptr [esi + 0x2e0], 0x14
        mov    edi, dword ptr [esp + 0x1c]
        push   edi
        call   sub_1_075F
        push   ebp
        push   0
        push   0
        push   -1
        push   esi
        call   sub_3_11F4
        add    esp, 0x14
        mov    ebx, eax
        cmp    ebx, 1
        jne    loc_3_711F
        push   edi
        call   sub_1_078D
        mov    ebx, eax
loc_3_711F:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0xc
loc_3_7128:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    si, 2
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ecx + 0x2e0]
        cmp    eax, 3
        je     loc_3_714A
        cmp    eax, 4
        je     loc_3_714A
        cmp    eax, 8
        jne    loc_3_71AF
loc_3_714A:
        push   0
        push   ecx
        call   sub_3_3E17
        add    esp, 8
        push   0
        push   0xc38                         ; -> loc_1_0C38
        push   dword ptr [edi]
        call   sub_3_31F4
        add    esp, 0xc
        mov    ebx, eax
        or     ebx, ebx
        je     loc_3_71AF
        mov    esi, dword ptr [edi]
        mov    dword ptr [esi + 0x2e0], 0x11
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_7199
        movzx  eax, word ptr [0x18]          ; -> loc_2_0018
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_7199:
        push   ebx
        push   dword ptr [edi + 0xc]
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        push   esi
        call   sub_3_11F4
        add    esp, 0x14
        mov    si, ax
loc_3_71AF:
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        cmp    si, 2
        jne    loc_3_71D6
        or     ecx, ecx
        je     loc_3_71D6
        movzx  eax, word ptr [0xb2]          ; -> loc_2_00B2
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_71D6:
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_71DD:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, 2
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [ecx + 0x2e0]
        cmp    eax, 3
        je     loc_3_7200
        cmp    eax, 4
        je     loc_3_7200
        cmp    eax, 8
        jne    loc_3_7265
loc_3_7200:
        push   0
        push   ecx
        call   sub_3_3E17
        add    esp, 8
        push   0
        push   0xc50                         ; -> obj1:0x0C50
        push   dword ptr [esi]
        call   sub_3_31F4
        add    esp, 0xc
        mov    ebx, eax
        or     ebx, ebx
        je     loc_3_7265
        mov    edi, dword ptr [esi]
        mov    dword ptr [edi + 0x2e0], 0xe
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     ecx, ecx
        je     loc_3_724F
        movzx  eax, word ptr [0x16]          ; -> loc_2_0016
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_724F:
        push   ebx
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   edi
        call   sub_3_11F4
        add    esp, 0x14
        movzx  edi, ax
loc_3_7265:
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        cmp    edi, 2
        jne    loc_3_728B
        or     ecx, ecx
        je     loc_3_728B
        movzx  eax, word ptr [0xb0]          ; -> loc_2_00B0
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_728B:
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_7292:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    di, 2
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 0x2e0], 0xf
        jne    loc_3_7339
        lea    ebx, [esi + 0x10]
        push   0
        push   ebx
        push   eax
        call   sub_3_31F4
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
        je     loc_3_730B
        or     ecx, ecx
        je     loc_3_72E3
        movzx  eax, word ptr [0x1a]          ; -> loc_2_001A
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 8
loc_3_72E3:
        mov    eax, dword ptr [esi]
        push   ebp
        mov    dword ptr [eax + 0x2e0], 0x17
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        call   sub_3_11F4
        add    esp, 0x14
        mov    di, ax
        mov    ecx, dword ptr [0xc00]        ; -> loc_1_0C00
loc_3_730B:
        cmp    di, 2
        jne    loc_3_7339
        mov    eax, dword ptr [esi]
        or     ecx, ecx
        mov    dword ptr [eax + 0x2e0], 0xf
        je     loc_3_7339
        push   ebx
        movzx  eax, word ptr [0xb4]          ; -> loc_2_00B4
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   ecx
        call   sub_1_0588
        add    esp, 0xc
loc_3_7339:
        mov    ax, di
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_7341:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esi]
        mov    eax, dword ptr [ebx + 0x2e0]
        cmp    eax, 0xf
        je     loc_3_7366
        cmp    eax, 0x1a
        je     loc_3_736B
        cmp    eax, 0x17
        je     loc_3_7366
        mov    eax, 2
        jmp    loc_3_73D6
loc_3_7366:
        cmp    eax, 0x1a
        jne    loc_3_737B
loc_3_736B:
        mov    edi, dword ptr [esp + 0x14]
        cmp    edi, 0x11
        jne    loc_3_737F
        mov    eax, 2
        jmp    loc_3_73D6
loc_3_737B:
        mov    edi, dword ptr [esp + 0x14]
loc_3_737F:
        push   1
        push   0x68
        call   sub_1_04C8
        add    esp, 8
        or     eax, eax
        jne    loc_3_7396
        mov    eax, 2
        jmp    loc_3_73D6
loc_3_7396:
        mov    dword ptr [eax + 4], edi
        lea    edi, [eax + 8]
        mov    ecx, 0x18
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebx + 0x838], 0
        jne    loc_3_73BA
        mov    dword ptr [ebx + 0x838], eax
        jmp    loc_3_73C2
loc_3_73BA:
        mov    ecx, dword ptr [ebx + 0x83c]
        mov    dword ptr [ecx], eax
loc_3_73C2:
        mov    dword ptr [ebx + 0x83c], eax
        push   ebx
        call   sub_3_73DA
        add    esp, 4
        mov    eax, 1
loc_3_73D6:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_73DA:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 0x2e0], 0xf
        jne    loc_3_7473
        cmp    dword ptr [esi + 0x834], 0
        jne    loc_3_7473
        mov    ecx, dword ptr [esi + 0x838]
        or     ecx, ecx
        je     loc_3_7473
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 0x838], eax
        or     ecx, ecx
        je     loc_3_7473
        mov    dword ptr [esi + 0x834], ecx
        mov    eax, dword ptr [ecx + 4]
        cmp    eax, 4
        je     loc_3_742F
        cmp    eax, 0x11
        je     loc_3_743D
        cmp    eax, 0x13
        je     loc_3_7448
        cmp    eax, 0x14
        je     loc_3_7453
        mov    eax, 2
        jmp    loc_3_7462
loc_3_742F:
        add    ecx, 8
        push   ecx
        call   sub_3_0C35
        add    esp, 4
        jmp    loc_3_7462
loc_3_743D:
        add    ecx, 8
        push   ecx
        call   sub_3_6929
        jmp    loc_3_745C
loc_3_7448:
        add    ecx, 8
        push   ecx
        call   sub_3_765E
        jmp    loc_3_745C
loc_3_7453:
        add    ecx, 8
        push   ecx
        call   sub_3_793F
loc_3_745C:
        add    esp, 4
        movzx  eax, ax
loc_3_7462:
        cmp    eax, 1
        je     loc_3_7473
        push   1
        push   eax
        push   esi
        call   sub_3_7475
        add    esp, 0xc
loc_3_7473:
        pop    esi
        ret    
sub_3_7475:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esi + 0x834]
        or     edi, edi
        je     loc_3_75BE
        mov    dword ptr [esi + 0x834], 0
        mov    eax, dword ptr [edi + 4]
        cmp    eax, 0x11
        je     loc_3_74B2
        cmp    eax, 0x13
        jb     loc_3_7591
        cmp    eax, 0x14
        jbe    loc_3_7548
        jmp    loc_3_7591
loc_3_74B2:
        mov    ecx, dword ptr [esp + 0x10]
        or     ecx, ecx
        jne    loc_3_74E7
        mov    dword ptr [esi + 0x2e0], 0x1a
        mov    dword ptr [esi + 0x7eb], 1
        mov    dword ptr [esi + 0x82c], ecx
        push   dword ptr [esi + 0x828]
        call   sub_1_07E4
        add    esp, 4
        jmp    loc_3_7591
loc_3_74E7:
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [esi + 0x828], eax
        mov    dword ptr [esi + 0x82c], ecx
        push   dword ptr [esi + 0x828]
        call   sub_1_07E4
        add    esp, 4
        push   dword ptr [esi + 0x828]
        call   sub_1_07F6
        add    esp, 4
        mov    dword ptr [esi + 0x828], 0
        mov    dword ptr [esi + 0x7eb], 6
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_7591
        movzx  ecx, word ptr [0xc0]          ; -> loc_2_00C0
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
        jmp    loc_3_7591
loc_3_7548:
        mov    dword ptr [esi + 0x2e0], 0xf
        mov    eax, dword ptr [esi + 0x5e2]
        or     eax, eax
        je     loc_3_7579
        cmp    dword ptr [esi + 0x5de], eax
        je     loc_3_756F
        push   0
        push   eax
        call   sub_1_04D0
        add    esp, 8
loc_3_756F:
        mov    dword ptr [esi + 0x5e2], 0
loc_3_7579:
        mov    eax, dword ptr [edi + 0xc]
        cmp    eax, -1
        je     loc_3_7591
        push   dword ptr [edi + 0x14]
        push   dword ptr [esp + 0x14]
        push   dword ptr [edi + 0x10]
        push   eax
        call   sub_1_06F0
loc_3_7591:
        push   0
        push   edi
        call   sub_1_04D0
        add    esp, 8
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_75AC
        push   esi
        call   sub_3_73DA
        add    esp, 4
loc_3_75AC:
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_75BE
        push   eax
        call   sub_1_0590
        add    esp, 4
loc_3_75BE:
        pop    edi
        pop    esi
        ret    
sub_3_75C1:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0x834], 0
        jne    loc_3_75F1
        mov    eax, dword ptr [esi + 0x838]
        or     eax, eax
        je     loc_3_75E2
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0x838], ecx
loc_3_75E2:
        mov    dword ptr [esi + 0x834], eax
        cmp    dword ptr [esi + 0x834], 0
        je     loc_3_761D
loc_3_75F1:
        mov    edi, dword ptr [esp + 0x10]
loc_3_75F5:
        push   0
        push   edi
        push   esi
        call   sub_3_7475
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x838]
        or     eax, eax
        je     loc_3_7613
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0x838], ecx
loc_3_7613:
        mov    dword ptr [esi + 0x834], eax
        or     eax, eax
        jne    loc_3_75F5
loc_3_761D:
        pop    edi
        pop    esi
        ret    
sub_3_7620:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x10]
        mov    esi, ecx
        mov    edi, dword ptr [esp + 0x14]
        sub    esi, edi
        mov    eax, esi
        cdq    
        xor    eax, edx
        sub    eax, edx
        lea    esi, [eax + 1]
        mov    eax, 0x10000
        cdq    
        idiv   esi
        cmp    edi, ecx
        lea    ecx, [eax + 1]
        mov    eax, dword ptr [esp + 0xc]
        ja     loc_3_7653
        sub    edx, edx
        div    ecx
        add    eax, edi
        jmp    loc_3_765B
loc_3_7653:
        sub    edx, edx
        div    ecx
        sub    edi, eax
        mov    eax, edi
loc_3_765B:
        pop    edi
        pop    esi
        ret    
sub_3_765E:
        sub    esp, 0x410
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x424]
        mov    dword ptr [esp + 0x10], 0
        mov    ebp, dword ptr [eax]
        add    eax, 0x10
        mov    dword ptr [esp + 0x18], eax
        cmp    dword ptr [ebp + 0x2e0], 0xf
        jne    loc_3_7921
        movzx  ecx, byte ptr [ebp + 0x7fb]
        imul   ecx, ecx, 3
        movzx  eax, byte ptr [eax + 8]
        mov    bl, byte ptr [ecx + eax + 0xc5c] ; -> loc_1_0C5C
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_76C2
        movzx  ecx, word ptr [0x26]          ; -> loc_2_0026
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_76C2:
        cmp    bl, 1
        jne    loc_3_76E6
        mov    eax, dword ptr [esp + 0x18]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [ebp + 0x7f3], eax
        jne    loc_3_76E6
        mov    eax, dword ptr [esp + 0x18]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [ebp + 0x7f7], eax
        jne    loc_3_76E6
        sub    bl, bl
loc_3_76E6:
        lea    edi, [ebp + 0x7f3]
        mov    esi, dword ptr [esp + 0x18]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        or     bl, bl
        jne    loc_3_76FF
        sub    ax, ax
        jmp    loc_3_7934
loc_3_76FF:
        test   bl, 8
        je     loc_3_7725
        push   0
        push   0xc84                         ; -> obj1:0x0C84
        push   ebp
        call   sub_3_31F4
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_7725
        mov    ax, 2
        jmp    loc_3_7934
loc_3_7725:
        test   bl, 0x10
        je     loc_3_774D
        push   dword ptr [esp + 0x10]
        push   0xc98                         ; -> obj1:0x0C98
        push   ebp
        call   sub_3_31F4
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_774D
        mov    ax, 2
        jmp    loc_3_7934
loc_3_774D:
        test   bl, 2
        je     loc_3_7775
        push   dword ptr [esp + 0x10]
        push   0xcac                         ; -> loc_1_0CAC
        push   ebp
        call   sub_3_31F4
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_7775
        mov    ax, 2
        jmp    loc_3_7934
loc_3_7775:
        test   bl, 4
        je     loc_3_779D
        push   dword ptr [esp + 0x10]
        push   0xcc0                         ; -> loc_1_0CC0
        push   ebp
        call   sub_3_31F4
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_779D
        mov    ax, 2
        jmp    loc_3_7934
loc_3_779D:
        test   bl, 1
        je     loc_3_77C5
        push   dword ptr [esp + 0x10]
        push   0xcd4                         ; -> loc_1_0CD4
        push   ebp
        call   sub_3_31F4
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_77C5
        mov    ax, 2
        jmp    loc_3_7934
loc_3_77C5:
        push   1
        push   0x19a
        call   sub_1_04C8
        mov    dword ptr [esp + 0x24], eax
        add    esp, 8
        or     eax, eax
        jne    loc_3_77F3
        push   0
        push   dword ptr [esp + 0x14]
        call   sub_1_04D0
        add    esp, 8
        mov    ax, 2
        jmp    loc_3_7934
loc_3_77F3:
        mov    esi, 0xcf0                    ; -> obj1:0x0CF0
        lea    edi, [esp + 0x20]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    eax, [esp + 0x120]
        push   eax
        push   dword ptr [ebp + 0x818]
        mov    eax, dword ptr [esp + 0x20]
        push   dword ptr [ebp + 0x814]
        push   dword ptr [eax]
        call   sub_3_7620
        add    esp, 0xc
        push   eax
        call   sub_3_30F5
        add    esp, 8
        mov    esi, 0xcf8                    ; -> loc_1_0CF8
        lea    edi, [esp + 0x220]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        lea    eax, [esp + 0x320]
        push   eax
        push   dword ptr [ebp + 0x820]
        mov    eax, dword ptr [esp + 0x20]
        push   dword ptr [ebp + 0x81c]
        push   dword ptr [eax + 4]
        call   sub_3_7620
        add    esp, 0xc
        push   eax
        call   sub_3_30F5
        mov    esi, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [esp + 0x24]
        add    esp, 8
        mov    eax, esi
        cmp    byte ptr [eax], 0
        je     loc_3_78A6
loc_3_7875:
        lea    eax, [esp + 0x20]
        push   2
        push   eax
        mov    edi, esi
        lea    eax, [esp + 0x1c]
        push   eax
        push   ebx
        push   esi
        call   sub_3_2F1C
        add    esp, 0x14
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    esi, ecx
        mov    eax, dword ptr [esp + 0x14]
        inc    eax
        add    ebx, eax
        cmp    byte ptr [esi], 0
        jne    loc_3_7875
loc_3_78A6:
        push   0
        mov    byte ptr [ebx], 0
        push   dword ptr [esp + 0x14]
        call   sub_1_04D0
        mov    eax, dword ptr [esp + 0x42c]
        add    esp, 8
        mov    edi, dword ptr [eax]
        add    edi, 0x2e0
        push   dword ptr [esp + 0x1c]
        mov    dword ptr [edi], 0x17
        push   dword ptr [eax + 0xc]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   ebp
        call   sub_3_11F4
        add    esp, 0x14
        mov    si, ax
        cmp    si, 2
        jne    loc_3_7931
        push   0
        mov    dword ptr [edi], 0xf
        push   dword ptr [esp + 0x20]
        call   sub_1_04D0
        add    esp, 8
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_7931
        movzx  ecx, word ptr [0xbc]          ; -> loc_2_00BC
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
        jmp    loc_3_7931
loc_3_7921:
        lea    edi, [ebp + 0x7f3]
        mov    esi, dword ptr [esp + 0x18]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        sub    si, si
loc_3_7931:
        mov    ax, si
loc_3_7934:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x410
        ret    
sub_3_793F:
        sub    esp, 0x210
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x224]
        mov    si, 2
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 0x1c], eax
        cmp    dword ptr [eax + 0x2e0], 0xf
        jne    loc_3_7AEC
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_7987
        movzx  ecx, word ptr [0x28]          ; -> loc_2_0028
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_7987:
        push   0
        push   0xd00                         ; -> obj1:0x0D00
        push   dword ptr [esp + 0x24]
        call   sub_3_31F4
        mov    dword ptr [esp + 0x20], eax
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_79CB
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_79C2
        movzx  ecx, word ptr [0xbe]          ; -> loc_2_00BE
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_79C2:
        mov    ax, 2
        jmp    loc_3_7AEF
loc_3_79CB:
        push   1
        push   0x19a
        call   sub_1_04C8
        mov    dword ptr [esp + 0x20], eax
        add    esp, 8
        or     eax, eax
        jne    loc_3_7A19
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_7A02
        movzx  ecx, word ptr [0xbe]          ; -> loc_2_00BE
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_7A02:
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_1_04D0
        add    esp, 8
        mov    ax, 2
        jmp    loc_3_7AEF
loc_3_7A19:
        mov    esi, 0xd10                    ; -> loc_1_0D10
        lea    edi, [esp + 0x20]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    al, byte ptr [ebx + 0x10]
        mov    byte ptr [esp + 0x120], al
        mov    byte ptr [esp + 0x121], 0
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, esi
        mov    ebp, dword ptr [esp + 0x18]
        cmp    byte ptr [eax], 0
        je     loc_3_7A76
loc_3_7A45:
        lea    eax, [esp + 0x20]
        push   1
        push   eax
        mov    edi, esi
        lea    eax, [esp + 0x18]
        push   eax
        push   ebp
        push   esi
        call   sub_3_2F1C
        add    esp, 0x14
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    esi, ecx
        mov    eax, dword ptr [esp + 0x10]
        inc    eax
        add    ebp, eax
        cmp    byte ptr [esi], 0
        jne    loc_3_7A45
loc_3_7A76:
        push   0
        mov    byte ptr [ebp], 0
        push   dword ptr [esp + 0x18]
        call   sub_1_04D0
        add    esp, 8
        mov    edi, dword ptr [ebx]
        add    edi, 0x2e0
        push   dword ptr [esp + 0x18]
        mov    dword ptr [edi], 0x17
        push   dword ptr [ebx + 0xc]
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        push   dword ptr [esp + 0x2c]
        call   sub_3_11F4
        add    esp, 0x14
        mov    si, ax
        cmp    si, 2
        jne    loc_3_7AEC
        push   0
        mov    dword ptr [edi], 0xf
        push   dword ptr [esp + 0x1c]
        call   sub_1_04D0
        add    esp, 8
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_7AEC
        movzx  ecx, word ptr [0xbe]          ; -> loc_2_00BE
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_7AEC:
        mov    ax, si
loc_3_7AEF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x210
        ret    
sub_3_7AFA:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        sub    ebx, ebx
        mov    edi, dword ptr [eax + 0x5fa]
        sub    ebp, ebp
        cmp    edi, ebx
        je     loc_3_7B46
        mov    esi, dword ptr [esp + 0x1c]
loc_3_7B14:
        push   esi
        lea    eax, [edi + 0xf]
        push   eax
        push   dword ptr [esp + 0x20]
        call   sub_3_3146
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_7B40
        movzx  eax, byte ptr [edi + 0xe]
        sub    eax, esi
        cmp    eax, -1
        jne    loc_3_7B3B
        mov    ebx, 1
        jmp    loc_3_7B40
loc_3_7B3B:
        mov    ebp, 1
loc_3_7B40:
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_3_7B14
loc_3_7B46:
        lea    eax, [ebp + ebx*2]
        or     eax, eax
        je     loc_3_7B64
        cmp    eax, 1
        je     loc_3_7B6B
        cmp    eax, 2
        je     loc_3_7B72
        cmp    eax, 3
        je     loc_3_7B76
        mov    eax, 3
        jmp    loc_3_7B7B
loc_3_7B64:
        mov    eax, 3
        jmp    loc_3_7B7B
loc_3_7B6B:
        mov    eax, 2
        jmp    loc_3_7B7B
loc_3_7B72:
        sub    eax, eax
        jmp    loc_3_7B7B
loc_3_7B76:
        mov    eax, 4
loc_3_7B7B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_7B80:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    ebx, 2
        cmp    dword ptr [esi + 0x2e0], 0x1a
        jne    loc_3_7C1D
        cmp    dword ptr [esi + 0x7eb], 1
        jne    loc_3_7C1D
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_7BC2
        movzx  ecx, word ptr [0x20]          ; -> loc_2_0020
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_7BC2:
        lea    eax, [esi + 0x7df]
        mov    edi, dword ptr [esp + 0x14]
        push   eax
        push   dword ptr [edi + 0x10]
        push   esi
        call   sub_3_70CA
        mov    ebx, eax
        mov    dword ptr [esi + 0x2e0], 0x1a
        mov    dword ptr [esi + 0x7eb], 9
        cmp    ebx, 2
        jne    loc_3_7C21
        mov    dword ptr [esi + 0x7eb], 6
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_7C21
        movzx  ecx, word ptr [0xba]          ; -> loc_2_00BA
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
        jmp    loc_3_7C21
loc_3_7C1D:
        mov    edi, dword ptr [esp + 0x14]
loc_3_7C21:
        mov    dword ptr [edi + 4], ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_7C28:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    ebx, 2
        cmp    dword ptr [esi + 0x2e0], 0x1a
        jne    loc_3_7D09
        cmp    dword ptr [esi + 0x7eb], 9
        jne    loc_3_7D09
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_7C6F
        movzx  ecx, word ptr [0x1c]          ; -> loc_2_001C
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_7C6F:
        mov    edi, dword ptr [esp + 0x18]
        lea    eax, [esi + 0x7df]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [esi + 0x7ef], ecx
        push   eax
        mov    eax, 0xc20                    ; -> loc_1_0C20
        or     ecx, ecx
        jne    loc_3_7C91
        mov    eax, 0xc08                    ; -> loc_1_0C08
loc_3_7C91:
        push   eax
        push   esi
        call   sub_3_70CA
        mov    ebx, eax
        mov    dword ptr [esi + 0x2e0], 0x1a
        cmp    ebx, 2
        jne    loc_3_7CD5
        mov    dword ptr [esi + 0x7eb], 6
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_7D0D
        movzx  ecx, word ptr [0xb8]          ; -> loc_2_00B8
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
        jmp    loc_3_7D0D
loc_3_7CD5:
        mov    dword ptr [esi + 0x7eb], 4
        mov    ebp, dword ptr [esi + 0x29c]
        push   esi
        push   0x172                         ; -> sub_1_0172
        push   -1
        push   ebp
        call   sub_1_04B8
        add    esp, 0x10
        push   esi
        push   0x172                         ; -> sub_1_0172
        push   -1
        push   ebp
        call   sub_1_04C0
        add    esp, 0x10
        jmp    loc_3_7D0D
loc_3_7D09:
        mov    edi, dword ptr [esp + 0x18]
loc_3_7D0D:
        mov    dword ptr [edi + 4], ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_7D15:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, 2
        cmp    dword ptr [esi + 0x2e0], 0x1a
        jne    loc_3_7DFD
        mov    eax, dword ptr [esi + 0x7eb]
        cmp    eax, 4
        je     loc_3_7D42
        cmp    eax, 5
        jne    loc_3_7DFD
loc_3_7D42:
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_7D62
        movzx  ecx, word ptr [0x1e]          ; -> loc_2_001E
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
loc_3_7D62:
        push   0
        push   0
        push   -1
        push   dword ptr [esi + 0x29c]
        call   sub_1_04C0
        add    esp, 0x10
        push   0
        push   dword ptr [esi + 0x29c]
        call   sub_1_0488
        add    esp, 8
        push   1
        lea    eax, [esi + 0x640]
        push   eax
        call   sub_3_64E3
        lea    eax, [esi + 0x7df]
        push   eax
        mov    eax, 0xc2c                    ; -> loc_1_0C2C
        mov    ebx, dword ptr [esp + 0x18]
        cmp    dword ptr [ebx + 8], 0
        jne    loc_3_7DAF
        mov    eax, 0xc14                    ; -> loc_1_0C14
loc_3_7DAF:
        push   eax
        push   esi
        call   sub_3_70CA
        mov    edi, eax
        mov    dword ptr [esi + 0x2e0], 0x1a
        mov    dword ptr [esi + 0x7eb], 1
        cmp    edi, 2
        jne    loc_3_7E01
        mov    dword ptr [esi + 0x7eb], 6
        mov    eax, dword ptr [0xc00]        ; -> loc_1_0C00
        or     eax, eax
        je     loc_3_7E01
        movzx  ecx, word ptr [0xb8]          ; -> loc_2_00B8
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        push   ecx
        push   eax
        call   sub_1_0588
        add    esp, 8
        jmp    loc_3_7E01
loc_3_7DFD:
        mov    ebx, dword ptr [esp + 0x14]
loc_3_7E01:
        mov    dword ptr [ebx + 4], edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_7E08:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0x40000000
        cmp    eax, 5
        ja     loc_3_7E1F
        jmp    loc_3_7EB5
loc_3_7E1F:
        sub    eax, eax
        jmp    loc_3_7EB2
loc_3_7E26:
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esp + 0x14]
        add    esi, 0x7a2
        push   dword ptr [edi + 4]
        lea    eax, [edi + 0xc]
        push   eax
        push   esi
        call   dword ptr [esi]
        add    esp, 0xc
        mov    dword ptr [edi + 8], eax
        jmp    loc_3_7EAD
loc_3_7E46:
        mov    edi, dword ptr [esp + 0x14]
        push   dword ptr [edi + 4]
        lea    eax, [edi + 0xc]
        push   eax
        push   dword ptr [esp + 0x14]
        call   sub_3_7AFA
        add    esp, 0xc
        mov    dword ptr [edi + 8], eax
        jmp    loc_3_7EAD
loc_3_7E62:
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x10]
        call   sub_3_7B80
        add    esp, 8
        jmp    loc_3_7EAD
loc_3_7E74:
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x10]
        call   sub_3_7C28
        add    esp, 8
        jmp    loc_3_7EAD
loc_3_7E86:
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x10]
        call   sub_3_7D15
        add    esp, 8
        jmp    loc_3_7EAD
loc_3_7E98:
        mov    esi, dword ptr [esp + 0xc]
        add    esi, 0x84d
        mov    edi, dword ptr [esp + 0x14]
        mov    ecx, 0x13
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_3_7EAD:
        mov    eax, 1
loc_3_7EB2:
        pop    edi
        pop    esi
        ret    
loc_3_7EB5:
        jle    loc_3_7EB8
loc_3_7EB8:
        add    byte ptr [esi + 0x7e], al     ; -> loc_3_7E46
        add    byte ptr [eax], al
        bound  edi, qword ptr [esi]
        add    byte ptr [esi + edi*2], dh    ; -> loc_3_7E74
        add    byte ptr [esi - 0x67ffff82], al ; -> loc_3_7E86
        jle    loc_3_7ECC
loc_3_7ECC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_7ED0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 0xc], eax
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0x10], ecx
        add    esi, 0x18
        mov    edx, dword ptr [esp + 0x1c]
        mov    dword ptr [esi - 4], edx
        mov    dword ptr [esi - 0x18], 0x7f0e ; -> loc_3_7F0E
        push   ecx
        push   eax
        push   esi
        call   sub_3_7FC1
        pop    esi
        ret    0x18
loc_3_7F0E:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [esp + 0x18], 2
        jne    loc_3_7F6E
        mov    eax, dword ptr [esp + 0x14]
        mov    bl, byte ptr [eax + 1]
        cmp    bl, 0x80
        jae    loc_3_7F6E
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 4]
        or     edi, edi
        je     loc_3_7F6E
        mov    edx, dword ptr [0xc00]        ; -> loc_1_0C00
        or     edx, edx
        je     loc_3_7F59
        movzx  eax, bl
        movzx  ecx, byte ptr [edi + eax]
        push   ecx
        push   eax
        movzx  eax, word ptr [0x2a]          ; -> loc_2_002A
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   edx
        call   sub_1_0588
        add    esp, 0x10
loc_3_7F59:
        movzx  eax, bl
        mov    al, byte ptr [edi + eax]
        cmp    al, 0xff
        je     loc_3_7F6E
        push   eax
        push   esi
        call   sub_3_7F77
        sub    eax, eax
        jmp    loc_3_7F73
loc_3_7F6E:
        mov    eax, 3
loc_3_7F73:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_7F77:
        push   ebx
        push   esi
        mov    bl, byte ptr [esp + 0x10]
        cmp    bl, 0x11
        ja     loc_3_7F92
        mov    esi, dword ptr [esp + 0xc]
        push   ebx
        add    esi, 0x18
        push   esi
        call   sub_3_7FFF
        jmp    loc_3_7FBC
loc_3_7F92:
        cmp    bl, 0x22
        mov    esi, dword ptr [esp + 0xc]
        jne    loc_3_7FA6
        push   dword ptr [esi + 0x14]
        call   sub_3_6C71
        add    esp, 4
loc_3_7FA6:
        push   dword ptr [esi + 0x10]
        push   0
        movzx  eax, bl
        add    eax, 0x510
        push   eax
        push   dword ptr [esi + 0xc]
        call   sub_1_06F0
loc_3_7FBC:
        pop    esi
        pop    ebx
        ret    8
sub_3_7FC1:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 5], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ecx + 9], eax
        ret    0xc
sub_3_7FDC:
        mov    al, byte ptr [esp + 4]
        cmp    al, 9
        ja     loc_3_7FE8
        add    al, 0x30
        jmp    loc_3_7FFC
loc_3_7FE8:
        cmp    al, 0xa
        jb     loc_3_7FF4
        cmp    al, 0xd
        ja     loc_3_7FF4
        add    al, 0x37
        jmp    loc_3_7FFC
loc_3_7FF4:
        cmp    al, 0xe
        mov    al, 0x2a
        je     loc_3_7FFC
        mov    al, 0x23
loc_3_7FFC:
        ret    4
sub_3_7FFF:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_3_8019
        cmp    eax, 1
        je     loc_3_804B
        cmp    eax, 2
        je     loc_3_8078
        jmp    loc_3_80BF
loc_3_8019:
        mov    al, byte ptr [esp + 0xc]
        cmp    al, 0x10
        jne    loc_3_802C
        mov    dword ptr [esi], 1
        jmp    loc_3_80BF
loc_3_802C:
        cmp    al, 0xf
        ja     loc_3_80BF
        mov    byte ptr [esi + 4], al
        push   dword ptr [esi + 9]
        push   eax
        call   sub_3_7FDC
        movzx  eax, al
        push   eax
        push   0x505
        jmp    loc_3_80B7
loc_3_804B:
        mov    al, byte ptr [esp + 0xc]
        cmp    al, 0xf
        ja     loc_3_8070
        mov    byte ptr [esi + 4], al
        mov    dword ptr [esi], 2
        push   dword ptr [esi + 9]
        push   eax
        call   sub_3_7FDC
        movzx  eax, al
        push   eax
        push   0x503
        jmp    loc_3_80B7
loc_3_8070:
        mov    dword ptr [esi], 0
        jmp    loc_3_80BF
loc_3_8078:
        mov    cl, byte ptr [esi + 4]
        mov    al, byte ptr [esp + 0xc]
        cmp    al, cl
        jne    loc_3_808B
        mov    dword ptr [esi], 2
        jmp    loc_3_80BF
loc_3_808B:
        cmp    al, 0x11
        je     loc_3_8097
        cmp    al, 0x10
        jne    loc_3_80BF
        cmp    al, 0x11
        jne    loc_3_809F
loc_3_8097:
        mov    dword ptr [esi], 0
        jmp    loc_3_80A5
loc_3_809F:
        mov    dword ptr [esi], 1
loc_3_80A5:
        push   dword ptr [esi + 9]
        push   ecx
        call   sub_3_7FDC
        movzx  eax, al
        push   eax
        push   0x504
loc_3_80B7:
        push   dword ptr [esi + 5]
        call   sub_1_06F0
loc_3_80BF:
        pop    esi
        ret    8
        db  000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x8  RW-
        db  04Dh, 043h, 058h, 000h, 000h, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x5e  R-X
sub_5_0000:
        VxDCall  SHELL._SHELL_CancelAppyTimeEvent ; cd200f801700
        add    byte ptr [eax], al
sub_5_0008:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        add    byte ptr [eax], al
sub_5_0010:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        add    byte ptr [eax], al
sub_5_0018:
        VMMCall  VMM._RegEnumValue               ; cd2050810100
        add    byte ptr [eax], al
sub_5_0020:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        add    byte ptr [eax], al
sub_5_0028:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_5_0030:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_5_0038:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  000h
sub_5_0048:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        add    byte ptr [eax], al
sub_5_0050:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        add    byte ptr [eax], al
sub_5_0058:
        VxDCall  Dev_049D.Service_8001h          ; cd2001809d04
VxD__LTEXT ENDS

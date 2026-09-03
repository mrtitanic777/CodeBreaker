; lmouse_pcode_02_02.asm -- LMOUSE object 2 segment _PTEXT  (part 2/3)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_2_3F69:
        push   ebp
        mov    ebp, esp
        call   sub_2_2591
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, eax
        sub    edx, dword ptr [ecx + 48h]
        cmp    edx, 2eeh
        ja     SHORT loc_2_3F85
        sub    eax, eax
        jmp    SHORT loc_2_3F8D
loc_2_3F85:
        mov    dword ptr [ecx + 48h], eax
        mov    eax, 1
loc_2_3F8D:
        pop    ebp
        ret
sub_2_3F8F:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    word ptr [eax + 63h], 18h
        mov    dword ptr [eax + 8], 18h
        mov    dword ptr [eax + 4], 18h
        mov    eax, 1
        ret
sub_2_3FB0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        sub    word ptr [eax + 67h], 2
        mov    word ptr [eax + 63h], 19h
        mov    dword ptr [eax + 8], 19h
        mov    dword ptr [eax + 4], 19h
        mov    eax, 1
        ret
sub_2_3FD6:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    al, byte ptr [ebp + 8]
        cmp    al, 1
        jne    SHORT loc_2_3FF6
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_2_3F8F
        add    esp, 4
        mov    dword ptr [esi + 77h], 1
        jmp    SHORT loc_2_401E
loc_2_3FF6:
        cmp    al, 3
        mov    esi, dword ptr [ebp + 0ch]
        jne    SHORT loc_2_400F
        push   esi
        call   sub_2_3FB0
        add    esp, 4
        mov    dword ptr [esi + 77h], 1
        jmp    SHORT loc_2_401E
loc_2_400F:
        cmp    dword ptr [esi + 77h], 0
        jne    SHORT loc_2_401E
        push   esi
        call   sub_2_3FB0
        add    esp, 4
loc_2_401E:
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
sub_2_4026:
        push   ebp
        mov    ebp, esp
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    ecx, 10h
        je     SHORT loc_2_4050
        cmp    ecx, 11h
        je     SHORT loc_2_4050
        cmp    ecx, 12h
        jne    SHORT loc_2_4066
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 23h
        mov    dword ptr [eax + 4], 23h
        jmp    SHORT loc_2_4061
loc_2_4050:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 8], 22h
        mov    dword ptr [eax + 4], 22h
loc_2_4061:
        mov    eax, 1
loc_2_4066:
        pop    ebp
        ret
sub_2_4068:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        sub    esi, esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_455F
        add    esp, 0ch
        cmp    eax, esi
        je     SHORT loc_2_4098
        mov    eax, dword ptr [ebp - 4]
        mov    esi, 1
        mov    ecx, dword ptr [eax + 30h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], ecx
loc_2_4098:
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_409F:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx + 8], 0
        mov    dword ptr [ecx + 10h], 0
        mov    dword ptr [ecx + 14h], 0
        mov    dword ptr [ecx + 1ch], 0
        mov    dword ptr [ecx + 24h], 2
        mov    dword ptr [ecx + 28h], 0
        mov    dword ptr [ecx + 2ch], 0
        mov    word ptr [ecx + 50h], 0
        mov    word ptr [ecx + 52h], 0
        mov    dword ptr [ecx + 4ch], 0
        mov    dword ptr [ecx + 56h], 0
        mov    byte ptr [ecx + 5ah], 0
        mov    dword ptr [ecx + 30h], 0
        mov    dword ptr [ecx + 34h], 0
        mov    dword ptr [ecx + 3ch], 0
        mov    dword ptr [ecx + 40h], 0
        mov    dword ptr [ecx + 44h], 0
        mov    eax, dword ptr [ecx + 7bh]
        and    eax, 0fffffffeh
        mov    dword ptr [ecx + 7bh], eax
        and    eax, 0fffffffdh
        mov    dword ptr [ecx + 7bh], eax
        mov    word ptr [ecx + 69h], 0
        mov    word ptr [ecx + 65h], 0
        mov    word ptr [ecx + 67h], 0
        mov    dword ptr [ecx + 73h], 0
        mov    eax, 1
        mov    dword ptr [ecx + 6bh], 0
        mov    dword ptr [ecx + 6fh], 0
        mov    dword ptr [ecx + 77h], 0
        mov    dword ptr [ecx + 48h], 0
        mov    dword ptr [ecx + 83h], 0
        mov    dword ptr [ecx + 7fh], 0
        pop    ebp
        ret
sub_2_4180:
        enter  0, 0
        push   esi
        mov    eax, 0c0009h
        db  0BEh, 0A0h, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        mov    eax, 0
        jb     SHORT loc_2_41A7
        db  089h, 035h, 094h, 013h, 000h, 000h
        mov    eax, 1
loc_2_41A7:
        pop    esi
        leave
        ret
        db  0C8h, 000h, 000h, 000h, 056h, 0B8h, 009h, 000h, 00Ch, 000h, 0BEh, 0A0h, 000h, 000h, 000h, 0CDh
        db  020h, 01Ch, 001h, 001h, 000h, 0B8h, 001h, 000h, 000h, 000h, 073h, 005h, 0B8h, 000h, 000h, 000h
        db  000h, 05Eh, 0C9h, 0C3h, 000h, 000h
sub_2_41D0:
        push   ebp
        mov    ebp, esp
        push   esi
        sub    esi, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_41DB:
        mov    ecx, eax
        db  02Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    ecx, 3e8h
        jbe    SHORT loc_2_41FC
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [eax + 20h]
        add    esp, 4
        mov    esi, eax
loc_2_41FC:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret
sub_2_4201:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        push   dword ptr [esi + 30h]
        call   sub_2_965A
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_2_4228
        mov    edi, 1
        push   esi
        call   sub_2_41D0
        add    esp, 4
loc_2_4228:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_422E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        sub    esi, esi
        push   dword ptr [edi + 30h]
        call   sub_2_966F
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_2_4255
        mov    esi, 1
        push   edi
        call   sub_2_41D0
        add    esp, 4
loc_2_4255:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_425B:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        push   dword ptr [esi + 30h]
        call   sub_2_9699
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_2_4282
        mov    edi, 1
        push   esi
        call   sub_2_41D0
        add    esp, 4
loc_2_4282:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_4288:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_2_425B
        mov    esp, ebp
        pop    ebp
        ret
sub_2_4297:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_2_425B
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h
sub_2_42A8:
        db  0B8h, 000h, 000h, 000h, 000h
        ret
sub_2_42AE:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        add    eax, 4
        db  03Dh, 018h, 000h, 000h, 000h
        jb     SHORT loc_2_42C0
        sub    eax, eax
loc_2_42C0:
        pop    ebp
        ret
sub_2_42C2:
        call   sub_2_42A8
        or     eax, eax
        je     SHORT loc_2_42E1
loc_2_42CB:
        cmp    dword ptr [eax], 0
        jne    SHORT loc_2_42DD
        push   eax
        call   sub_2_42AE
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_42CB
loc_2_42DD:
        or     eax, eax
        jne    SHORT loc_2_42E5
loc_2_42E1:
        sub    eax, eax
        jmp    SHORT loc_2_42E7
loc_2_42E5:
        mov    eax, dword ptr [eax]
loc_2_42E7:
        ret
sub_2_42E8:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  0A3h, 018h, 000h, 000h, 000h
        pop    ebp
        ret
sub_2_42F5:
        db  0A1h, 018h, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_4312
        push   eax
        call   sub_2_4619
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_4312
        db  0A1h, 018h, 000h, 000h, 000h
        jmp    SHORT loc_2_4317
loc_2_4312:
        call   sub_2_42C2
loc_2_4317:
        ret
sub_2_4318:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 6
        jb     SHORT loc_2_432A
        call   sub_2_42F5
        jmp    SHORT loc_2_4331
loc_2_432A:
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
loc_2_4331:
        pop    ebp
        ret
sub_2_4333:
        push   ebp
        mov    ebp, esp
        call   sub_2_42A8
        or     eax, eax
        je     SHORT loc_2_435A
loc_2_433F:
        cmp    dword ptr [eax], 0
        je     SHORT loc_2_4351
        push   eax
        call   sub_2_42AE
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_433F
loc_2_4351:
        or     eax, eax
        je     SHORT loc_2_435A
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax], ecx
loc_2_435A:
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        pop    ebp
        ret
sub_2_4362:
        push   ebp
        mov    ebp, esp
        push   esi
        call   sub_2_42A8
        or     eax, eax
        je     SHORT loc_2_4394
        mov    esi, dword ptr [ebp + 8]
loc_2_4372:
        cmp    dword ptr [eax], esi
        je     SHORT loc_2_4383
        push   eax
        call   sub_2_42AE
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_4372
loc_2_4383:
        or     eax, eax
        je     SHORT loc_2_4394
        mov    dword ptr [eax], 0
        mov    eax, 1
        jmp    SHORT loc_2_4396
loc_2_4394:
        sub    eax, eax
loc_2_4396:
        pop    esi
        pop    ebp
        ret
sub_2_4399:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [eax + 18h]
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_43B3
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0
loc_2_43B3:
        sub    eax, eax
        pop    ebp
        ret
sub_2_43B7:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_4594
        add    esp, 8
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        pop    ebp
        ret
sub_2_43D3:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [eax + 1ch]
        mov    esp, ebp
        pop    ebp
        ret
sub_2_43E7:
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        push   1
        push   0dbh
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_43F8:
        add    esp, 8
        mov    edi, eax
        cmp    edi, esi
        je     NEAR PTR loc_2_4497
        push   1
        push   10h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_440E:
        add    esp, 8
        mov    dword ptr [edi + 4], eax
        push   1
        push   10h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_441D:
        add    esp, 8
        mov    dword ptr [edi + 8], eax
        push   0ah
        lea    eax, [edi + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_442E:
        add    esp, 8
        push   edi
        call   sub_2_9725
        add    esp, 4
        mov    ebx, eax
        mov    eax, dword ptr [edi + 10h]
        cmp    eax, esi
        je     SHORT loc_2_4458
        cmp    dword ptr [edi + 4], esi
        je     SHORT loc_2_4458
        cmp    dword ptr [edi + 8], esi
        je     SHORT loc_2_4458
        cmp    ebx, esi
        je     SHORT loc_2_4458
        mov    esi, 1
        jmp    SHORT loc_2_4488
loc_2_4458:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_445E:
        add    esp, 4
        push   0
        push   dword ptr [edi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_446B:
        add    esp, 8
        push   0
        push   dword ptr [edi + 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_4478:
        add    esp, 8
        or     ebx, ebx
        je     SHORT loc_2_4488
        push   edi
        call   sub_2_97BB
        add    esp, 4
loc_2_4488:
        or     esi, esi
        jne    SHORT loc_2_449D
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_4494:
        add    esp, 8
loc_2_4497:
        or     esi, esi
        jne    SHORT loc_2_449D
        sub    edi, edi
loc_2_449D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_2_44A3:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_97BB
        add    esp, 4
        mov    eax, dword ptr [esi + 0abh]
        or     eax, eax
        je     SHORT loc_2_44C8
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_44C5:
        add    esp, 8
loc_2_44C8:
        mov    eax, dword ptr [esi + 0a7h]
        or     eax, eax
        je     SHORT loc_2_44DD
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_44DA:
        add    esp, 8
loc_2_44DD:
        push   dword ptr [esi + 10h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_44E5:
        add    esp, 4
        push   0
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_44F2:
        add    esp, 8
        push   0
        push   dword ptr [esi + 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_44FF:
        add    esp, 8
        mov    eax, dword ptr [esi + 0d7h]
        or     eax, eax
        je     SHORT loc_2_4517
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_4514:
        add    esp, 8
loc_2_4517:
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_451F:
        add    esp, 8
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
sub_2_452A:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   1
        push   dword ptr [ebp + 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_453B:
        add    esp, 8
        mov    dword ptr [esi + 0d7h], eax
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        pop    esi
        pop    ebp
        ret
        db  000h, 000h, 000h
sub_2_4550:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_2_4318
        mov    esp, ebp
        pop    ebp
        ret
sub_2_455F:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, 1
        push   dword ptr [ebp + 8]
        call   sub_2_4550
        mov    ecx, dword ptr [ebp + 0ch]
        add    esp, 4
        mov    dword ptr [ecx], eax
        or     eax, eax
        mov    eax, dword ptr [ebp + 10h]
        jne    SHORT loc_2_4589
        mov    dword ptr [eax], 0ffffffffh
        sub    esi, esi
        jmp    SHORT loc_2_458F
loc_2_4589:
        mov    dword ptr [eax], 0
loc_2_458F:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret
sub_2_4594:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        call   sub_2_42A8
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_2_45CB
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
loc_2_45AB:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_2_45BC
        push   ebx
        push   eax
        call   edi
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_45CB
loc_2_45BC:
        push   esi
        call   sub_2_42AE
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_2_45AB
loc_2_45CB:
        cmp    esi, 1
        sbb    eax, eax
        inc    eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_2_45D6:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        inc    dword ptr [eax]
        sub    eax, eax
        ret
sub_2_45E2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_4594
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
loc_2_4607:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 0ch]
        sub    ecx, dword ptr [ebp + 8]
        cmp    ecx, 1
        sbb    eax, eax
        neg    eax
        pop    ebp
        ret
sub_2_4619:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_4594
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h
sub_2_4630:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_43D3
        mov    esp, ebp
        pop    ebp
        ret
sub_2_4644:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 2ch]
        push   edi
        call   sub_2_966F
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_4676
        push   esi
        call   sub_2_41D0
        add    esp, 4
        cmp    dword ptr [esi + 2ch], edi
        je     SHORT loc_2_4676
        push   dword ptr [esi + 2ch]
        push   esi
        call   sub_2_9371
        add    esp, 8
loc_2_4676:
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  000h, 000h
loc_2_467C:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_2_4644
        add    esp, 4
        sub    eax, eax
        pop    ebp
        ret
sub_2_468E:
        push   0
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_4594
        add    esp, 8
        ret
        db  000h, 000h
loc_2_46A0:
        push   ebp
        mov    esi, ebp
        db  083h, 03Dh, 074h, 002h, 000h, 000h, 000h
        je     SHORT loc_2_4721
        movzx  ecx, word ptr [esi + 1ch]
        cmp    ecx, 34h
        ja     SHORT loc_2_46CA
        db  066h, 08Bh, 00Ch, 04Dh, 018h, 052h, 000h, 000h
        movsx  ecx, cx
        db  081h, 0C1h, 082h, 052h, 000h, 000h
        call   ecx
        jmp    SHORT loc_2_4725
loc_2_46CA:
        cmp    cl, 6ch
        jne    SHORT loc_2_46F1
        shr    ecx, 8
        sub    ecx, 10h
        js     SHORT loc_2_4721
        cmp    ecx, 1dh
        ja     SHORT loc_2_4721
        db  066h, 08Bh, 00Ch, 04Dh, 088h, 003h, 000h, 000h
        movsx  ecx, cx
        db  081h, 0C1h, 0B0h, 048h, 000h, 000h
        call   ecx
        jmp    SHORT loc_2_4725
loc_2_46F1:
        cmp    ecx, 4dh
        jne    SHORT loc_2_470A
        mov    ecx, 64h
        mov    word ptr [esi], cx
        db  08Bh, 00Dh, 060h, 002h, 000h, 000h
        mov    word ptr [esi + 38h], cx
        jmp    SHORT loc_2_4724
loc_2_470A:
        cmp    ecx, 6dh
        jne    SHORT loc_2_4721
        db  08Bh, 00Dh, 058h, 002h, 000h, 000h
        mov    word ptr [esi], cx
        shr    ecx, 10h
        mov    word ptr [esi + 38h], cx
        jmp    SHORT loc_2_4724
loc_2_4721:
        stc
        jmp    SHORT loc_2_4725
loc_2_4724:
        clc
loc_2_4725:
        pop    ebp
        ret
        db  0C8h, 000h, 000h, 000h, 056h, 083h, 03Dh, 074h, 002h, 000h, 000h, 001h, 075h, 017h, 0B8h, 033h
        db  000h, 000h, 000h, 0BEh, 0A0h, 046h, 000h, 000h, 0CDh, 020h, 018h, 001h, 001h, 000h, 0B8h, 001h
        db  000h, 000h, 000h, 073h, 005h, 0B8h, 000h, 000h, 000h, 000h, 05Eh, 0C9h, 0C3h
sub_2_4754:
        enter  0, 0
        push   esi
        mov    eax, 33h
        db  0BEh, 0A0h, 046h, 000h, 000h
        db  0CDh, 020h, 041h, 000h, 001h, 000h
        mov    eax, 0
        jb     SHORT loc_2_477F
        call   sub_2_51E9
        call   sub_2_55A4
        mov    eax, 1
loc_2_477F:
        db  0A3h, 074h, 002h, 000h, 000h
        pop    esi
        leave
        ret
sub_2_4787:
        push   ebp
        mov    ebp, esp
        add    esp, -8
        db  0C7h, 005h, 04Ch, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        push   204h
        push   179h
        db  0CDh, 020h, 0A8h, 000h, 001h, 000h
        add    esp, 8
        or     eax, eax
        je     NEAR PTR loc_2_48AB
        mov    dword ptr [ebp - 4], eax
        mov    eax, 33h
        db  0CDh, 020h, 042h, 000h, 001h, 000h
        db  066h, 089h, 015h, 078h, 002h, 000h, 000h
        db  066h, 089h, 00Dh, 07Ah, 002h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        xor    ecx, ecx
        db  0CDh, 020h, 01Dh, 000h, 001h, 000h
        jb     NEAR PTR loc_2_48AB
        and    ecx, 0ffffh
        db  089h, 00Dh, 060h, 002h, 000h, 000h
        mov    dword ptr [ebp - 8], edx
        mov    edi, dword ptr [ebp - 4]
        db  0BEh, 078h, 002h, 000h, 000h
        mov    ecx, 0f9h
        rep movsb
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0f9h
        push   eax
        push   73h
        push   eax
        call   sub_2_8933
        add    esp, 8
        or     eax, eax
        pop    eax
        jne    SHORT loc_2_4834
        mov    edi, eax
        xor    eax, eax
        cld
        mov    ecx, 73h
        repne scasb
        jne    SHORT loc_2_4834
        dec    edi
        db  0BEh, 067h, 002h, 000h, 000h
        mov    ecx, 0dh
        jmp    SHORT loc_2_4847
loc_2_4834:
        mov    ecx, 10h
        db  0BEh, 064h, 002h, 000h, 000h
        mov    edi, dword ptr [ebp - 4]
        add    edi, 0f9h
loc_2_4847:
        rep movsb
        db  08Bh, 00Dh, 060h, 002h, 000h, 000h
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        add    edx, 80h
        mov    eax, 33h
        db  0CDh, 020h, 043h, 000h, 001h, 000h
        pop    eax
        db  066h, 0C7h, 005h, 050h, 002h, 000h, 000h, 0F9h, 000h
        db  066h, 0A3h, 052h, 002h, 000h, 000h
        db  066h, 0C7h, 005h, 054h, 002h, 000h, 000h, 0B9h, 000h
        db  066h, 0A3h, 056h, 002h, 000h, 000h
        db  066h, 0C7h, 005h, 058h, 002h, 000h, 000h, 090h, 000h
        db  066h, 0C7h, 005h, 05Ch, 002h, 000h, 000h, 0A5h, 000h
        db  066h, 0A3h, 05Ah, 002h, 000h, 000h
        db  066h, 0A3h, 05Eh, 002h, 000h, 000h
        db  0C7h, 005h, 04Ch, 002h, 000h, 000h, 001h, 000h, 000h, 000h
loc_2_48AB:
        leave
        ret
        db  000h, 000h, 000h
loc_2_48B0:
        xor    eax, eax
        mov    word ptr [esi + 1ch], 0ffffh
        mov    word ptr [esi + 10h], 10h
        mov    word ptr [esi + 18h], ax
        mov    word ptr [esi], ax
        mov    word ptr [esi + 14h], cs
        mov    word ptr [esi + 4], ds
        clc
        ret
        db  066h, 0C7h, 046h, 01Ch, 0FFh, 0FFh, 066h, 0C7h, 046h, 010h, 02Ah, 006h, 066h, 0C7h, 046h, 018h
        db  000h, 000h, 066h, 0C7h, 046h, 014h, 000h, 000h, 0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_676C
        add    esp, 8
        or     eax, eax
        mov    edx, dword ptr [ebp - 4]
        mov    eax, 0ffffffffh
        jge    SHORT loc_2_490C
        xor    eax, eax
        xor    edx, edx
loc_2_490C:
        mov    word ptr [esi + 14h], dx
        leave
        ret
        db  083h, 0F9h, 064h, 077h, 013h, 051h, 053h, 0E8h, 0BBh, 023h, 000h, 000h, 083h, 0C4h, 008h, 00Bh
        db  0C0h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 07Dh, 002h, 033h, 0C0h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_6CBE
        add    esp, 8
        or     eax, eax
        mov    eax, 0ffffffffh
        mov    edx, dword ptr [ebp - 4]
        jge    SHORT loc_2_4950
        xor    eax, eax
        xor    edx, edx
loc_2_4950:
        mov    word ptr [esi + 14h], dx
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_6C89
        add    esp, 8
        or     eax, eax
        mov    eax, 0ffffffffh
        mov    edx, dword ptr [ebp - 4]
        jge    SHORT loc_2_4979
        xor    eax, eax
        xor    edx, edx
loc_2_4979:
        mov    word ptr [esi + 14h], dx
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -4
        movzx  eax, word ptr [esi + 18h]
        lea    ecx, [ebp - 4]
        push   ecx
        push   eax
        push   ebx
        call   sub_2_6C44
        add    esp, 0ch
        or     eax, eax
        mov    eax, 0ffffffffh
        mov    edx, dword ptr [ebp - 4]
        jge    SHORT loc_2_49A7
        xor    eax, eax
        xor    edx, edx
loc_2_49A7:
        mov    word ptr [esi + 14h], dx
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_6C14
        add    esp, 8
        or     eax, eax
        mov    eax, 0ffffffffh
        mov    edx, dword ptr [ebp - 4]
        jge    SHORT loc_2_49D0
        xor    eax, eax
        xor    edx, edx
loc_2_49D0:
        mov    word ptr [esi + 14h], dx
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_6BE4
        add    esp, 8
        or     eax, eax
        mov    eax, 0ffffffffh
        mov    edx, dword ptr [ebp - 4]
        jge    SHORT loc_2_49F9
        xor    eax, eax
        xor    edx, edx
loc_2_49F9:
        mov    word ptr [esi + 14h], dx
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_6BB3
        add    esp, 8
        or     eax, eax
        mov    eax, 0ffffffffh
        mov    edx, dword ptr [ebp - 4]
        jge    SHORT loc_2_4A22
        xor    eax, eax
        xor    edx, edx
loc_2_4A22:
        mov    word ptr [esi + 14h], dx
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -8
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   ecx
        push   eax
        push   ebx
        call   sub_2_691D
        add    esp, 0ch
        or     eax, eax
        mov    eax, 0ffffffffh
        mov    ecx, dword ptr [ebp - 4]
        dec    ecx
        mov    edx, dword ptr [ebp - 8]
        dec    edx
        jge    SHORT loc_2_4A56
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
loc_2_4A56:
        mov    word ptr [esi + 18h], cx
        mov    word ptr [esi + 14h], dx
        leave
        ret
        db  000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        db  004h, 000h, 000h, 000h
sub_2_4A74:
        cmp    edx, 4
        jbe    SHORT loc_2_4A7B
        xor    edx, edx
loc_2_4A7B:
        db  08Bh, 014h, 095h, 060h, 04Ah, 000h, 000h
        ret
sub_2_4A83:
        push   ebx
        xor    ebx, ebx
loc_2_4A86:
        inc    ebx
        shr    eax, 1
        or     eax, eax
        jne    SHORT loc_2_4A86
        mov    eax, ebx
        pop    ebx
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -4
        mov    eax, ecx
        call   sub_2_4A83
        lea    ecx, [ebp - 4]
        push   ecx
        push   eax
        push   ebx
        call   sub_2_69ED
        add    esp, 0ch
        or     eax, eax
        jl     SHORT loc_2_4ABF
        mov    edx, dword ptr [ebp - 4]
        call   sub_2_4A74
        mov    eax, 0ffffffffh
        jmp    SHORT loc_2_4AC3
loc_2_4ABF:
        xor    eax, eax
        xor    edx, edx
loc_2_4AC3:
        mov    word ptr [esi + 14h], dx
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -4
        inc    ecx
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        push   ebx
        call   sub_2_69AE
        add    esp, 0ch
        or     eax, eax
        jl     SHORT loc_2_4AF1
        mov    edx, dword ptr [ebp - 4]
        call   sub_2_4A74
        mov    eax, 0ffffffffh
        jmp    SHORT loc_2_4AF5
loc_2_4AF1:
        xor    eax, eax
        xor    edx, edx
loc_2_4AF5:
        mov    word ptr [esi + 14h], dx
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -4
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_2_6DDA
        add    esp, 8
        or     eax, eax
        mov    eax, 0ffffffffh
        mov    edx, dword ptr [ebp - 4]
        jge    SHORT loc_2_4B1E
        xor    eax, eax
        xor    edx, edx
loc_2_4B1E:
        mov    word ptr [esi + 14h], dx
        leave
        ret
        db  083h, 0F9h, 020h, 077h, 013h, 051h, 053h, 0E8h, 0C5h, 022h, 000h, 000h, 083h, 0C4h, 008h, 00Bh
        db  0C0h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 07Dh, 002h, 033h, 0C0h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -8
        mov    dword ptr [ebp - 8], 0
        lea    eax, [ebp - 4]
        push   eax
        push   1
        push   ebx
        call   sub_2_6D10
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_4B65
        or     dword ptr [ebp - 8], 1
loc_2_4B65:
        lea    eax, [ebp - 4]
        push   eax
        push   2
        push   ebx
        call   sub_2_6D10
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_4B7E
        or     dword ptr [ebp - 8], 2
loc_2_4B7E:
        lea    eax, [ebp - 4]
        push   eax
        push   3
        push   ebx
        call   sub_2_6D10
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_4B97
        or     dword ptr [ebp - 8], 4
loc_2_4B97:
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [esi + 18h], cx
        mov    word ptr [esi + 14h], 1
        mov    eax, 0ffffffffh
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -4
        xor    eax, eax
        or     edx, edx
        jne    SHORT loc_2_4BB9
        xor    ecx, ecx
loc_2_4BB9:
        push   edx
        and    edx, 0fffffffeh
        pop    edx
        jne    SHORT loc_2_4C20
        cmp    ecx, 7
        ja     SHORT loc_2_4C20
        mov    dword ptr [ebp - 4], ecx
        mov    eax, 0
        test   ecx, 1
        je     SHORT loc_2_4BD7
        not    eax
loc_2_4BD7:
        push   eax
        push   1
        push   ebx
        call   sub_2_6D62
        add    esp, 0ch
        mov    eax, 0
        test   dword ptr [ebp - 4], 2
        je     SHORT loc_2_4BF3
        not    eax
loc_2_4BF3:
        push   eax
        push   2
        push   ebx
        call   sub_2_6D62
        add    esp, 0ch
        mov    eax, 0
        test   dword ptr [ebp - 4], 4
        je     SHORT loc_2_4C0F
        not    eax
loc_2_4C0F:
        push   eax
        push   3
        push   ebx
        call   sub_2_6D62
        add    esp, 0ch
        mov    eax, 0ffffffffh
loc_2_4C20:
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -8
        mov    dword ptr [ebp - 8], 0
        lea    eax, [ebp - 4]
        push   eax
        push   1
        push   ebx
        call   sub_2_67A2
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_4C48
        or     dword ptr [ebp - 8], 1
loc_2_4C48:
        lea    eax, [ebp - 4]
        push   eax
        push   2
        push   ebx
        call   sub_2_67A2
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_4C61
        or     dword ptr [ebp - 8], 2
loc_2_4C61:
        lea    eax, [ebp - 4]
        push   eax
        push   3
        push   ebx
        call   sub_2_67A2
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_4C7A
        or     dword ptr [ebp - 8], 4
loc_2_4C7A:
        lea    eax, [ebp - 4]
        push   eax
        push   4
        push   ebx
        call   sub_2_67A2
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_4C93
        or     dword ptr [ebp - 8], 8
loc_2_4C93:
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [esi + 18h], cx
        mov    edx, 1
        mov    word ptr [esi + 14h], dx
        mov    eax, 0ffffffffh
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -4
        xor    eax, eax
        or     edx, edx
        jne    SHORT loc_2_4CB8
        xor    ecx, ecx
loc_2_4CB8:
        push   edx
        and    edx, 0fffffffeh
        pop    edx
        jne    SHORT loc_2_4D3B
        cmp    ecx, 0fh
        ja     SHORT loc_2_4D3B
        mov    dword ptr [ebp - 4], ecx
        mov    eax, 0
        test   ecx, 1
        je     SHORT loc_2_4CD6
        not    eax
loc_2_4CD6:
        push   eax
        push   1
        push   ebx
        call   sub_2_67F4
        add    esp, 0ch
        mov    eax, 0
        test   dword ptr [ebp - 4], 2
        je     SHORT loc_2_4CF2
        not    eax
loc_2_4CF2:
        push   eax
        push   2
        push   ebx
        call   sub_2_67F4
        add    esp, 0ch
        mov    eax, 0
        test   dword ptr [ebp - 4], 4
        je     SHORT loc_2_4D0E
        not    eax
loc_2_4D0E:
        push   eax
        push   3
        push   ebx
        call   sub_2_67F4
        add    esp, 0ch
        mov    eax, 0
        test   dword ptr [ebp - 4], 8
        je     SHORT loc_2_4D2A
        not    eax
loc_2_4D2A:
        push   eax
        push   4
        push   ebx
        call   sub_2_67F4
        add    esp, 0ch
        mov    eax, 0ffffffffh
loc_2_4D3B:
        leave
        ret
loc_2_4D3D:
        push   ebp
        mov    ebp, esp
        add    esp, -4
        cmp    ecx, 4
        mov    eax, 0
        jae    SHORT loc_2_4D77
        inc    ecx
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        push   ebx
        call   sub_2_694F
        add    esp, 0ch
        or     eax, eax
        mov    eax, 0
        mov    edx, eax
        jl     SHORT loc_2_4D73
        mov    eax, 0ffffffffh
        mov    ecx, dword ptr [ebp - 4]
        dec    ecx
        inc    edx
        shl    edx, cl
loc_2_4D73:
        mov    word ptr [esi + 14h], dx
loc_2_4D77:
        leave
        ret
        db  083h, 0F9h, 004h, 073h, 021h, 041h, 083h, 0FAh, 008h, 077h, 01Bh, 08Bh, 0C2h, 0E8h, 0F8h, 0FCh
        db  0FFh, 0FFh, 050h, 051h, 053h, 0E8h, 0EEh, 01Bh, 000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 0B8h
        db  0FFh, 0FFh, 0FFh, 0FFh, 07Dh, 002h, 033h, 0C0h, 0C3h, 00Bh, 0DBh, 074h, 001h, 04Bh, 053h, 0E8h
        db  0A3h, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0C3h
sub_2_4DB1:
        push   eax
        push   ecx
        push   6
        call   sub_2_4550
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        pop    ecx
        pop    eax
        ret
        db  033h, 0DBh, 08Ah, 05Eh, 011h, 0E8h, 0D4h, 0FFh, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 02Dh, 08Bh, 0D8h
        db  00Fh, 0B6h, 046h, 010h, 03Dh, 013h, 000h, 000h, 000h, 077h, 020h, 00Fh, 0B7h, 04Eh, 018h, 00Fh
        db  0B7h, 056h, 014h, 066h, 08Bh, 004h, 045h, 0C4h, 003h, 000h, 000h, 00Fh, 0BFh, 0C0h, 005h, 03Dh
        db  04Dh, 000h, 000h, 0FFh, 0D0h, 066h, 089h, 046h, 01Ch, 0EBh, 006h, 066h, 0C7h, 046h, 01Ch, 000h
        db  000h, 0F8h, 0C3h, 066h, 0C7h, 046h, 01Ch, 000h, 000h, 0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        mov    eax, 0
        movzx  ebx, byte ptr [esi + 10h]
        or     ebx, ebx
        je     SHORT loc_2_4E51
        lea    eax, [ebp - 4]
        push   eax
        push   6
        call   sub_2_4068
        add    esp, 8
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_2_4E51
        xor    ebx, ebx
        cmp    dword ptr [ebp - 4], 14h
        je     SHORT loc_2_4E47
        cmp    dword ptr [ebp - 4], 15h
        jne    SHORT loc_2_4E49
loc_2_4E47:
        not    bl
loc_2_4E49:
        mov    byte ptr [esi + 11h], bl
        mov    eax, 0ffffffffh
loc_2_4E51:
        mov    word ptr [esi + 1ch], ax
        clc
        leave
        ret
        db  083h, 03Dh, 04Ch, 002h, 000h, 000h, 001h, 075h, 018h, 0A1h, 058h, 002h, 000h, 000h, 066h, 089h
        db  006h, 0C1h, 0E8h, 010h, 066h, 089h, 046h, 038h, 0A1h, 05Ch, 002h, 000h, 000h, 066h, 089h, 046h
        db  004h, 0F8h, 0C3h, 080h, 07Eh, 010h, 003h, 075h, 013h, 056h, 06Ah, 006h, 0E8h, 089h, 008h, 000h
        db  000h, 083h, 0C4h, 008h, 066h, 0C7h, 046h, 01Ch, 0FFh, 0FFh, 0EBh, 006h, 066h, 0C7h, 046h, 01Ch
        db  000h, 000h, 0F8h, 0C3h, 00Fh, 0B7h, 05Eh, 010h, 00Fh, 0B7h, 04Eh, 018h, 00Ah, 0DBh, 075h, 050h
        db  066h, 08Bh, 046h, 038h, 0C1h, 0E0h, 010h, 066h, 08Bh, 006h, 00Ah, 0C9h, 075h, 021h, 00Ah, 0FFh
        db  074h, 00Ch, 0C7h, 005h, 080h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 0EBh, 06Eh, 0C7h, 005h
        db  080h, 003h, 000h, 000h, 001h, 000h, 000h, 000h, 0A3h, 084h, 003h, 000h, 000h, 0EBh, 05Dh, 00Ah
        db  0FFh, 074h, 00Ch, 0C7h, 005h, 078h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 0EBh, 04Dh, 0C7h
        db  005h, 078h, 003h, 000h, 000h, 001h, 000h, 000h, 000h, 0A3h, 07Ch, 003h, 000h, 000h, 0EBh, 03Ch
        db  00Ah, 0C9h, 075h, 01Ah, 083h, 03Dh, 080h, 003h, 000h, 000h, 001h, 075h, 037h, 0A1h, 084h, 003h
        db  000h, 000h, 066h, 089h, 006h, 0C1h, 0E8h, 010h, 066h, 089h, 046h, 038h, 0EBh, 01Eh, 083h, 03Dh
        db  078h, 003h, 000h, 000h, 001h, 075h, 01Dh, 0A1h, 07Ch, 003h, 000h, 000h, 066h, 089h, 006h, 0C1h
        db  0E8h, 010h, 066h, 089h, 046h, 038h, 066h, 0C7h, 046h, 004h, 000h, 000h, 066h, 0C7h, 046h, 01Ch
        db  0FFh, 0FFh, 0EBh, 006h, 066h, 0C7h, 046h, 01Ch, 000h, 000h, 0F8h, 0C3h, 066h, 0C7h, 046h, 01Ch
        db  000h, 000h, 0F8h, 0C3h, 066h, 0C7h, 046h, 01Ch, 000h, 000h, 0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        mov    word ptr [esi + 1ch], 1a6ch
        xor    ebx, ebx
        mov    word ptr [esi + 18h], bx
        lea    eax, [ebp - 4]
        call   sub_2_4DB1
        je     SHORT loc_2_4FAE
        lea    eax, [ebp - 4]
        push   eax
        push   3
        push   edx
        call   sub_2_67A2
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_4F87
        inc    ebx
        inc    ebx
loc_2_4F87:
        call   sub_2_4DB1
        lea    eax, [ebp - 4]
        push   eax
        push   2
        push   edx
        call   sub_2_67A2
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_4FA4
        or     ebx, 4
loc_2_4FA4:
        mov    word ptr [esi + 1ch], 0ffffh
        mov    word ptr [esi + 10h], bx
loc_2_4FAE:
        clc
        leave
        ret
        db  000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 004h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  002h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 006h, 000h, 000h, 000h, 0B8h, 06Ch, 01Bh, 000h
        db  000h, 00Fh, 0B7h, 05Eh, 010h, 083h, 0FBh, 006h, 077h, 02Bh, 08Bh, 00Ch, 09Dh, 0B1h, 04Fh, 000h
        db  000h, 083h, 0F9h, 0FFh, 074h, 01Fh, 0E8h, 0C5h, 0FDh, 0FFh, 0FFh, 075h, 007h, 0B8h, 06Ch, 01Bh
        db  000h, 000h, 0EBh, 011h, 08Bh, 0DAh, 0BAh, 001h, 000h, 000h, 000h, 0E8h, 0A9h, 0FCh, 0FFh, 0FFh
        db  0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 066h, 089h, 046h, 01Ch, 0F8h, 0C3h, 066h, 0C7h, 046h, 01Ch, 000h
        db  000h, 0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        call   sub_2_4DB1
        mov    eax, 0
        je     SHORT loc_2_5051
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_6B61
        add    esp, 8
        xor    edx, edx
        mov    eax, dword ptr [ebp - 4]
        mov    ebx, 5ah
        div    bx
        mov    ebx, 3
        or     eax, eax
        je     SHORT loc_2_5051
        cmp    eax, ebx
        je     SHORT loc_2_5051
        and    eax, ebx
        xor    eax, ebx
loc_2_5051:
        mov    byte ptr [esi + 10h], al
        clc
        leave
        ret
        db  00Fh, 0BEh, 046h, 010h, 0BBh, 003h, 000h, 000h, 000h, 03Bh, 0C3h, 077h, 020h, 074h, 006h, 00Bh
        db  0C0h, 074h, 009h, 033h, 0C3h, 0BBh, 05Ah, 000h, 000h, 000h, 0F7h, 0E3h, 0E8h, 039h, 0FDh, 0FFh
        db  0FFh, 074h, 00Ah, 050h, 052h, 0E8h, 0FBh, 01Ah, 000h, 000h, 083h, 0C4h, 008h, 0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        call   sub_2_4DB1
        mov    ebx, 0
        mov    ecx, ebx
        je     SHORT loc_2_50C3
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_66A4
        add    esp, 8
        xor    ebx, ebx
        mov    ecx, dword ptr [ebp - 4]
        or     ecx, ecx
        jne    SHORT loc_2_50B8
        db  08Bh, 00Dh, 074h, 003h, 000h, 000h
        jmp    SHORT loc_2_50C3
loc_2_50B8:
        inc    ebx
        cmp    ecx, 1
        je     SHORT loc_2_50C3
        mov    ecx, 2
loc_2_50C3:
        mov    word ptr [esi + 10h], bx
        mov    word ptr [esi + 18h], cx
        clc
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -4
        cmp    word ptr [esi + 10h], 55h
        je     SHORT loc_2_511C
        call   sub_2_4DB1
        je     SHORT loc_2_511C
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_6860
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        jecxz  loc_2_5102
        cmp    word ptr [esi + 10h], 0
        jne    SHORT loc_2_511C
        mov    ecx, 0
        jmp    SHORT loc_2_510D
loc_2_5102:
        cmp    word ptr [esi + 10h], cx
        je     SHORT loc_2_511C
        mov    ecx, 1
loc_2_510D:
        call   sub_2_4DB1
        push   ecx
        push   edx
        call   sub_2_68C6
        add    esp, 8
loc_2_511C:
        clc
        leave
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -4
        call   sub_2_4DB1
        mov    ecx, 0
        je     SHORT loc_2_5148
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_6860
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        jecxz  loc_2_5148
        mov    ecx, 0ffffffffh
loc_2_5148:
        mov    byte ptr [esi + 10h], cl
        clc
        leave
        ret
        db  066h, 0C7h, 046h, 01Ch, 000h, 000h, 0F8h, 0C3h, 00Fh, 0B7h, 05Eh, 010h, 00Fh, 0B7h, 04Eh, 018h
        db  0E3h, 010h, 083h, 0F9h, 002h, 076h, 005h, 0B9h, 002h, 000h, 000h, 000h, 089h, 00Dh, 074h, 003h
        db  000h, 000h, 08Bh, 00Dh, 074h, 003h, 000h, 000h, 00Bh, 0DBh, 075h, 002h, 033h, 0C9h, 0E8h, 030h
        db  0FCh, 0FFh, 0FFh, 074h, 00Ah, 051h, 052h, 0E8h, 035h, 015h, 000h, 000h, 083h, 0C4h, 008h, 0F8h
        db  0C3h, 097h, 066h, 0B8h, 014h, 038h, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 097h, 0B8h, 000h, 000h
        db  000h, 000h, 083h, 0FFh, 0FFh, 074h, 016h, 057h, 0B9h, 010h, 000h, 000h, 000h, 033h, 0C0h, 0F3h
        db  0AAh, 05Fh, 057h, 06Ah, 006h, 0E8h, 0F0h, 003h, 000h, 000h, 083h, 0C4h, 008h, 066h, 0C7h, 046h
        db  01Ch, 0FFh, 0FFh, 0F8h, 0C3h, 066h, 0C7h, 046h, 01Ch, 000h, 000h, 0F8h, 0C3h, 066h, 0C7h, 046h
        db  010h, 053h, 053h, 0B4h, 036h, 0B0h, 034h, 066h, 089h, 046h, 018h, 0F8h, 0C3h, 0F8h, 0C3h, 0F8h
        db  0C3h, 0F8h, 0C3h, 0F8h, 0C3h, 0F8h, 0C3h, 0F8h, 0C3h, 0F8h, 0C3h
sub_2_51E9:
        call   sub_2_51F4
        call   sub_2_51FF
        ret
sub_2_51F4:
        db  0C7h, 005h, 074h, 003h, 000h, 000h, 001h, 000h, 000h, 000h
        ret
sub_2_51FF:
        xor    eax, eax
        db  0A3h, 078h, 003h, 000h, 000h
        db  0A3h, 07Ch, 003h, 000h, 000h
        db  0A3h, 080h, 003h, 000h, 000h
        db  0A3h, 084h, 003h, 000h, 000h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 005h, 000h, 055h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 095h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0B7h, 000h, 0B9h, 000h, 0BBh, 000h, 000h, 000h
        db  000h, 000h, 07Bh, 001h, 000h, 000h, 0A6h, 002h, 0AEh, 002h, 001h, 003h
loc_2_5282:
        stc
        ret
        db  0F8h, 0C3h, 0C3h, 00Fh, 0B7h, 046h, 014h, 083h, 0F8h, 064h, 076h, 005h, 0B8h, 064h, 000h, 000h
        db  000h, 0A3h, 0ECh, 003h, 000h, 000h, 00Fh, 0B7h, 046h, 010h, 0E8h, 01Fh, 000h, 000h, 000h, 08Bh
        db  0D8h, 00Fh, 0B7h, 046h, 018h, 0E8h, 014h, 000h, 000h, 000h, 0E8h, 0FEh, 0FAh, 0FFh, 0FFh, 074h
        db  00Bh, 050h, 053h, 052h, 0E8h, 072h, 014h, 000h, 000h, 083h, 0C4h, 00Ch, 0F8h, 0C3h, 083h, 0F8h
        db  001h, 07Dh, 005h, 0B8h, 001h, 000h, 000h, 000h, 083h, 0F8h, 064h, 076h, 005h, 0B8h, 064h, 000h
        db  000h, 000h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -8
        xor    eax, eax
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], eax
        call   sub_2_4DB1
        je     SHORT loc_2_52FD
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_66F6
        add    esp, 0ch
loc_2_52FD:
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 10h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [esi + 18h], ax
        db  0A1h, 0ECh, 003h, 000h, 000h
        mov    word ptr [esi + 14h], ax
        clc
        leave
        ret
        db  066h, 0C7h, 046h, 010h, 020h, 008h, 033h, 0C0h, 033h, 0C9h, 0E8h, 08Bh, 0FAh, 0FFh, 0FFh, 074h
        db  006h, 08Bh, 042h, 05Ch, 08Bh, 04Ah, 028h, 0C1h, 0E1h, 008h, 00Bh, 0C1h, 066h, 089h, 046h, 018h
        db  0F8h, 0C3h, 0F8h, 0C3h, 0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        movsx  eax, word ptr [esi + 10h]
        inc    eax
        je     SHORT loc_2_536F
        dec    eax
        mov    ebx, 0fffffffeh
        cmp    eax, 4
        ja     SHORT loc_2_53BC
        jne    SHORT loc_2_5359
        xor    eax, eax
loc_2_5359:
        call   sub_2_4DB1
        mov    ebx, 0
        je     SHORT loc_2_53BC
        push   eax
        push   edx
        call   sub_2_66BF
        add    esp, 8
loc_2_536F:
        call   sub_2_4DB1
        mov    ebx, 0
        je     SHORT loc_2_53BC
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_66A4
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        or     eax, eax
        jne    SHORT loc_2_5394
        mov    eax, 4
loc_2_5394:
        mov    word ptr [esi + 10h], ax
        db  083h, 03Dh, 04Ch, 002h, 000h, 000h, 001h
        jne    SHORT loc_2_53BA
        dec    eax
        shl    eax, 4
        db  066h, 003h, 005h, 054h, 002h, 000h, 000h
        mov    word ptr [esi + 4], ax
        db  066h, 0A1h, 056h, 002h, 000h, 000h
        mov    word ptr [esi + 38h], ax
loc_2_53BA:
        xor    ebx, ebx
loc_2_53BC:
        mov    word ptr [esi + 1ch], bx
        clc
        leave
        ret
        db  000h, 020h, 010h, 030h, 008h, 028h, 018h, 038h, 004h, 024h, 014h, 034h, 00Ch, 02Ch, 01Ch, 03Ch
sub_2_53D3:
        and    eax, 0fh
        db  00Fh, 0B6h, 080h, 0C3h, 053h, 000h, 000h
        ret
sub_2_53DE:
        or     eax, eax
        je     SHORT loc_2_53FC
        shl    eax, 1ah
        xor    ebx, ebx
        mov    ecx, 5
loc_2_53EC:
        inc    ebx
        shl    eax, 1
        jb     SHORT loc_2_53F3
        loop   loc_2_53EC
loc_2_53F3:
        mov    eax, ebx
        cmp    eax, 4
        jbe    SHORT loc_2_53FC
        xor    eax, eax
loc_2_53FC:
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -0ch
        movzx  ecx, word ptr [esi + 18h]
        jecxz  loc_2_546F
        movzx  eax, word ptr [esi + 18h]
        shr    eax, 8
        call   sub_2_53DE
        or     eax, eax
        je     SHORT loc_2_542C
        call   sub_2_4DB1
        je     SHORT loc_2_546F
        push   eax
        push   1
        push   edx
        call   sub_2_6981
        add    esp, 0ch
loc_2_542C:
        movzx  eax, word ptr [esi + 18h]
        xor    ah, ah
        call   sub_2_53DE
        or     eax, eax
        je     SHORT loc_2_544C
        call   sub_2_4DB1
        push   eax
        push   2
        push   edx
        call   sub_2_6981
        add    esp, 0ch
loc_2_544C:
        movsx  eax, word ptr [esi + 10h]
        cdq
        mov    ecx, 168h
        idiv   ecx
        or     edx, edx
        jns    SHORT loc_2_545E
        add    edx, ecx
loc_2_545E:
        mov    eax, edx
        call   sub_2_4DB1
        push   eax
        push   edx
        call   sub_2_6B7C
        add    esp, 8
loc_2_546F:
        call   sub_2_4DB1
        mov    eax, 0
        je     NEAR PTR loc_2_5521
        lea    eax, [ebp - 8]
        push   eax
        push   1
        push   edx
        call   sub_2_694F
        add    esp, 0ch
        push   dword ptr [ebp - 8]
        call   sub_2_A035
        add    esp, 4
        call   sub_2_53D3
        xor    ebx, ebx
        mov    bh, al
        call   sub_2_4DB1
        lea    eax, [ebp - 8]
        push   eax
        push   2
        push   edx
        call   sub_2_694F
        add    esp, 0ch
        push   dword ptr [ebp - 8]
        call   sub_2_A035
        add    esp, 4
        call   sub_2_53D3
        mov    bl, al
        mov    word ptr [esi + 18h], bx
        call   sub_2_4DB1
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_6B61
        add    esp, 8
        lea    eax, [ebp - 0ch]
        push   eax
        push   6
        call   sub_2_5780
        add    esp, 8
        mov    eax, dword ptr [ebp - 0ch]
        call   sub_2_53D3
        mov    ebx, eax
        push   6
        call   sub_2_57D0
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_5511
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 10h], ax
        mov    eax, ebx
        mov    ah, 1ah
        jmp    SHORT loc_2_5521
loc_2_5511:
        mov    word ptr [esi + 10h], bx
        mov    ebx, dword ptr [ebp - 4]
        mov    word ptr [esi + 14h], bx
        mov    eax, 0ffffffffh
loc_2_5521:
        mov    word ptr [esi + 1ch], ax
        clc
        leave
        ret
        db  066h, 0C7h, 046h, 01Ch, 097h, 0F4h, 0F8h, 0C3h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        movzx  eax, word ptr [esi + 18h]
        or     eax, eax
        jne    SHORT loc_2_5546
        mov    word ptr [esi + 18h], 154h
        jmp    SHORT loc_2_557A
loc_2_5546:
        xchg   edi, eax
        mov    ax, 3814h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_2_5580
        push   edi
        movzx  ecx, word ptr [esi + 18h]
        xor    eax, eax
        rep stosb
        pop    edi
        call   sub_2_4DB1
        lea    eax, [ebp - 4]
        push   eax
        push   edx
        call   sub_2_6B61
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [edi + 0bh], ax
loc_2_557A:
        xor    eax, eax
        mov    word ptr [esi + 1ch], ax
loc_2_5580:
        clc
        leave
        ret
        db  083h, 03Dh, 04Ch, 002h, 000h, 000h, 001h, 075h, 016h, 0A1h, 050h, 002h, 000h, 000h, 066h, 089h
        db  046h, 014h, 0C1h, 0E8h, 010h, 066h, 089h, 046h, 038h, 033h, 0C0h, 066h, 089h, 046h, 01Ch, 0F8h
        db  0C3h
sub_2_55A4:
        ret
        db  000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_455F
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_55D6
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 8]
        call   sub_2_55DD
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
loc_2_55D6:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
sub_2_55DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        lea    ecx, [edi + 28h]
        mov    ax, word ptr [esi + 8]
        mov    edx, dword ptr [ecx]
        shl    dx, 8
        xor    dx, ax
        and    dx, 700h
        xor    dx, ax
        mov    word ptr [esi + 8], dx
        mov    eax, dword ptr [edi + 74h]
        db  08Bh, 004h, 085h, 000h, 000h, 000h, 000h
        xor    ax, dx
        and    ax, 7
        xor    ax, dx
        mov    word ptr [esi + 8], ax
        movzx  ebx, word ptr [edi + 7ah]
        cmp    ebx, 8
        jne    SHORT loc_2_5630
        mov    ebx, 3
        jmp    SHORT loc_2_5634
loc_2_5630:
        jbe    SHORT loc_2_5634
        sub    ebx, ebx
loc_2_5634:
        mov    ax, word ptr [esi + 8]
        shl    bx, 3
        xor    bx, ax
        and    bx, 38h
        xor    bx, ax
        mov    word ptr [esi + 8], bx
        push   ecx
        call   sub_2_3F1C
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        shl    ax, 6
        xor    ax, bx
        and    ax, 40h
        xor    ax, bx
        mov    word ptr [esi + 8], ax
        and    word ptr [esi + 8], 7fffh
        cmp    dword ptr [edi + 30h], 1bh
        jne    SHORT loc_2_568F
        mov    ax, word ptr [esi + 8]
        or     ax, 8000h
        mov    word ptr [esi + 8], ax
        and    ax, 8fffh
        or     ax, 800h
        mov    word ptr [esi + 8], ax
loc_2_568F:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_2_676C
        add    esp, 8
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 0ch], ax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [esi + 0eh], ax
        mov    eax, dword ptr [edi + 5ch]
        mov    word ptr [esi + 0ah], ax
        mov    eax, dword ptr [edi + 68h]
        mov    word ptr [esi + 4], ax
        mov    ax, word ptr [esi + 8]
        and    ah, 7
        cmp    ah, 2
        jne    SHORT loc_2_56F8
        lea    eax, [ebp - 8]
        push   eax
        push   edi
        call   sub_2_6A59
        add    esp, 8
        mov    ecx, 64h
        mov    eax, dword ptr [ebp - 8]
        sub    edx, edx
        div    ecx
        mov    word ptr [esi], ax
        mov    eax, dword ptr [edi + 6ch]
        mov    word ptr [esi + 2], ax
        mov    eax, dword ptr [edi + 38h]
        mov    word ptr [esi + 0ah], ax
        mov    ax, word ptr [edi + 78h]
        mov    word ptr [esi + 6], ax
        jmp    SHORT loc_2_5709
loc_2_56F8:
        mov    word ptr [esi], 0
        mov    word ptr [esi + 2], 0
        mov    word ptr [esi + 6], 0
loc_2_5709:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_455F
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_5740
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 8]
        call   sub_2_5747
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
loc_2_5740:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
sub_2_5747:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [edx + 5ch]
        mov    byte ptr [ecx + 10h], al
        mov    al, byte ptr [edx + 28h]
        cmp    al, 2
        mov    byte ptr [ecx + 11h], al
        jne    SHORT loc_2_5768
        mov    al, byte ptr [edx + 38h]
        mov    byte ptr [ecx + 18h], al
        jmp    SHORT loc_2_576C
loc_2_5768:
        mov    byte ptr [ecx + 18h], 0
loc_2_576C:
        mov    al, byte ptr [edx + 30h]
        mov    byte ptr [ecx + 19h], al
        mov    eax, dword ptr [edx + 44h]
        mov    word ptr [ecx + 14h], ax
        sub    eax, eax
        pop    ebp
        ret
        db  000h, 000h, 000h
sub_2_5780:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_455F
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_57AE
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 8]
        call   sub_2_57B5
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
loc_2_57AE:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret
sub_2_57B5:
        push   ebp
        mov    ebp, esp
        push   0
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        mov    ecx, dword ptr [ebp + 0ch]
        add    esp, 0ch
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        ret
sub_2_57D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_455F
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_2_57FA
        push   dword ptr [ebp - 4]
        call   sub_2_5801
        add    esp, 4
        jmp    SHORT loc_2_57FD
loc_2_57FA:
        mov    eax, dword ptr [ebp - 0ch]
loc_2_57FD:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_5801:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 30h]
        call   sub_2_96AE
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 056h, 0A1h, 03Ch, 000h, 000h, 000h, 00Bh, 0C0h, 074h, 013h, 08Bh, 030h, 06Ah, 000h, 050h
        db  0E8h, 000h, 055h, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 0C6h, 00Bh, 0F6h, 075h, 0EDh, 0C7h, 005h
        db  03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 06Ah, 000h, 0FFh, 035h, 038h, 000h, 000h, 000h
        db  0E8h, 0E0h, 054h, 000h, 000h, 083h, 0C4h, 008h, 0C7h, 005h, 038h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0B8h, 001h, 000h, 000h, 000h, 05Eh, 0C3h
sub_2_585C:
        push   ebp
        mov    ebp, esp
        db  0A1h, 03Ch, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_5876
        mov    ecx, dword ptr [ebp + 8]
loc_2_586B:
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_2_5878
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_2_586B
loc_2_5876:
        sub    eax, eax
loc_2_5878:
        pop    ebp
        ret
sub_2_587A:
        push   ebp
        mov    ebp, esp
        db  0B8h, 050h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
loc_2_5885:
        cmp    dword ptr [eax], ecx
        je     SHORT loc_2_5898
        add    eax, 0ch
        db  03Dh, 080h, 000h, 000h, 000h
        jb     SHORT loc_2_5885
        db  0B8h, 080h, 000h, 000h, 000h
loc_2_5898:
        pop    ebp
        ret
sub_2_589A:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        db  0BEh, 098h, 000h, 000h, 000h
        lea    edi, [ebp - 18h]
        movsd
        movsb
        push   1
        push   28h
        call   sub_2_AD20
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    esi, eax
        push   28h
        push   eax
        call   sub_2_80EC
        add    esp, 8
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_2_58E3
        push   0
        push   eax
        call   sub_2_AD28
        add    esp, 8
        sub    eax, eax
        jmp    NEAR PTR loc_2_59BA
loc_2_58E3:
        call   sub_2_205F
        mov    dword ptr [ebp - 0ch], eax
        push   1
        push   10h
        call   sub_2_AD20
        add    esp, 8
        mov    dword ptr [ebp - 10h], eax
        mov    edi, eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        je     NEAR PTR loc_2_59AA
loc_2_5908:
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 4], eax
        push   dword ptr [esi]
        call   sub_2_587A
        add    esp, 4
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [edi + 0ch], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edi + 8], eax
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_5981
        mov    eax, dword ptr [esi]
        cmp    eax, 9
        jg     SHORT loc_2_5981
        add    al, 30h
        mov    byte ptr [ebp - 15h], al
        push   -1
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_2_AD08
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, -1
        je     SHORT loc_2_5981
        cmp    ebx, -2
        je     SHORT loc_2_5981
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0ah
        push   ebx
        call   sub_2_AD18
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_2_5978
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 10h
        mov    dword ptr [edi + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    dword ptr [edi + 8], eax
loc_2_5978:
        push   ebx
        call   sub_2_AD10
        add    esp, 4
loc_2_5981:
        add    esi, 4
        cmp    dword ptr [esi], 0
        je     SHORT loc_2_599B
        push   1
        push   10h
        call   sub_2_AD20
        add    esp, 8
        mov    dword ptr [edi], eax
        mov    edi, eax
        jmp    SHORT loc_2_59A1
loc_2_599B:
        mov    dword ptr [edi], 0
loc_2_59A1:
        cmp    dword ptr [esi], 0
        jne    NEAR PTR loc_2_5908
loc_2_59AA:
        push   0
        push   dword ptr [ebp - 8]
        call   sub_2_AD28
        add    esp, 8
        mov    eax, dword ptr [ebp - 10h]
loc_2_59BA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_59C1:
        push   esi
        push   1
        push   28h
        call   sub_2_AD20
        add    esp, 8
        mov    esi, eax
        push   28h
        push   esi
        call   sub_2_80BC
        add    esp, 8
        mov    eax, esi
        pop    esi
        ret
sub_2_59DF:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 10h]
        push   dword ptr [ebp + 14h]
        mov    dword ptr [eax], 4
        push   eax
        push   dword ptr [ebp + 0ch]
        push   3
        db  068h, 090h, 000h, 000h, 000h
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_AD58
        add    esp, 1ch
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        pop    ebp
        ret
sub_2_5A0F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 0ch]
        shl    ecx, 18h
        mov    eax, dword ptr [ebp + 10h]
        shl    eax, 10h
        or     ecx, eax
        or     ecx, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 4], ecx
        push   0
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   3
        db  068h, 090h, 000h, 000h, 000h
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_AD60
        add    esp, 1ch
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    esp, ebp
        pop    ebp
        ret
sub_2_5A50:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        db  0BEh, 0A0h, 000h, 000h, 000h
        lea    edi, [ebp - 0ch]
        movsd
        movsd
        movsw
        mov    eax, dword ptr [ebp + 10h]
        push   dword ptr [ebp + 14h]
        mov    dword ptr [eax], 1dh
        push   eax
        lea    eax, [ebp - 0ch]
        push   dword ptr [ebp + 0ch]
        push   1
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_2_AD58
        add    esp, 1ch
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_5A94:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   esi
        push   edi
        sub    esi, esi
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_5A50
        add    esp, 10h
        cmp    eax, esi
        jne    SHORT loc_2_5ACE
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        push   edi
        call   sub_2_5A50
        add    esp, 10h
        cmp    eax, esi
        je     SHORT loc_2_5B0F
loc_2_5ACE:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_5AE8
        push   1eh
        db  0FFh, 035h, 018h, 000h, 000h, 000h
        lea    eax, [ebp - 24h]
        push   eax
        call   sub_2_6E78
        add    esp, 0ch
loc_2_5AE8:
        db  0BFh, 018h, 000h, 000h, 000h
loc_2_5AED:
        push   dword ptr [edi]
        lea    eax, [ebp - 24h]
        push   eax
        call   sub_2_6EA0
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_5B0C
        add    edi, 8
        db  081h, 0FFh, 038h, 000h, 000h, 000h
        jb     SHORT loc_2_5AED
        jmp    SHORT loc_2_5B0F
loc_2_5B0C:
        mov    esi, dword ptr [edi + 4]
loc_2_5B0F:
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_5B17:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        sub    esi, esi
        push   2
        push   dword ptr [ebp + 8]
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_AD38
        add    esp, 0ch
        cmp    eax, esi
        jne    SHORT loc_2_5B80
        push   esi
        push   esi
        push   2
        push   dword ptr [ebp - 8]
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_AD40
        add    esp, 14h
        cmp    eax, esi
        jne    SHORT loc_2_5B80
        mov    eax, dword ptr [ebp - 4]
        push   esi
        movzx  ecx, word ptr [eax + 4]
        mov    eax, dword ptr [ebp + 0ch]
        push   esi
        mov    dword ptr [eax], ecx
        lea    eax, [ebp - 4]
        push   4
        push   dword ptr [ebp - 8]
        push   eax
        call   sub_2_AD40
        add    esp, 14h
        cmp    eax, esi
        jne    SHORT loc_2_5B80
        mov    eax, dword ptr [ebp - 4]
        mov    esi, 1
        movzx  ecx, word ptr [eax + 2]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], ecx
loc_2_5B80:
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_5B87:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], ebx
        mov    dword ptr [esi], ebx
        lea    eax, [ebp - 14h]
        push   ebx
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        call   sub_2_59DF
        add    esp, 10h
        cmp    eax, ebx
        jne    SHORT loc_2_5BCA
        push   1
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   edi
        call   sub_2_59DF
        add    esp, 10h
        cmp    eax, ebx
        je     SHORT loc_2_5BFF
loc_2_5BCA:
        mov    eax, dword ptr [ebp - 8]
        shr    eax, 18h
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0f0000h
        shr    eax, 10h
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    dword ptr [esi + 4], eax
        push   eax
        call   sub_2_21A7
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_5BFA
        mov    dword ptr [esi], eax
loc_2_5BFA:
        mov    ebx, 1
loc_2_5BFF:
        or     ebx, ebx
        jne    NEAR PTR loc_2_5CCF
        lea    eax, [esi + 8]
        push   eax
        lea    eax, [esi + 4]
        push   eax
        push   edi
        call   sub_2_5B17
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_2_5CD3
        push   0
        push   0
        push   edi
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_AD48
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_2_5CBC
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_5CBC
        push   1
        push   dword ptr [ebp - 0ch]
        call   sub_2_AD20
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_2_5CBC
        push   0
        push   dword ptr [ebp - 0ch]
        push   eax
        push   0
        push   edi
        call   sub_2_AD50
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_2_5CBC
        push   5ch
        push   dword ptr [ebp - 4]
        call   sub_2_7030
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_2_5CBC
        inc    edi
        mov    eax, edi
loc_2_5C80:
        push   26h
        push   eax
        call   sub_2_7000
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_5C94
        mov    byte ptr [eax], 5ch
        jmp    SHORT loc_2_5C80
loc_2_5C94:
        push   0
        push   edi
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_2_AD30
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_2_5CBC
        lea    eax, [esi + 8]
        push   eax
        lea    eax, [esi + 4]
        push   eax
        push   dword ptr [ebp - 10h]
        call   sub_2_5B17
        add    esp, 0ch
        mov    ebx, eax
loc_2_5CBC:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_5CCF
        push   0
        push   dword ptr [ebp - 4]
        call   sub_2_AD28
        add    esp, 8
loc_2_5CCF:
        or     ebx, ebx
        je     SHORT loc_2_5CEF
loc_2_5CD3:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_2_5CEF
        push   dword ptr [esi + 4]
        call   sub_2_21A7
        add    esp, 4
        mov    dword ptr [esi], eax
        or     eax, eax
        jne    SHORT loc_2_5CEF
        mov    dword ptr [esi], 4
loc_2_5CEF:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_5CF8:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, word ptr [esi]
        push   eax
        call   sub_2_585C
        add    esp, 4
        or     eax, eax
        je     SHORT loc_2_5D5E
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [eax + 0ch], ecx
        jne    SHORT loc_2_5D2E
        mov    edx, dword ptr [esi + 4]
        cmp    dword ptr [eax + 8], edx
        jne    SHORT loc_2_5D2E
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    eax, 1
        jmp    SHORT loc_2_5D60
loc_2_5D2E:
        or     ecx, ecx
        je     SHORT loc_2_5D4F
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_2_5D4F
        mov    dword ptr [eax + 0ch], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    eax, 1
        jmp    SHORT loc_2_5D60
loc_2_5D4F:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    eax, 1
        jmp    SHORT loc_2_5D60
loc_2_5D5E:
        sub    eax, eax
loc_2_5D60:
        pop    esi
        pop    ebp
        ret
sub_2_5D63:
        push   ebp
        mov    ebp, esp
        push   esi
        db  08Bh, 035h, 03Ch, 000h, 000h, 000h
        cmp    dword ptr [esi], 0
        je     SHORT loc_2_5D79
loc_2_5D72:
        mov    esi, dword ptr [esi]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_2_5D72
loc_2_5D79:
        push   1
        push   10h
        call   sub_2_AD20
        add    esp, 8
        mov    dword ptr [esi], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [edx + 0ch], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax], ecx
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
sub_2_5DB2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   dword ptr [ebp + 8]
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_5B87
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_5DFC
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_5DFC
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_5CF8
        add    esp, 8
        or     eax, eax
        je     SHORT loc_2_5DED
        mov    eax, 1
        jmp    SHORT loc_2_5DFE
loc_2_5DED:
        push   esi
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_5D63
        add    esp, 8
        jmp    SHORT loc_2_5DFE
loc_2_5DFC:
        sub    eax, eax
loc_2_5DFE:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_5E03:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0ch], 2
        jne    SHORT loc_2_5E2A
        add    eax, 10h
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   dword ptr [eax + 0ch]
        call   sub_2_5DB2
        add    esp, 8
        or     eax, eax
        mov    eax, 0
        je     SHORT loc_2_5E2F
loc_2_5E2A:
        mov    eax, 1
loc_2_5E2F:
        pop    ebp
        ret
sub_2_5E31:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 1
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 14h], edi
        and    dword ptr [esi + 8], 0ffffffefh
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    ebx, 1
        je     SHORT loc_2_5ECE
        or     ebx, ebx
        jne    SHORT loc_2_5E5E
        or     edi, edi
        je     SHORT loc_2_5ECE
loc_2_5E5E:
        push   dword ptr [edi + 0ch]
        call   sub_2_5A94
        add    esp, 4
        mov    dword ptr [esi + 0ch], eax
        or     eax, eax
        je     SHORT loc_2_5EC5
        mov    eax, dword ptr [esi + 8]
        or     eax, 1
        mov    dword ptr [esi + 8], eax
        or     eax, 2
        mov    dword ptr [esi + 8], eax
        push   edi
        push   esi
        call   sub_2_5E03
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        jne    SHORT loc_2_5EFE
        cmp    dword ptr [esi + 0ch], 2
        jne    SHORT loc_2_5EFE
        mov    eax, dword ptr [esi + 8]
        and    eax, 0fffffffeh
        mov    dword ptr [esi + 8], eax
        and    eax, 0fffffffdh
        mov    dword ptr [esi + 8], eax
        or     eax, 10h
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp - 4], 1
        mov    dword ptr [eax], 2
        mov    eax, dword ptr [esi]
        mov    dword ptr [eax + 4], 0
        jmp    SHORT loc_2_5EFE
loc_2_5EC5:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_2_5EFE
loc_2_5ECE:
        call   sub_2_8134
        mov    dword ptr [esi + 0ch], eax
        call   sub_2_8181
        mov    dword ptr [esi + 10h], eax
        mov    ecx, dword ptr [esi + 8]
        and    ecx, 0fffffffeh
        mov    dword ptr [esi + 8], ecx
        dec    ebx
        cmp    ebx, 1
        sbb    eax, eax
        lea    eax, [eax*2 + 2]
        xor    eax, ecx
        and    eax, 2
        xor    eax, ecx
        mov    dword ptr [esi + 8], eax
loc_2_5EFE:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_5F08:
        db  083h, 03Dh, 038h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_5F1B
        call   sub_2_59C1
        db  0A3h, 038h, 000h, 000h, 000h
loc_2_5F1B:
        db  083h, 03Dh, 03Ch, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_5F2E
        call   sub_2_589A
        db  0A3h, 03Ch, 000h, 000h, 000h
loc_2_5F2E:
        call   sub_2_7FD0
        shl    eax, 2
        db  033h, 005h, 040h, 000h, 000h, 000h
        and    eax, 4
        db  031h, 005h, 040h, 000h, 000h, 000h
        db  083h, 025h, 040h, 000h, 000h, 000h, 0F7h
        db  0B8h, 038h, 000h, 000h, 000h
        ret
        db  000h, 000h
loc_2_5F54:
        and    word ptr [ebp + 2ch], 0fffeh
        movzx  eax, word ptr [ebp + 1ch]
        cmp    eax, 1
        ja     SHORT loc_2_5F76
        je     SHORT loc_2_5F6B
        mov    word ptr [ebp + 1ch], 62ah
        ret
loc_2_5F6B:
        mov    ax, word ptr [ebp + 10h]
        db  066h, 0A3h, 049h, 005h, 000h, 000h
        ret
loc_2_5F76:
        cmp    ah, 4ch
        jne    SHORT loc_2_5FAC
        movzx  eax, al
        cmp    al, 0bh
        jae    SHORT loc_2_5F94
        db  066h, 08Bh, 004h, 045h, 0DAh, 004h, 000h, 000h
        movsx  eax, ax
        db  005h, 024h, 061h, 000h, 000h
        jmp    eax
loc_2_5F94:
        sub    al, 50h
        cmp    al, 28h
        jae    SHORT loc_2_5FBF
        db  066h, 08Bh, 004h, 045h, 0F0h, 004h, 000h, 000h
        movsx  eax, ax
        db  005h, 024h, 061h, 000h, 000h
        jmp    eax
loc_2_5FAC:
        cmp    ah, 4fh
        jne    SHORT loc_2_5FBF
        cmp    al, 2
        jae    SHORT loc_2_5FBF
        movzx  eax, al
        db  0FFh, 024h, 085h, 040h, 005h, 000h, 000h
loc_2_5FBF:
        or     word ptr [ebp + 2ch], 1
        ret
sub_2_5FC5:
        movzx  eax, al
        movzx  ecx, cl
        mov    edx, esp
        mov    esi, dword ptr [esp]
        sub    esp, eax
        mov    dword ptr [esp], esi
        mov    esi, edx
        sub    edx, ecx
        mov    dword ptr [edx + 4], edi
        db  066h, 083h, 03Dh, 049h, 005h, 000h, 000h, 000h
        je     SHORT loc_2_5FEC
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
loc_2_5FEC:
        xchg   edi, eax
        mov    ax, 3430h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        cmp    edi, -1
        je     SHORT loc_2_6061
        mov    dword ptr [edx + 8], edi
        ret
sub_2_6001:
        mov    eax, dword ptr [edi]
        add    edi, 2
        movzx  eax, ax
        mov    dword ptr [edx], eax
        sub    edx, 4
        ret
        db  08Bh, 007h, 083h, 0C7h, 002h, 00Fh, 0BFh, 0C0h, 089h, 002h, 083h, 0EAh, 004h, 0C3h, 08Bh, 007h
        db  083h, 0C7h, 004h, 089h, 002h, 083h, 0EAh, 004h, 0C3h, 089h, 032h, 083h, 0EAh, 004h, 083h, 0EEh
        db  004h, 08Bh, 007h, 083h, 0C7h, 004h, 0E8h, 035h, 000h, 000h, 000h, 089h, 006h, 083h, 0EEh, 004h
        db  0C3h, 08Bh, 007h, 083h, 0C7h, 004h, 0E8h, 025h, 000h, 000h, 000h, 089h, 002h, 083h, 0EAh, 004h
        db  0C3h, 083h, 0C6h, 008h, 08Bh, 07Eh, 0FCh, 08Bh, 006h, 066h, 089h, 007h, 0C3h
sub_2_605C:
        mov    word ptr [ebp + 1ch], ax
        ret
loc_2_6061:
        pushad
        db  0BEh, 0BCh, 004h, 000h, 000h
        xor    eax, eax
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
        push   esi
        mov    esi, eax
        shr    eax, 10h
        test   dword ptr [ebx], 20h
        jne    SHORT loc_2_608A
        shl    eax, 4
        movzx  esi, si
        add    eax, esi
loc_2_6085:
        add    eax, dword ptr [ebx + 4]
        pop    esi
        ret
loc_2_608A:
        test   dword ptr [ebx], 180h
        jne    SHORT loc_2_6097
        movzx  esi, si
        jmp    SHORT loc_2_609B
loc_2_6097:
        mov    ax, word ptr [ebp + 3ch]
loc_2_609B:
        push   edx
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        cmp    eax, -1
        je     SHORT loc_2_6061
        add    eax, esi
        cmp    eax, 110000h
        jb     SHORT loc_2_6085
        pop    esi
        ret
sub_2_60BA:
        sub    edi, dword ptr [esi]
        add    dword ptr [ebp + 30h], edi
        db  0FEh, 005h, 048h, 005h, 000h, 000h
        db  080h, 03Dh, 048h, 005h, 000h, 000h, 001h
        mov    eax, 0ffffffech
        ret
sub_2_60D2:
        db  0FEh, 00Dh, 048h, 005h, 000h, 000h
        call   sub_2_605C
        ret
        push   ebp
        mov    ebp, esp
        add    esp, -8
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_455F
        add    esp, 0ch
        or     eax, eax
        mov    eax, dword ptr [ebp - 8]
        je     SHORT loc_2_6106
        lea    eax, [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
loc_2_6106:
        leave
        ret
loc_2_6108:
        mov    al, 8
        mov    cl, 8
        call   sub_2_5FC5
        call   sub_2_60BA
        jne    SHORT loc_2_611B
        call   dword ptr [esi - 4]
loc_2_611B:
        call   sub_2_60D2
        add    esp, 8
        ret
loc_2_6124:
        db  0BFh, 070h, 065h, 000h, 000h
        jmp    SHORT loc_2_6108
        db  0BFh, 09Bh, 065h, 000h, 000h, 0EBh, 0D6h, 0B0h, 014h, 0B1h, 010h, 0E8h, 08Ah, 0FEh, 0FFh, 0FFh
        db  0E8h, 0E8h, 0FEh, 0FFh, 0FFh, 0E8h, 075h, 0FFh, 0FFh, 0FFh, 075h, 003h, 0FFh, 056h, 0FCh, 0E8h
        db  083h, 0FFh, 0FFh, 0FFh, 0E8h, 0FCh, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 014h, 0C3h, 0BFh, 0A9h, 065h
        db  000h, 000h, 0EBh, 0D3h, 0B0h, 010h, 0B1h, 008h, 0E8h, 05Dh, 0FEh, 0FFh, 0FFh, 0E8h, 094h, 0FEh
        db  0FFh, 0FFh, 0E8h, 0CEh, 0FEh, 0FFh, 0FFh, 0E8h, 043h, 0FFh, 0FFh, 0FFh, 075h, 003h, 0FFh, 056h
        db  0FCh, 0E8h, 051h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 010h, 0C3h, 0BFh, 0B9h, 065h, 000h, 000h, 0EBh
        db  0D3h, 0BFh, 0E2h, 065h, 000h, 000h, 0EBh, 09Fh, 0BFh, 0F2h, 065h, 000h, 000h, 0EBh, 098h, 0BFh
        db  002h, 066h, 000h, 000h, 0EBh, 0BEh, 0BFh, 014h, 066h, 000h, 000h, 0EBh, 08Ah, 0BFh, 023h, 066h
        db  000h, 000h, 0EBh, 083h, 0BFh, 033h, 066h, 000h, 000h, 0EBh, 0A9h, 0BFh, 045h, 066h, 000h, 000h
        db  0E9h, 072h, 0FFh, 0FFh, 0FFh, 0B0h, 010h, 0B1h, 008h, 0E8h, 0FCh, 0FDh, 0FFh, 0FFh, 0E8h, 072h
        db  0FEh, 0FFh, 0FFh, 0E8h, 02Eh, 0FEh, 0FFh, 0FFh, 0E8h, 0E2h, 0FEh, 0FFh, 0FFh, 075h, 00Ah, 0E8h
        db  0FFh, 0FEh, 0FFh, 0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 0E9h, 0FEh, 0FFh, 0FFh, 083h, 0C4h
        db  010h, 0C3h, 0BFh, 058h, 066h, 000h, 000h, 0EBh, 0CCh, 0B0h, 018h, 0B1h, 010h, 0E8h, 0C8h, 0FDh
        db  0FFh, 0FFh, 0E8h, 026h, 0FEh, 0FFh, 0FFh, 0E8h, 0FAh, 0FDh, 0FFh, 0FFh, 0E8h, 0AEh, 0FEh, 0FFh
        db  0FFh, 075h, 00Ah, 0E8h, 0CBh, 0FEh, 0FFh, 0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 0B5h, 0FEh
        db  0FFh, 0FFh, 0E8h, 02Eh, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 018h, 0C3h, 0BFh, 092h, 066h, 000h, 000h
        db  0EBh, 0C7h, 0BFh, 0A4h, 066h, 000h, 000h, 0EBh, 0C0h, 0B0h, 010h, 0B1h, 008h, 0E8h, 088h, 0FDh
        db  0FFh, 0FFh, 0E8h, 0BFh, 0FDh, 0FFh, 0FFh, 0E8h, 0BAh, 0FDh, 0FFh, 0FFh, 0E8h, 06Eh, 0FEh, 0FFh
        db  0FFh, 075h, 00Ah, 0E8h, 08Bh, 0FEh, 0FFh, 0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 075h, 0FEh
        db  0FFh, 0FFh, 083h, 0C4h, 010h, 0C3h, 0BFh, 0BFh, 066h, 000h, 000h, 0EBh, 0CCh, 0B0h, 024h, 0B1h
        db  018h, 0E8h, 054h, 0FDh, 0FFh, 0FFh, 0E8h, 0B2h, 0FDh, 0FFh, 0FFh, 0E8h, 0ADh, 0FDh, 0FFh, 0FFh
        db  0E8h, 081h, 0FDh, 0FFh, 0FFh, 0E8h, 035h, 0FEh, 0FFh, 0FFh, 075h, 00Ah, 0E8h, 052h, 0FEh, 0FFh
        db  0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 03Ch, 0FEh, 0FFh, 0FFh, 0E8h, 0B5h, 0FDh, 0FFh, 0FFh
        db  0E8h, 0B0h, 0FDh, 0FFh, 0FFh, 083h, 0C4h, 024h, 0C3h, 0BFh, 0F6h, 066h, 000h, 000h, 0EBh, 0BDh
        db  0B0h, 014h, 0B1h, 008h, 0E8h, 011h, 0FDh, 0FFh, 0FFh, 0E8h, 056h, 0FDh, 0FFh, 0FFh, 0E8h, 051h
        db  0FDh, 0FFh, 0FFh, 0E8h, 03Eh, 0FDh, 0FFh, 0FFh, 0E8h, 0F2h, 0FDh, 0FFh, 0FFh, 075h, 00Ah, 0E8h
        db  00Fh, 0FEh, 0FFh, 0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 0F9h, 0FDh, 0FFh, 0FFh, 083h, 0C4h
        db  014h, 0C3h, 0BFh, 02Fh, 067h, 000h, 000h, 0EBh, 0C7h, 0BFh, 06Ch, 067h, 000h, 000h, 0E9h, 006h
        db  0FFh, 0FFh, 0FFh, 0BFh, 07Eh, 067h, 000h, 000h, 0E9h, 0FCh, 0FEh, 0FFh, 0FFh, 0BFh, 090h, 067h
        db  000h, 000h, 0E9h, 032h, 0FFh, 0FFh, 0FFh, 0B0h, 01Ch, 0B1h, 010h, 0E8h, 0BAh, 0FCh, 0FFh, 0FFh
        db  0E8h, 018h, 0FDh, 0FFh, 0FFh, 0E8h, 0ECh, 0FCh, 0FFh, 0FFh, 0E8h, 0E7h, 0FCh, 0FFh, 0FFh, 0E8h
        db  09Bh, 0FDh, 0FFh, 0FFh, 075h, 00Ah, 0E8h, 0B8h, 0FDh, 0FFh, 0FFh, 074h, 003h, 0FFh, 056h, 0FCh
        db  0E8h, 0A2h, 0FDh, 0FFh, 0FFh, 0E8h, 01Bh, 0FDh, 0FFh, 0FFh, 083h, 0C4h, 01Ch, 0C3h, 0BFh, 0A2h
        db  067h, 000h, 000h, 0EBh, 0C2h, 0B0h, 014h, 0B1h, 008h, 0E8h, 07Ch, 0FCh, 0FFh, 0FFh, 0E8h, 0C1h
        db  0FCh, 0FFh, 0FFh, 0E8h, 0AEh, 0FCh, 0FFh, 0FFh, 0E8h, 0A9h, 0FCh, 0FFh, 0FFh, 0E8h, 05Dh, 0FDh
        db  0FFh, 0FFh, 075h, 00Ah, 0E8h, 07Ah, 0FDh, 0FFh, 0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 064h
        db  0FDh, 0FFh, 0FFh, 083h, 0C4h, 014h, 0C3h, 0BFh, 0F4h, 067h, 000h, 000h, 0EBh, 0C7h, 0B0h, 018h
        db  0B1h, 010h, 0E8h, 043h, 0FCh, 0FFh, 0FFh, 0E8h, 0A1h, 0FCh, 0FFh, 0FFh, 0E8h, 075h, 0FCh, 0FFh
        db  0FFh, 0E8h, 029h, 0FDh, 0FFh, 0FFh, 075h, 00Ah, 0E8h, 046h, 0FDh, 0FFh, 0FFh, 074h, 003h, 0FFh
        db  056h, 0FCh, 0E8h, 030h, 0FDh, 0FFh, 0FFh, 0E8h, 0A9h, 0FCh, 0FFh, 0FFh, 083h, 0C4h, 018h, 0C3h
        db  0BFh, 060h, 068h, 000h, 000h, 0EBh, 0C7h, 0B0h, 010h, 0B1h, 008h, 0E8h, 00Ah, 0FCh, 0FFh, 0FFh
        db  0E8h, 04Fh, 0FCh, 0FFh, 0FFh, 0E8h, 03Ch, 0FCh, 0FFh, 0FFh, 0E8h, 0F0h, 0FCh, 0FFh, 0FFh, 075h
        db  00Ah, 0E8h, 00Dh, 0FDh, 0FFh, 0FFh, 074h, 003h, 0FFh, 056h, 0FCh, 0E8h, 0F7h, 0FCh, 0FFh, 0FFh
        db  083h, 0C4h, 010h, 0C3h, 0BFh, 0C6h, 068h, 000h, 000h, 0EBh, 0CCh, 0B0h, 024h, 0B1h, 018h, 0E8h
        db  0D6h, 0FBh, 0FFh, 0FFh, 0E8h, 034h, 0FCh, 0FFh, 0FFh, 0E8h, 02Fh, 0FCh, 0FFh, 0FFh, 0E8h, 003h
        db  0FCh, 0FFh, 0FFh, 0E8h, 0B7h, 0FCh, 0FFh, 0FFh, 075h, 00Ah, 0E8h, 0D4h, 0FCh, 0FFh, 0FFh, 074h
        db  003h, 0FFh, 056h, 0FCh, 0E8h, 0BEh, 0FCh, 0FFh, 0FFh, 0E8h, 037h, 0FCh, 0FFh, 0FFh, 0E8h, 032h
        db  0FCh, 0FFh, 0FFh, 083h, 0C4h, 024h, 0C3h, 0BFh, 01Dh, 069h, 000h, 000h, 0EBh, 0BDh, 0BFh, 04Fh
        db  069h, 000h, 000h, 0E9h, 0CFh, 0FEh, 0FFh, 0FFh, 0BFh, 081h, 069h, 000h, 000h, 0E9h, 003h, 0FFh
        db  0FFh, 0FFh, 0BFh, 0AEh, 069h, 000h, 000h, 0E9h, 0BBh, 0FEh, 0FFh, 0FFh, 0BFh, 0EDh, 069h, 000h
        db  000h, 0E9h, 0B1h, 0FEh, 0FFh, 0FFh, 0BFh, 059h, 06Ah, 000h, 000h, 0E9h, 099h, 0FDh, 0FFh, 0FFh
        db  0BFh, 078h, 06Ah, 000h, 000h, 0E9h, 0CFh, 0FDh, 0FFh, 0FFh, 0BFh, 061h, 06Bh, 000h, 000h, 0E9h
        db  085h, 0FDh, 0FFh, 0FFh, 0BFh, 07Ch, 06Bh, 000h, 000h, 0E9h, 0BBh, 0FDh, 0FFh, 0FFh, 0BFh, 0B3h
        db  06Bh, 000h, 000h, 0E9h, 071h, 0FDh, 0FFh, 0FFh, 0BFh, 0E4h, 06Bh, 000h, 000h, 0E9h, 067h, 0FDh
        db  0FFh, 0FFh, 0BFh, 014h, 06Ch, 000h, 000h, 0E9h, 05Dh, 0FDh, 0FFh, 0FFh, 0BFh, 044h, 06Ch, 000h
        db  000h, 0E9h, 061h, 0FEh, 0FFh, 0FFh, 0BFh, 089h, 06Ch, 000h, 000h, 0E9h, 0CEh, 0FEh, 0FFh, 0FFh
        db  0BFh, 0B8h, 06Ch, 000h, 000h, 0E9h, 03Fh, 0FDh, 0FFh, 0FFh, 0BFh, 064h, 06Eh, 000h, 000h, 0E9h
        db  035h, 0FDh, 0FFh, 0FFh, 0BFh, 06Ah, 06Eh, 000h, 000h, 0E9h, 02Bh, 0FDh, 0FFh, 0FFh, 0BFh, 070h
        db  06Eh, 000h, 000h, 0E9h, 021h, 0FDh, 0FFh, 0FFh, 0BFh, 0BEh, 06Ch, 000h, 000h, 0E9h, 017h, 0FDh
        db  0FFh, 0FFh, 0BFh, 0D9h, 06Ch, 000h, 000h, 0E9h, 04Dh, 0FDh, 0FFh, 0FFh, 0BFh, 010h, 06Dh, 000h
        db  000h, 0E9h, 011h, 0FEh, 0FFh, 0FFh, 0BFh, 062h, 06Dh, 000h, 000h, 0E9h, 045h, 0FEh, 0FFh, 0FFh
        db  0BFh, 0CEh, 06Dh, 000h, 000h, 0E9h, 0FDh, 0FDh, 0FFh, 0FFh, 0BFh, 0D4h, 06Dh, 000h, 000h, 0E9h
        db  031h, 0FEh, 0FFh, 0FFh, 0BFh, 0DAh, 06Dh, 000h, 000h, 0E9h, 0DBh, 0FCh, 0FFh, 0FFh, 0BFh, 0F5h
        db  06Dh, 000h, 000h, 0E9h, 011h, 0FDh, 0FFh, 0FFh, 0BFh, 02Ch, 06Eh, 000h, 000h, 0E9h, 007h, 0FDh
        db  0FFh, 0FFh
loc_2_652D:
        mov    al, 0ch
        mov    cl, 8
        call   sub_2_5FC5
        call   sub_2_6001
        call   sub_2_60BA
        jne    SHORT loc_2_6545
        call   dword ptr [esi - 4]
loc_2_6545:
        call   sub_2_60D2
        add    esp, 0ch
        ret
loc_2_654E:
        db  0BFh, 0EBh, 0A7h, 000h, 000h
        jmp    SHORT loc_2_652D
loc_2_6555:
        db  0BFh, 0D7h, 0A6h, 000h, 000h
        jmp    SHORT loc_2_652D
sub_2_655C:
        call   sub_2_468E
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_6566:
        push   eax
        call   sub_2_42E8
        add    esp, 4
        ret
loc_2_6570:
        db  083h, 03Dh, 004h, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_6588
        call   sub_2_95DD
        push   1
        call   sub_2_9885
        add    esp, 4
loc_2_6588:
        call   sub_2_655C
        db  0A1h, 004h, 000h, 000h, 000h
        inc    eax
        db  0A3h, 004h, 000h, 000h, 000h
        sub    eax, eax
        ret
loc_2_659B:
        db  0A1h, 004h, 000h, 000h, 000h
        dec    eax
        db  0A3h, 004h, 000h, 000h, 000h
        sub    eax, eax
        ret
sub_2_65A9:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], 62ah
        sub    eax, eax
        ret
sub_2_65B9:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_6E78
        add    esp, 0ch
        or     edi, edi
        je     SHORT loc_2_65DC
        mov    byte ptr [esi + edi - 1], 0
loc_2_65DC:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_65E2:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], 0
        sub    eax, eax
        ret
loc_2_65F2:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], 62ah
        sub    eax, eax
        ret
loc_2_6602:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_65B9
        mov    esp, ebp
        pop    ebp
        ret
loc_2_6614:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        call   sub_2_65E2
        mov    esp, ebp
        pop    ebp
        ret
loc_2_6623:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [eax], 6
        sub    eax, eax
        ret
loc_2_6633:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_8933
        mov    esp, ebp
        pop    ebp
        ret
loc_2_6645:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [eax], ecx
        sub    eax, eax
        pop    ebp
        ret
        db  000h
sub_2_6658:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 28h]
        mov    word ptr [edx], ax
        mov    eax, dword ptr [ecx + 44h]
        mov    word ptr [edx + 2], ax
        mov    word ptr [edx + 4], 0
        mov    eax, dword ptr [ecx + 5ch]
        mov    word ptr [edx + 6], ax
        cmp    dword ptr [ecx + 28h], 2
        jne    SHORT loc_2_668E
        mov    eax, dword ptr [ecx + 38h]
        mov    word ptr [edx + 8], ax
        mov    word ptr [edx + 0ah], 0
loc_2_668E:
        sub    eax, eax
        pop    ebp
        ret
sub_2_6692:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 30h]
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    dword ptr [eax], ecx
        sub    eax, eax
        ret
sub_2_66A4:
        push   ebp
        mov    ebp, esp
        push   0
        push   8
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        mov    ecx, dword ptr [ebp + 0ch]
        add    esp, 0ch
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        ret
sub_2_66BF:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], edi
        lea    eax, [ebp - 4]
        push   eax
        push   8
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9825
        add    esp, 0ch
        push   edi
        push   dword ptr [esi + 2ch]
        push   dword ptr [esi + 34h]
        call   sub_2_7AFD
        add    esp, 0ch
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_66F6:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 10h]
        push   eax
        push   9
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
sub_2_672F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], esi
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 8]
        push   eax
        push   9
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_9825
        add    esp, 0ch
        push   esi
        push   dword ptr [edi + 2ch]
        push   dword ptr [edi + 34h]
        call   sub_2_7F7A
        add    esp, 0ch
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_676C:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 4ch]
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    dword ptr [eax], ecx
        sub    eax, eax
        ret
loc_2_677E:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_AB2D
        mov    esp, ebp
        pop    ebp
        ret
loc_2_6790:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_AB30
        mov    esp, ebp
        pop    ebp
        ret
sub_2_67A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, 0fffffffdh
        lea    ecx, [esi - 1]
        cmp    ecx, 20h
        jae    SHORT loc_2_67EF
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 10h]
        push   eax
        push   4
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        add    esp, 0ch
        push   esi
        call   sub_2_A035
        add    esp, 4
        and    eax, dword ptr [ebp - 4]
        cmp    eax, 1
        sbb    ecx, ecx
        inc    ecx
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], ecx
        sub    eax, eax
loc_2_67EF:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_67F4:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, 0fffffffdh
        lea    ecx, [esi - 1]
        cmp    ecx, 20h
        jae    SHORT loc_2_685A
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 10h]
        push   eax
        push   4
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_9847
        add    esp, 0ch
        cmp    dword ptr [ebp + 10h], 0
        push   esi
        je     SHORT loc_2_683E
        call   sub_2_A035
        add    esp, 4
        or     dword ptr [ebp - 4], eax
        jmp    SHORT loc_2_684B
loc_2_683E:
        call   sub_2_A035
        add    esp, 4
        not    eax
        and    dword ptr [ebp - 4], eax
loc_2_684B:
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   edi
        call   sub_2_A531
        add    esp, 0ch
loc_2_685A:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_6860:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_691D
        add    esp, 0ch
        lea    eax, [ebp - 10h]
        push   eax
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_2_694F
        add    esp, 0ch
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp - 8]
        push   esi
        call   sub_2_694F
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 10h], eax
        jne    SHORT loc_2_68B6
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 0ch], eax
        jne    SHORT loc_2_68B6
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 1
        jmp    SHORT loc_2_68BF
loc_2_68B6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], 0
loc_2_68BF:
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_68C6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_691D
        add    esp, 0ch
        mov    edi, dword ptr [ebp + 0ch]
        or     edi, edi
        je     SHORT loc_2_68EE
        mov    eax, dword ptr [ebp - 4]
        jmp    SHORT loc_2_68F1
loc_2_68EE:
        mov    eax, dword ptr [ebp - 8]
loc_2_68F1:
        push   eax
        push   dword ptr [ebp - 8]
        push   esi
        call   sub_2_6981
        add    esp, 0ch
        or     edi, edi
        je     SHORT loc_2_6907
        mov    eax, dword ptr [ebp - 8]
        jmp    SHORT loc_2_690A
loc_2_6907:
        mov    eax, dword ptr [ebp - 4]
loc_2_690A:
        push   eax
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_2_6981
        add    esp, 0ch
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_691D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 30h]
        call   sub_2_95E8
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        mov    esp, ebp
        pop    ebp
        ret
sub_2_694F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_A46C
        add    esp, 0ch
        mov    esi, eax
        push   dword ptr [ebp - 4]
        call   sub_2_A020
        mov    ecx, dword ptr [ebp + 10h]
        add    esp, 4
        mov    dword ptr [ecx], eax
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_6981:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, 0fffffffdh
        lea    edx, [ecx - 1]
        cmp    edx, 20h
        jae    SHORT loc_2_69AC
        push   ecx
        call   sub_2_A035
        add    esp, 4
        push   eax
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_A494
        add    esp, 0ch
loc_2_69AC:
        pop    ebp
        ret
sub_2_69AE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        sub    esi, esi
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 4ch], ecx
        jae    SHORT loc_2_69CE
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], esi
        mov    esi, 0fffffffdh
        jmp    SHORT loc_2_69E6
loc_2_69CE:
        lea    edx, [ebp - 4]
        push   edx
        push   ecx
        push   dword ptr [eax + 30h]
        call   sub_2_9612
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
loc_2_69E6:
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_69ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, 0fffffffdh
        lea    ecx, [edi - 1]
        cmp    ecx, 20h
        jae    SHORT loc_2_6A52
        mov    ebx, 1
        mov    esi, dword ptr [ebp + 8]
loc_2_6A0E:
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   esi
        call   sub_2_694F
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], edi
        je     SHORT loc_2_6A27
        inc    ebx
        cmp    ebx, 20h
        jbe    SHORT loc_2_6A0E
loc_2_6A27:
        cmp    ebx, 4
        ja     SHORT loc_2_6A44
        lea    eax, [ebp - 8]
        push   eax
        push   ebx
        push   esi
        call   sub_2_69AE
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], edx
        jmp    SHORT loc_2_6A52
loc_2_6A44:
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, 0fffffff6h
        mov    dword ptr [ecx], 0
loc_2_6A52:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_6A59:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    eax, eax
        cmp    dword ptr [ecx + 28h], 2
        jne    SHORT loc_2_6A71
        mov    edx, dword ptr [ecx + 3ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], edx
        jmp    SHORT loc_2_6A76
loc_2_6A71:
        mov    eax, 0fffffff5h
loc_2_6A76:
        pop    ebp
        ret
sub_2_6A78:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 4b0h
        cmp    dword ptr [esi + 28h], 2
        jne    NEAR PTR loc_2_6B39
        mov    eax, dword ptr [esi + 74h]
        cmp    eax, 1
        jb     SHORT loc_2_6AA7
        cmp    eax, 2
        jbe    SHORT loc_2_6AF9
        cmp    eax, 6
        je     SHORT loc_2_6AF9
        cmp    eax, 7
        je     SHORT loc_2_6B18
loc_2_6AA7:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 4b0h
        je     SHORT loc_2_6AC6
        cmp    eax, 2580h
        je     SHORT loc_2_6AC6
        mov    dword ptr [esi + 3ch], 4b0h
        mov    ebx, 0ffffffeeh
        jmp    SHORT loc_2_6B3E
loc_2_6AC6:
        cmp    dword ptr [esi + 30h], 17h
        je     SHORT loc_2_6AEB
        cmp    dword ptr [esi + 30h], 13h
        jne    SHORT loc_2_6AD8
        cmp    dword ptr [esi + 74h], 4
        je     SHORT loc_2_6AEB
loc_2_6AD8:
        cmp    dword ptr [esi + 30h], 0bh
        je     SHORT loc_2_6AEB
        cmp    dword ptr [esi + 30h], 29h
        je     SHORT loc_2_6AEB
        mov    dword ptr [esi + 3ch], eax
        sub    ebx, ebx
        jmp    SHORT loc_2_6B3E
loc_2_6AEB:
        mov    dword ptr [esi + 3ch], 4b0h
        mov    ebx, 0ffffffeeh
        jmp    SHORT loc_2_6B3E
loc_2_6AF9:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 4b0h
        jne    SHORT loc_2_6B0A
        mov    dword ptr [esi + 3ch], eax
        sub    ebx, ebx
        jmp    SHORT loc_2_6B3E
loc_2_6B0A:
        mov    dword ptr [esi + 3ch], 4b0h
        mov    ebx, 0ffffffeeh
        jmp    SHORT loc_2_6B3E
loc_2_6B18:
        mov    edi, 960h
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, edi
        jne    SHORT loc_2_6B2B
        mov    dword ptr [esi + 3ch], eax
        sub    ebx, ebx
        jmp    SHORT loc_2_6B3E
loc_2_6B2B:
        mov    dword ptr [esi + 3ch], 960h
        mov    ebx, 0ffffffeeh
        jmp    SHORT loc_2_6B3E
loc_2_6B39:
        mov    ebx, 0fffffff5h
loc_2_6B3E:
        or     ebx, ebx
        jne    SHORT loc_2_6B5A
        push   dword ptr [esi + 3ch]
        push   esi
        call   sub_2_4630
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_6B5A
        mov    dword ptr [esi + 3ch], edi
        mov    ebx, 0ffffffeeh
loc_2_6B5A:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_6B61:
        push   ebp
        mov    ebp, esp
        push   0
        push   6
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        mov    ecx, dword ptr [ebp + 0ch]
        add    esp, 0ch
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        ret
sub_2_6B7C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], edi
        lea    eax, [ebp - 4]
        push   eax
        push   6
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9825
        add    esp, 0ch
        push   edi
        push   dword ptr [esi + 2ch]
        push   dword ptr [esi + 34h]
        call   sub_2_7F19
        add    esp, 0ch
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_6BB3:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_422E
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_2_6BD9
        movzx  ecx, word ptr [edi + 8fh]
        mov    eax, dword ptr [ebp + 0ch]
        inc    ecx
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_2_6BDE
loc_2_6BD9:
        mov    esi, 0fffffff4h
loc_2_6BDE:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_6BE4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_4201
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_2_6C09
        mov    eax, dword ptr [ebp + 0ch]
        movzx  ecx, word ptr [edi + 8dh]
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_2_6C0E
loc_2_6C09:
        mov    esi, 0fffffff3h
loc_2_6C0E:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_6C14:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_425B
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_2_6C39
        mov    eax, dword ptr [ebp + 0ch]
        movzx  ecx, word ptr [edi + 91h]
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_2_6C3E
loc_2_6C39:
        mov    esi, 0fffffff1h
loc_2_6C3E:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_6C44:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_4288
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_2_6C7E
        movzx  ecx, word ptr [edi + 91h]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    ecx, eax
        jb     SHORT loc_2_6C77
        mov    ecx, dword ptr [edi + eax*4 + 8fh]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_2_6C83
loc_2_6C77:
        mov    esi, 0fffffff0h
        jmp    SHORT loc_2_6C83
loc_2_6C7E:
        mov    esi, 0fffffff1h
loc_2_6C83:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_6C89:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    esi, esi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_4297
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_2_6CAD
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [edi + 9bh]
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_2_6CB2
loc_2_6CAD:
        mov    esi, 0fffffff1h
loc_2_6CB2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_6CB8:
        mov    eax, 0fffffffeh
        ret
sub_2_6CBE:
        push   ebp
        mov    ebp, esp
        push   0
        push   3
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        mov    ecx, dword ptr [ebp + 0ch]
        add    esp, 0ch
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        ret
sub_2_6CD9:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], edi
        lea    eax, [ebp - 4]
        push   eax
        push   3
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9825
        add    esp, 0ch
        push   edi
        push   dword ptr [esi + 2ch]
        push   dword ptr [esi + 34h]
        call   sub_2_7CBD
        add    esp, 0ch
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_6D10:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, 0fffffffdh
        lea    ecx, [esi - 1]
        cmp    ecx, 20h
        jae    SHORT loc_2_6D5D
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 10h]
        push   eax
        push   5
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        add    esp, 0ch
        push   esi
        call   sub_2_A035
        add    esp, 4
        and    eax, dword ptr [ebp - 4]
        cmp    eax, 1
        sbb    ecx, ecx
        inc    ecx
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], ecx
        sub    eax, eax
loc_2_6D5D:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_6D62:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, 0fffffffdh
        lea    ecx, [esi - 1]
        cmp    ecx, 20h
        jae    SHORT loc_2_6DC8
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 10h]
        push   eax
        push   5
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_2_9847
        add    esp, 0ch
        cmp    dword ptr [ebp + 10h], 0
        push   esi
        je     SHORT loc_2_6DAC
        call   sub_2_A035
        add    esp, 4
        or     dword ptr [ebp - 4], eax
        jmp    SHORT loc_2_6DB9
loc_2_6DAC:
        call   sub_2_A035
        add    esp, 4
        not    eax
        and    dword ptr [ebp - 4], eax
loc_2_6DB9:
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   edi
        call   sub_2_A4E4
        add    esp, 0ch
loc_2_6DC8:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_6DCE:
        mov    eax, 0fffffffeh
        ret
loc_2_6DD4:
        mov    eax, 0fffffffeh
        ret
sub_2_6DDA:
        push   ebp
        mov    ebp, esp
        push   0
        push   1
        push   dword ptr [ebp + 8]
        call   sub_2_9847
        mov    ecx, dword ptr [ebp + 0ch]
        add    esp, 0ch
        mov    dword ptr [ecx], eax
        sub    eax, eax
        pop    ebp
        ret
loc_2_6DF5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], edi
        lea    eax, [ebp - 4]
        push   eax
        push   1
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_9825
        add    esp, 0ch
        push   edi
        push   dword ptr [esi + 2ch]
        push   dword ptr [esi + 34h]
        call   sub_2_7E1D
        add    esp, 0ch
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_2_6E2C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, 0fffffffdh
        lea    ecx, [edi - 1]
        cmp    ecx, 20h
        jae    SHORT loc_2_6E60
        push   0
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_6D62
        add    esp, 0ch
        or     eax, eax
        jl     SHORT loc_2_6E60
        push   0
        push   edi
        push   esi
        call   sub_2_67F4
        add    esp, 0ch
loc_2_6E60:
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_2_6E64:
        mov    eax, 0fffffffeh
        ret
loc_2_6E6A:
        mov    eax, 0fffffffeh
        ret
loc_2_6E70:
        mov    eax, 0fffffffeh
        ret
        db  000h, 000h
sub_2_6E78:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    ebx, edi
        mov    ecx, dword ptr [ebp + 10h]
        jcxz   loc_2_6E98
loc_2_6E8C:
        lodsb
        or     al, al
        je     SHORT loc_2_6E94
        stosb
        loop   loc_2_6E8C
loc_2_6E94:
        xor    al, al
        rep stosb
loc_2_6E98:
        mov    eax, ebx
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
        db  000h
sub_2_6EA0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 8]
        mov    al, 0ffh
loc_2_6EAD:
        or     al, al
        je     SHORT loc_2_6EDD
        lodsb
        mov    ah, byte ptr [ebx]
        inc    ebx
        cmp    ah, al
        je     SHORT loc_2_6EAD
        sub    al, 41h
        cmp    al, 1ah
        sbb    cl, cl
        and    cl, 20h
        add    al, cl
        add    al, 41h
        xchg   al, ah
        sub    al, 41h
        cmp    al, 1ah
        sbb    cl, cl
        and    cl, 20h
        add    al, cl
        add    al, 41h
        cmp    al, ah
        je     SHORT loc_2_6EAD
        sbb    al, al
        sbb    al, 0ffh
loc_2_6EDD:
        movsx  eax, al
        pop    ebx
        pop    esi
        leave
        ret
sub_2_6EE4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    bl, 1
        mov    ecx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        cmp    cl, 0ah
        jne    SHORT loc_2_6EFA
        cwde
loc_2_6EFA:
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        cld
        xchg   ebx, eax
        or     al, al
        je     SHORT loc_2_6F17
        cmp    cl, 0ah
        jne    SHORT loc_2_6F17
        or     edx, edx
        jns    SHORT loc_2_6F17
        mov    al, 2dh
        stosb
        neg    ebx
        adc    edx, 0
        neg    edx
loc_2_6F17:
        mov    esi, edi
loc_2_6F19:
        xchg   edx, eax
        xor    edx, edx
        or     eax, eax
        je     SHORT loc_2_6F22
        div    ecx
loc_2_6F22:
        xchg   ebx, eax
        div    ecx
        xchg   edx, eax
        xchg   ebx, edx
        add    al, 30h
        cmp    al, 39h
        jbe    SHORT loc_2_6F30
        add    al, 27h
loc_2_6F30:
        stosb
        mov    eax, edx
        or     eax, ebx
        jne    SHORT loc_2_6F19
        mov    byte ptr [edi], al
loc_2_6F39:
        dec    edi
        lodsb
        xchg   byte ptr [edi], al
        mov    byte ptr [esi - 1], al
        lea    eax, [esi + 1]
        cmp    eax, edi
        jb     SHORT loc_2_6F39
        pop    eax
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
        db  000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        cwde
        xor    ebx, ebx
loc_2_6F5E:
        lodsb
        cmp    al, 20h
        je     SHORT loc_2_6F5E
        cmp    al, 9
        je     SHORT loc_2_6F5E
        push   eax
        cmp    al, 2dh
        je     SHORT loc_2_6F70
        cmp    al, 2bh
        jne    SHORT loc_2_6F71
loc_2_6F70:
        lodsb
loc_2_6F71:
        cmp    al, 39h
        ja     SHORT loc_2_6F94
        sub    al, 30h
        jb     SHORT loc_2_6F94
        shl    ebx, 1
        rcl    edx, 1
        mov    ecx, ebx
        mov    edi, edx
        shl    ebx, 1
        rcl    edx, 1
        shl    ebx, 1
        rcl    edx, 1
        add    ebx, ecx
        adc    edx, edi
        add    ebx, eax
        adc    edx, 0
        jmp    SHORT loc_2_6F70
loc_2_6F94:
        pop    eax
        cmp    al, 2dh
        xchg   ebx, eax
        jne    SHORT loc_2_6FA1
        neg    eax
        adc    edx, 0
        neg    edx
loc_2_6FA1:
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
        db  000h, 000h
sub_2_6FA8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        jcxz   loc_2_6FF8
        mov    bh, 41h
        mov    bl, 5ah
        mov    dh, 20h
loc_2_6FC0:
        mov    ah, byte ptr [esi]
        mov    al, byte ptr [edi]
        or     ah, ah
        je     SHORT loc_2_6FE8
        or     al, al
        je     SHORT loc_2_6FE8
        inc    esi
        inc    edi
        cmp    ah, bh
        jb     SHORT loc_2_6FD8
        cmp    ah, bl
        ja     SHORT loc_2_6FD8
        add    ah, dh
loc_2_6FD8:
        cmp    al, bh
        jb     SHORT loc_2_6FE2
        cmp    al, bl
        ja     SHORT loc_2_6FE2
        add    al, dh
loc_2_6FE2:
        cmp    ah, al
        jne    SHORT loc_2_6FEE
        loop   loc_2_6FC0
loc_2_6FE8:
        xor    ecx, ecx
        cmp    ah, al
        je     SHORT loc_2_6FF8
loc_2_6FEE:
        mov    cx, 0
        jb     SHORT loc_2_6FF6
        dec    ecx
        dec    ecx
loc_2_6FF6:
        not    ecx
loc_2_6FF8:
        mov    eax, ecx
        pop    ebx
        pop    esi
        pop    edi
        leave
        ret
        db  000h
sub_2_7000:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, edi
        xor    eax, eax
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        mov    al, byte ptr [ebp + 0ch]
        mov    edi, ebx
        repne scasb
        je     SHORT loc_2_7029
        or     al, al
        jne    SHORT loc_2_7025
        inc    edi
        jmp    SHORT loc_2_7029
loc_2_7025:
        xor    eax, eax
        jmp    SHORT loc_2_702C
loc_2_7029:
        lea    eax, [edi - 1]
loc_2_702C:
        pop    ebx
        pop    edi
        leave
        ret
sub_2_7030:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        dec    edi
        mov    al, byte ptr [ebp + 0ch]
        std
        repne scasb
        je     SHORT loc_2_7051
        xor    eax, eax
        mov    edx, eax
        jmp    SHORT loc_2_7054
loc_2_7051:
        lea    eax, [edi + 1]
loc_2_7054:
        cld
        pop    edi
        leave
        ret
sub_2_7058:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, ebx
        jmp    SHORT loc_2_706F
        db  090h
loc_2_7064:
        sub    al, 61h
        cmp    al, 1ah
        jae    SHORT loc_2_706E
        add    al, 41h
        mov    byte ptr [ebx], al
loc_2_706E:
        inc    ebx
loc_2_706F:
        mov    al, byte ptr [ebx]
        or     al, al
        jne    SHORT loc_2_7064
        xchg   edx, eax
        pop    ebx
        leave
        ret
        db  000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        add    esp, -4
        push   esi
        push   edi
        push   ebx
        mov    dl, 1
        mov    byte ptr [ebp - 1], dl
        mov    esi, dword ptr [ebp + 8]
loc_2_708D:
        lodsb
        cmp    al, 20h
        je     SHORT loc_2_708D
        cmp    al, 0dh
        ja     SHORT loc_2_709C
        cmp    al, 9
        jb     SHORT loc_2_709C
        jmp    SHORT loc_2_708D
loc_2_709C:
        cmp    al, 2bh
        je     SHORT loc_2_70A8
        cmp    al, 2dh
        jne    SHORT loc_2_70A9
        or     byte ptr [ebp - 1], 2
loc_2_70A8:
        lodsb
loc_2_70A9:
        mov    ecx, dword ptr [ebp + 10h]
        jecxz  loc_2_70BD
        cmp    ecx, 1
        je     SHORT loc_2_70B8
        cmp    ecx, 24h
        jbe    SHORT loc_2_70DC
loc_2_70B8:
        jmp    NEAR PTR loc_2_7150
loc_2_70BD:
        mov    ecx, 0ah
        cmp    al, 30h
        jne    SHORT loc_2_70DC
        cmp    byte ptr [esi], 78h
        je     SHORT loc_2_70D7
        cmp    byte ptr [esi], 58h
        je     SHORT loc_2_70D7
        mov    ecx, 8
        jmp    SHORT loc_2_70DC
loc_2_70D7:
        mov    ecx, 10h
loc_2_70DC:
        mov    dword ptr [ebp + 10h], ecx
        cmp    ecx, 10h
        jne    SHORT loc_2_70F4
        cmp    al, 30h
        jne    SHORT loc_2_70F4
        cmp    byte ptr [esi], 78h
        je     SHORT loc_2_70F2
        cmp    byte ptr [esi], 58h
        jne    SHORT loc_2_70F4
loc_2_70F2:
        inc    esi
        lodsb
loc_2_70F4:
        xor    edx, edx
        xor    ebx, ebx
loc_2_70F8:
        call   sub_2_718D
        jb     SHORT loc_2_7159
        mov    ecx, dword ptr [ebp + 10h]
        cmp    eax, ecx
        jae    SHORT loc_2_7159
        or     byte ptr [ebp - 1], 8
        mov    edi, eax
        mov    eax, edx
        mul    ecx
        jae    SHORT loc_2_7116
        or     byte ptr [ebp - 1], 4
loc_2_7116:
        push   eax
        mov    eax, ebx
        mul    ecx
        pop    ecx
        add    edx, ecx
        jae    SHORT loc_2_7124
        or     byte ptr [ebp - 1], 4
loc_2_7124:
        add    eax, edi
        adc    edx, 0
        jae    SHORT loc_2_712F
        or     byte ptr [ebp - 1], 4
loc_2_712F:
        mov    ebx, eax
        lodsb
        jmp    SHORT loc_2_70F8
loc_2_7134:
        mov    edx, 7fffffffh
        mov    ebx, 0ffffffffh
        test   byte ptr [ebp - 1], 1
        je     SHORT loc_2_7170
        or     edx, 80000000h
        and    byte ptr [ebp - 1], 0fdh
        jmp    SHORT loc_2_7170
loc_2_7150:
        mov    esi, dword ptr [ebp + 8]
        xor    ebx, ebx
        xor    edx, edx
        jmp    SHORT loc_2_7170
loc_2_7159:
        dec    esi
        test   byte ptr [ebp - 1], 8
        je     SHORT loc_2_7150
        test   byte ptr [ebp - 1], 4
        jne    SHORT loc_2_7134
        test   byte ptr [ebp - 1], 1
        jne    SHORT loc_2_7170
        or     edx, edx
        js     SHORT loc_2_7134
loc_2_7170:
        mov    edi, dword ptr [ebp + 0ch]
        or     edi, edi
        je     SHORT loc_2_7179
        mov    dword ptr [edi], esi
loc_2_7179:
        mov    eax, ebx
        test   byte ptr [ebp - 1], 2
        je     SHORT loc_2_7188
        neg    eax
        adc    edx, 0
        neg    edx
loc_2_7188:
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_2_718D:
        cmp    al, 61h
        jb     SHORT loc_2_7193
        and    al, 0dfh
loc_2_7193:
        sub    al, 30h
        jb     SHORT loc_2_71A4
        cmp    al, 0ah
        jb     SHORT loc_2_71A3
        cmp    al, 11h
        jb     SHORT loc_2_71A4
        sub    al, 7
        cmp    al, 24h
loc_2_71A3:
        cmc
loc_2_71A4:
        movsx  eax, al
        pop    ebx
        pop    edi
        pop    esi
        leave
        ret
sub_2_71AC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    ebx, ebx
        xor    ecx, ecx
        mov    esi, dword ptr [ebp + 8]
loc_2_71B8:
        lodsb
        cmp    al, 20h
        je     SHORT loc_2_71B8
        cmp    al, 9
        je     SHORT loc_2_71B8
        cmp    al, 2bh
        je     SHORT loc_2_71CA
        cmp    al, 2dh
        jne    SHORT loc_2_71CB
        dec    ecx
loc_2_71CA:
        lodsb
loc_2_71CB:
        cmp    al, 30h
        jb     SHORT loc_2_71EF
        cmp    al, 39h
        jbe    SHORT loc_2_71DF
        or     al, 20h
        cmp    al, 66h
        ja     SHORT loc_2_71EF
        cmp    al, 61h
        jb     SHORT loc_2_71EF
        sub    al, 57h
loc_2_71DF:
        and    eax, 0fh
        cmp    eax, dword ptr [ebp + 0ch]
        jae    SHORT loc_2_71EF
        xchg   ebx, eax
        mul    dword ptr [ebp + 0ch]
        add    ebx, eax
        jmp    SHORT loc_2_71CA
loc_2_71EF:
        mov    eax, ebx
        jcxz   loc_2_71F6
        neg    eax
loc_2_71F6:
        pop    ebx
        pop    esi
        leave
        ret
        db  000h, 000h
sub_2_71FC:
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 8]
        cmp    al, 9
        jb     SHORT loc_2_7212
        cmp    al, 0dh
        ja     SHORT loc_2_7212
        cmp    al, 20h
        jne    SHORT loc_2_7212
        mov    al, 1
        jmp    SHORT loc_2_7214
loc_2_7212:
        xor    eax, eax
loc_2_7214:
        leave
        ret
        db  000h, 000h
sub_2_7218:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        db  068h, 07Ch, 000h, 000h, 000h
        mov    ebx, dword ptr [ebp + 10h]
        push   ebx
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_863A
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_2_7263
        push   dword ptr [ebp + 14h]
        push   ebx
        push   edi
        push   esi
        call   sub_2_867A
        add    esp, 10h
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   ebx
        push   edi
        push   esi
        call   sub_2_863A
        add    esp, 18h
loc_2_7263:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_7268:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 14h]
        or     edi, edi
        jne    SHORT loc_2_728A
        push   0
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_867A
        add    esp, 10h
        jmp    SHORT loc_2_72FF
loc_2_728A:
        push   1
        push   80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_7296:
        add    esp, 8
        db  0A3h, 03Ch, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_72FD
        push   80h
        push   eax
        db  068h, 080h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 10h]
        push   esi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_863A
        add    esp, 18h
        db  0FFh, 035h, 03Ch, 000h, 000h, 000h
        push   edi
        call   sub_2_6EA0
        add    esp, 8
        mov    ebx, eax
        push   0
        db  0FFh, 035h, 03Ch, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_72DD:
        add    esp, 8
        or     ebx, ebx
        je     SHORT loc_2_72F6
        push   edi
        push   esi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_867A
        add    esp, 10h
        jmp    SHORT loc_2_72FF
loc_2_72F6:
        mov    eax, 1
        jmp    SHORT loc_2_72FF
loc_2_72FD:
        sub    eax, eax
loc_2_72FF:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        mov    al, byte ptr [ebp + 8]
        cmp    al, 30h
        jl     SHORT loc_2_7317
        cmp    al, 39h
        mov    eax, 1
        jle    SHORT loc_2_7319
loc_2_7317:
        sub    eax, eax
loc_2_7319:
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    bl, byte ptr [ebp + 8]
        movsx  eax, bl
        push   eax
        call   sub_2_71FC
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_2_733C
        cmp    bl, 2ch
        mov    eax, 0
        jne    SHORT loc_2_7341
loc_2_733C:
        mov    eax, 1
loc_2_7341:
        pop    ebx
        pop    ebp
        ret
sub_2_7344:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + ecx*4]
        pop    ebp
        ret
sub_2_7352:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        sub    ebx, ebx
        cmp    dword ptr [ebp + 0ch], ebx
        je     SHORT loc_2_7391
        mov    esi, dword ptr [ebp + 14h]
loc_2_7364:
        or     edi, edi
        jne    SHORT loc_2_7391
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_2_7344
        add    esp, 8
        push   dword ptr [ebp + 10h]
        push   eax
        call   sub_2_6EA0
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_738B
        mov    dword ptr [esi], ebx
        mov    edi, 1
loc_2_738B:
        inc    ebx
        cmp    dword ptr [ebp + 0ch], ebx
        ja     SHORT loc_2_7364
loc_2_7391:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_7398:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax], 0
        je     SHORT loc_2_73AE
loc_2_73A3:
        cmp    byte ptr [eax], 20h
        jne    SHORT loc_2_73AE
        inc    eax
        cmp    byte ptr [eax], 0
        jne    SHORT loc_2_73A3
loc_2_73AE:
        pop    ebp
        ret
sub_2_73B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 0ch]
        db  068h, 084h, 000h, 000h, 000h
        push   dword ptr [ebp + 8]
        call   sub_2_6EE4
        add    esp, 0ch
        db  068h, 084h, 000h, 000h, 000h
        call   sub_2_7398
        add    esp, 4
        mov    ebx, eax
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, ecx
        push   1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_73EF:
        add    esp, 8
        mov    esi, eax
        push   edi
        push   ebx
        push   esi
        call   sub_2_6E78
        add    esp, 0ch
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_7406:
        push   ebp
        mov    ebp, esp
        mov    edx, 1
        mov    cl, byte ptr [ebp + 0ch]
        shl    edx, cl
        and    edx, dword ptr [ebp + 8]
        cmp    edx, 1
        sbb    eax, eax
        inc    eax
        pop    ebp
        ret
sub_2_741E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, 1fh
loc_2_742C:
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_2_7406
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_2_7442
        dec    edi
        cmp    edi, 3
        jae    SHORT loc_2_742C
loc_2_7442:
        inc    edi
        push   1
        lea    eax, [edi + 1]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_744E:
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    esi, eax
        sub    ebx, ebx
        cmp    edi, ebx
        je     SHORT loc_2_7477
loc_2_745C:
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_2_7406
        add    esp, 8
        cmp    eax, 1
        sbb    al, al
        add    al, 31h
        mov    byte ptr [esi], al
        inc    esi
        inc    ebx
        cmp    edi, ebx
        ja     SHORT loc_2_745C
loc_2_7477:
        mov    byte ptr [esi], 0
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7484:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        sub    eax, eax
        sub    ecx, ecx
        mov    edx, dword ptr [ebp + 8]
loc_2_7490:
        mov    bl, byte ptr [edx]
        or     bl, bl
        je     SHORT loc_2_74AB
        inc    edx
        cmp    bl, 31h
        jne    SHORT loc_2_74A5
        mov    esi, 1
        shl    esi, cl
        or     eax, esi
loc_2_74A5:
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_7490
loc_2_74AB:
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_74AF:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        sub    edx, edx
        sub    eax, eax
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, eax
        je     SHORT loc_2_752C
        cmp    byte ptr [ecx], 1
        sbb    esi, esi
        neg    esi
        cmp    esi, eax
        jne    SHORT loc_2_74E2
loc_2_74CA:
        mov    al, byte ptr [ecx + edx]
        cmp    al, 2ch
        jne    SHORT loc_2_74E2
        cmp    al, 20h
        jne    SHORT loc_2_74E2
        inc    edx
        cmp    byte ptr [ecx + edx], 1
        sbb    esi, esi
        neg    esi
        or     esi, esi
        je     SHORT loc_2_74CA
loc_2_74E2:
        or     esi, esi
        je     SHORT loc_2_74EF
        sub    eax, eax
        db  0A3h, 070h, 000h, 000h, 000h
        jmp    SHORT loc_2_752C
loc_2_74EF:
        lea    eax, [ecx + edx]
        or     esi, esi
        jne    SHORT loc_2_7514
loc_2_74F6:
        mov    bl, byte ptr [ecx + edx]
        cmp    bl, 2ch
        je     SHORT loc_2_7510
        cmp    bl, 20h
        je     SHORT loc_2_7510
        inc    edx
        cmp    byte ptr [ecx + edx], 1
        sbb    esi, esi
        neg    esi
        or     esi, esi
        je     SHORT loc_2_74F6
loc_2_7510:
        or     esi, esi
        je     SHORT loc_2_7520
loc_2_7514:
        db  0C7h, 005h, 070h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_2_752C
loc_2_7520:
        add    ecx, edx
        mov    byte ptr [ecx], 0
        inc    ecx
        db  089h, 00Dh, 070h, 000h, 000h, 000h
loc_2_752C:
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_7530:
        db  0FFh, 035h, 070h, 000h, 000h, 000h
        call   sub_2_74AF
        add    esp, 4
        ret
sub_2_753F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    eax, eax
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, edx
        mov    esi, dword ptr [edx]
        cmp    esi, eax
        je     SHORT loc_2_755A
loc_2_7551:
        add    ecx, 4
        inc    eax
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_2_7551
loc_2_755A:
        mov    ecx, dword ptr [ebp + 10h]
        shl    ecx, 2
        lea    edi, [eax + 1]
        cmp    ecx, edi
        jae    SHORT loc_2_756B
        sub    eax, eax
        jmp    SHORT loc_2_7587
loc_2_756B:
        or     esi, esi
        mov    ecx, dword ptr [ebp + 0ch]
        je     SHORT loc_2_7581
loc_2_7572:
        mov    esi, dword ptr [edx]
        mov    dword ptr [ecx], esi
        add    edx, 4
        add    ecx, 4
        cmp    dword ptr [edx], 0
        jne    SHORT loc_2_7572
loc_2_7581:
        mov    dword ptr [ecx], 0
loc_2_7587:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_758B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0C7h, 005h, 078h, 000h, 000h, 000h, 074h, 000h, 000h, 000h
        push   1
        push   80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_75A7:
        add    esp, 8
        db  0A3h, 03Ch, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_2_760F
        push   80h
        push   eax
        mov    esi, dword ptr [ebp + 14h]
        push   esi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_7218
        add    esp, 18h
        db  0FFh, 035h, 078h, 000h, 000h, 000h
        db  0FFh, 035h, 03Ch, 000h, 000h, 000h
        mov    ebx, dword ptr [ebp + 1ch]
        push   ebx
        mov    edi, dword ptr [ebp + 18h]
        push   edi
        call   sub_2_7352
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_2_75FF
        db  0FFh, 035h, 078h, 000h, 000h, 000h
        push   esi
        push   ebx
        push   edi
        call   sub_2_7352
        add    esp, 10h
loc_2_75FF:
        push   0
        db  0FFh, 035h, 03Ch, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_760C:
        add    esp, 8
loc_2_760F:
        db  0A1h, 078h, 000h, 000h, 000h
        pop    edi
        mov    eax, dword ptr [eax]
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_761B:
        push   ebp
        mov    ebp, esp
        push   7
        db  068h, 020h, 000h, 000h, 000h
        push   dword ptr [ebp + 14h]
        db  068h, 020h, 000h, 000h, 000h
        call   sub_2_7344
        add    esp, 8
        push   eax
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_758B
        add    esp, 18h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        pop    ebp
        ret
sub_2_764F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    ebx, dword ptr [ebp + 18h]
        push   ebx
        push   dword ptr [ebp + 14h]
        call   sub_2_73B0
        add    esp, 8
        mov    esi, eax
        push   1
        push   80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_7674:
        add    esp, 8
        db  0A3h, 03Ch, 000h, 000h, 000h
        cmp    eax, edi
        je     SHORT loc_2_76B9
        push   80h
        push   eax
        push   esi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_7218
        add    esp, 18h
        push   ebx
        db  0FFh, 035h, 03Ch, 000h, 000h, 000h
        call   sub_2_71AC
        add    esp, 8
        mov    edi, eax
        push   0
        db  0FFh, 035h, 03Ch, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_76B6:
        add    esp, 8
loc_2_76B9:
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_76C1:
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_76CB:
        push   ebp
        mov    ebp, esp
        push   0ah
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_764F
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   10h
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_764F
        mov    esp, ebp
        pop    ebp
        ret
sub_2_76FF:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        push   dword ptr [ebp + 14h]
        call   sub_2_741E
        add    esp, 4
        mov    esi, eax
        push   1
        push   80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_771F:
        add    esp, 8
        db  0A3h, 03Ch, 000h, 000h, 000h
        cmp    eax, edi
        je     SHORT loc_2_7763
        push   80h
        push   eax
        push   esi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_7218
        add    esp, 18h
        db  0FFh, 035h, 03Ch, 000h, 000h, 000h
        call   sub_2_7484
        add    esp, 4
        mov    edi, eax
        push   0
        db  0FFh, 035h, 03Ch, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_7760:
        add    esp, 8
loc_2_7763:
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_776B:
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_7774:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        cmp    dword ptr [ebp + 28h], esi
        je     NEAR PTR loc_2_783C
        push   1
        push   80h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_7791:
        add    esp, 8
        db  0A3h, 03Ch, 000h, 000h, 000h
        cmp    eax, esi
        je     SHORT loc_2_780B
        push   80h
        push   eax
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_7218
        add    esp, 18h
        db  0FFh, 035h, 03Ch, 000h, 000h, 000h
        call   sub_2_74AF
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_2_77FB
        db  0BFh, 048h, 000h, 000h, 000h
        mov    ebx, dword ptr [ebp + 1ch]
loc_2_77D1:
        db  068h, 074h, 000h, 000h, 000h
        push   eax
        push   dword ptr [ebp + 20h]
        push   ebx
        call   sub_2_7352
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_2_77F2
        db  0A1h, 074h, 000h, 000h, 000h
        mov    dword ptr [edi], eax
        add    edi, 4
        inc    esi
loc_2_77F2:
        call   sub_2_7530
        or     eax, eax
        jne    SHORT loc_2_77D1
loc_2_77FB:
        push   0
        db  0FFh, 035h, 03Ch, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_7808:
        add    esp, 8
loc_2_780B:
        db  0C7h, 004h, 0B5h, 048h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        or     esi, esi
        push   dword ptr [ebp + 28h]
        push   dword ptr [ebp + 24h]
        jne    SHORT loc_2_782D
        push   dword ptr [ebp + 18h]
        call   sub_2_753F
        add    esp, 0ch
        jmp    SHORT loc_2_783E
loc_2_782D:
        db  068h, 048h, 000h, 000h, 000h
        call   sub_2_753F
        add    esp, 0ch
        jmp    SHORT loc_2_783E
loc_2_783C:
        sub    eax, eax
loc_2_783E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_7843:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_7268
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 14h]
        db  068h, 020h, 000h, 000h, 000h
        call   sub_2_7344
        add    esp, 8
        push   eax
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_7843
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7881:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0ah
        push   dword ptr [ebp + 14h]
        call   sub_2_73B0
        add    esp, 8
        mov    esi, eax
        push   esi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_7268
        add    esp, 10h
        mov    edi, eax
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_78B1:
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   10h
        push   dword ptr [ebp + 14h]
        call   sub_2_73B0
        add    esp, 8
        mov    esi, eax
        push   esi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_7268
        add    esp, 10h
        mov    edi, eax
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_78EA:
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_78F3:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   dword ptr [ebp + 14h]
        call   sub_2_741E
        add    esp, 4
        mov    esi, eax
        push   esi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_7268
        add    esp, 10h
        mov    edi, eax
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_7921:
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_792A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        push   0ah
        push   dword ptr [ebp + 0ch]
        call   sub_2_73B0
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        je     NEAR PTR loc_2_79DF
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   1
        lea    edx, [ecx - 1]
        mov    esi, dword ptr [ebp + 8]
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edx, [edx + ecx - 1]
        inc    edx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_797A:
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_79D2
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_2_79D2:
        push   0
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_79DC:
        add    esp, 8
loc_2_79DF:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_79E8:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_79F5:
        add    esp, 8
        mov    eax, 1
        pop    ebp
        ret
        db  000h
sub_2_7A00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   10h
        db  0BFh, 0E0h, 006h, 000h, 000h
        db  068h, 0E0h, 006h, 000h, 000h
        push   dword ptr [ebp + 10h]
        call   sub_2_6EE4
        add    esp, 0ch
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [ebp + 8]
        lea    edx, [ecx - 1]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [eax + ecx*4]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        db  0BFh, 0E0h, 006h, 000h, 000h
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BFh, 000h, 007h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebx, ecx
        db  0BFh, 0E0h, 006h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        db  0BFh, 0E0h, 006h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        sub    ecx, edx
        push   10h
        db  08Dh, 081h, 0E0h, 006h, 000h, 000h
        push   eax
        push   dword ptr [ebp + 10h]
        call   sub_2_6EE4
        add    esp, 0ch
        db  0B8h, 0E0h, 006h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_2_7ABA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   4
        db  068h, 070h, 006h, 000h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_2_839E
        add    esp, 4
        db  0FFh, 034h, 085h, 070h, 006h, 000h, 000h
        db  0FFh, 035h, 018h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   esi
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_758B
        add    esp, 18h
        pop    esi
        pop    ebp
        ret
sub_2_7AFD:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        db  068h, 070h, 006h, 000h, 000h
        call   sub_2_7344
        add    esp, 8
        push   eax
        db  0FFh, 035h, 018h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_7843
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   4
        db  068h, 090h, 006h, 000h, 000h
        db  0FFh, 035h, 09Ch, 006h, 000h, 000h
        db  0FFh, 035h, 01Ch, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_758B
        mov    esp, ebp
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   0
        db  0FFh, 035h, 01Ch, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_7268
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7B95:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        push   dword ptr [ebp + 10h]
        db  0FFh, 035h, 048h, 001h, 000h, 000h
        call   sub_2_792A
        add    esp, 8
        mov    esi, eax
        cmp    esi, edi
        je     SHORT loc_2_7BDF
        push   dword ptr [ebp + 14h]
        push   esi
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   edi
        call   sub_2_76FF
        add    esp, 10h
        mov    edi, eax
        push   esi
        call   sub_2_79E8
        add    esp, 4
loc_2_7BDF:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_7BE5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        push   dword ptr [ebp + 10h]
        db  0FFh, 035h, 048h, 001h, 000h, 000h
        call   sub_2_792A
        add    esp, 8
        mov    esi, eax
        cmp    esi, edi
        je     SHORT loc_2_7C2F
        push   dword ptr [ebp + 14h]
        push   esi
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   edi
        call   sub_2_78F3
        add    esp, 10h
        mov    edi, eax
        push   esi
        call   sub_2_79E8
        add    esp, 4
loc_2_7C2F:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_7C35:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        sub    edi, edi
        push   dword ptr [ebp + 10h]
        db  0FFh, 035h, 040h, 001h, 000h, 000h
        call   sub_2_792A
        add    esp, 8
        mov    esi, eax
        cmp    esi, edi
        je     SHORT loc_2_7C8D
        push   6
        db  068h, 0C8h, 006h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        db  0FFh, 034h, 085h, 0C8h, 006h, 000h, 000h
        push   esi
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   edi
        call   sub_2_758B
        add    esp, 18h
        mov    edi, eax
        push   esi
        call   sub_2_79E8
        add    esp, 4
loc_2_7C8D:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_2_7C93:
        push   ebp
        mov    ebp, esp
        push   0
        db  0FFh, 035h, 04Ch, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_76CB
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7CBD:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        db  0FFh, 035h, 04Ch, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_7881
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7CE8:
        push   ebp
        mov    ebp, esp
        push   0
        db  0FFh, 035h, 058h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_76FF
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7D12:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        db  0FFh, 035h, 058h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_78F3
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7D3D:
        push   ebp
        mov    ebp, esp
        push   0
        db  0FFh, 035h, 050h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_76FF
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7D67:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        db  0FFh, 035h, 050h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_78F3
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7D92:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_2_83CC
        add    esp, 4
        push   eax
        db  0FFh, 035h, 024h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   esi
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   1
        call   sub_2_76CB
        add    esp, 10h
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        db  0FFh, 035h, 024h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   1
        call   sub_2_7881
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7DF3:
        push   ebp
        mov    ebp, esp
        push   30h
        db  0FFh, 035h, 054h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_76CB
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7E1D:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        db  0FFh, 035h, 054h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_7881
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7E48:
        push   ebp
        mov    ebp, esp
        push   esi
        push   3dh
        db  068h, 028h, 005h, 000h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_2_8400
        add    esp, 4
        db  0FFh, 034h, 085h, 028h, 005h, 000h, 000h
        db  0FFh, 035h, 030h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   esi
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   1
        call   sub_2_758B
        add    esp, 18h
        pop    esi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7344
        add    esp, 8
        push   eax
        db  0FFh, 035h, 030h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   1
        call   sub_2_7843
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7EC4:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        db  0FFh, 035h, 034h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   1
        call   sub_2_76CB
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7EEF:
        push   ebp
        mov    ebp, esp
        push   0
        db  0FFh, 035h, 038h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_76CB
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7F19:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        db  0FFh, 035h, 038h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_7881
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7F44:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_2_8370
        add    esp, 4
        push   eax
        db  0FFh, 035h, 03Ch, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   esi
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_76CB
        add    esp, 10h
        pop    esi
        pop    ebp
        ret
sub_2_7F7A:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        db  0FFh, 035h, 03Ch, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_7881
        mov    esp, ebp
        pop    ebp
        ret
sub_2_7FA5:
        push   ebp
        mov    ebp, esp
        push   1
        db  0FFh, 035h, 044h, 001h, 000h, 000h
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0ch]
        db  068h, 028h, 005h, 000h, 000h
        call   sub_2_7A00
        add    esp, 0ch
        push   eax
        push   0
        call   sub_2_761B
        mov    esp, ebp
        pop    ebp
        ret
        db  000h
sub_2_7FD0:
        push   0
        db  0FFh, 035h, 0A8h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_761B
        add    esp, 10h
        ret
        db  06Ah, 001h, 0FFh, 035h, 0B4h, 000h, 000h, 000h, 068h, 000h, 000h, 000h, 000h, 06Ah, 001h, 0E8h
        db  01Fh, 0F6h, 0FFh, 0FFh, 083h, 0C4h, 010h, 0C3h
sub_2_8000:
        push   0
        db  0FFh, 035h, 0B8h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_761B
        add    esp, 10h
        ret
        push   ebp
        mov    ebp, esp
        call   sub_2_8000
        or     eax, eax
        je     SHORT loc_2_8040
        movzx  eax, word ptr [ebp + 8]
        push   eax
        db  0FFh, 035h, 0BCh, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_76CB
        add    esp, 10h
        jmp    SHORT loc_2_8044
loc_2_8040:
        mov    ax, word ptr [ebp + 8]
loc_2_8044:
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        call   sub_2_8000
        or     eax, eax
        je     SHORT loc_2_806E
        movzx  eax, word ptr [ebp + 8]
        push   eax
        db  0FFh, 035h, 0C0h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_76CB
        add    esp, 10h
        jmp    SHORT loc_2_8072
loc_2_806E:
        mov    ax, word ptr [ebp + 8]
loc_2_8072:
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        call   sub_2_8000
        or     eax, eax
        je     SHORT loc_2_809C
        movzx  eax, word ptr [ebp + 8]
        push   eax
        db  0FFh, 035h, 0C4h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_76CB
        add    esp, 10h
        jmp    SHORT loc_2_80A0
loc_2_809C:
        mov    ax, word ptr [ebp + 8]
loc_2_80A0:
        pop    ebp
        ret
sub_2_80A2:
        push   0
        db  0FFh, 035h, 0C8h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_761B
        add    esp, 10h
        ret
        db  000h, 000h
sub_2_80BC:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   9
        db  068h, 060h, 001h, 000h, 000h
        db  068h, 088h, 000h, 000h, 000h
        db  068h, 0A0h, 000h, 000h, 000h
        db  0FFh, 035h, 07Ch, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_7774
        mov    esp, ebp
        pop    ebp
        ret
sub_2_80EC:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   5
        db  068h, 0E0h, 000h, 000h, 000h
        db  068h, 0F8h, 000h, 000h, 000h
        db  068h, 008h, 001h, 000h, 000h
        db  0FFh, 035h, 080h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_7774
        mov    esp, ebp
        pop    ebp
        ret
        db  06Ah, 001h, 0FFh, 035h, 078h, 000h, 000h, 000h, 068h, 000h, 000h, 000h, 000h, 06Ah, 001h, 0E8h
        db  0EBh, 0F4h, 0FFh, 0FFh, 083h, 0C4h, 010h, 0C3h
sub_2_8134:
        push   9
        db  068h, 060h, 001h, 000h, 000h
        db  0FFh, 035h, 060h, 001h, 000h, 000h
        db  0FFh, 035h, 070h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_758B
        add    esp, 18h
        ret
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        db  068h, 060h, 001h, 000h, 000h
        call   sub_2_7344
        add    esp, 8
        push   eax
        db  0FFh, 035h, 070h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_7843
        mov    esp, ebp
        pop    ebp
        ret
sub_2_8181:
        push   5
        db  068h, 0E0h, 000h, 000h, 000h
        db  0FFh, 035h, 060h, 001h, 000h, 000h
        db  0FFh, 035h, 074h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_758B
        add    esp, 18h
        ret
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        db  068h, 0E0h, 000h, 000h, 000h
        call   sub_2_7344
        add    esp, 8
        push   eax
        db  0FFh, 035h, 074h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_7843
        mov    esp, ebp
        pop    ebp
        ret
        db  06Ah, 03Dh, 068h, 000h, 000h, 000h, 000h, 0FFh, 035h, 000h, 000h, 000h, 000h, 0FFh, 035h, 084h
        db  000h, 000h, 000h, 068h, 000h, 000h, 000h, 000h, 06Ah, 001h, 0E8h, 09Eh, 0F3h, 0FFh, 0FFh, 083h
        db  0C4h, 018h, 0C3h
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_7344
        add    esp, 8
        push   eax
        db  0FFh, 035h, 084h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_7843
        mov    esp, ebp
        pop    ebp
        ret
        db  000h
sub_2_821C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   0ah
        mov    esi, dword ptr [ebp + 8]
        mov    eax, esi
        shr    eax, 8
        push   eax
        call   sub_2_73B0
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        push   0ah
        and    esi, 0ffh
        push   esi
        call   sub_2_73B0
        add    esp, 8
        mov    esi, eax
        push   1
        push   5
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_8257:
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_8285
        push   1
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_2_6E78
        add    esp, 0ch
        lea    edi, [ebx + 1]
        push   2
        mov    byte ptr [edi], 2eh
        inc    edi
        push   esi
        push   edi
        call   sub_2_6E78
        add    esp, 0ch
        inc    edi
        inc    edi
        jmp    SHORT loc_2_8288
loc_2_8285:
        mov    edi, dword ptr [ebp - 8]
loc_2_8288:
        mov    byte ptr [edi], 0
        push   ebx
        db  0FFh, 035h, 038h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_7268
        add    esp, 10h
        mov    edi, eax
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_82AB:
        add    esp, 8
        push   0
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_82B8:
        add    esp, 8
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_82C3:
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_82CF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        sub    bl, bl
        mov    byte ptr [ebp - 1], bl
        push   1
        push   0ah
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_82ED:
        add    esp, 8
        mov    edi, eax
        mov    esi, edi
        or     edi, edi
        je     SHORT loc_2_8366
        push   0ah
        push   edi
        db  068h, 044h, 000h, 000h, 000h
        db  0FFh, 035h, 038h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   1
        call   sub_2_7218
        add    esp, 18h
        cmp    byte ptr [edi], 2eh
        je     SHORT loc_2_8334
loc_2_831A:
        mov    dl, byte ptr [esi]
        or     dl, dl
        je     SHORT loc_2_8334
        mov    cl, 0ah
        mov    al, byte ptr [ebp - 1]
        imul   cl
        lea    eax, [edx + eax - 30h]
        inc    esi
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [esi], 2eh
        jne    SHORT loc_2_831A
loc_2_8334:
        inc    esi
        cmp    byte ptr [esi], 0
        je     SHORT loc_2_834C
loc_2_833A:
        mov    cl, 0ah
        mov    al, bl
        imul   cl
        mov    cl, byte ptr [esi]
        inc    esi
        lea    ebx, [eax + ecx - 30h]
        cmp    byte ptr [esi], 0
        jne    SHORT loc_2_833A
loc_2_834C:
        movzx  ecx, byte ptr [ebp - 1]
        shl    ecx, 8
        movzx  eax, bl
        add    eax, ecx
        mov    dword ptr [ebp - 8], eax
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_8363:
        add    esp, 8
loc_2_8366:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_2_8370:
        push   ebp
        mov    ebp, esp
        sub    edx, edx
        mov    ecx, dword ptr [ebp + 8]
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_2_8392
        db  0B8h, 000h, 000h, 000h, 000h
loc_2_8385:
        cmp    dword ptr [eax], 0
        je     SHORT loc_2_8392
        add    eax, 6
        inc    edx
        cmp    dword ptr [eax], ecx
        jne    SHORT loc_2_8385
loc_2_8392:
        imul   edx, edx, 6
        pop    ebp
        db  00Fh, 0B6h, 082h, 004h, 000h, 000h, 000h
        ret
sub_2_839E:
        push   ebp
        mov    ebp, esp
        sub    ecx, ecx
        mov    eax, dword ptr [ebp + 8]
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_2_83C0
        db  0BAh, 000h, 000h, 000h, 000h
loc_2_83B3:
        cmp    dword ptr [edx], 0
        je     SHORT loc_2_83C0
        add    edx, 6
        inc    ecx
        cmp    dword ptr [edx], eax
        jne    SHORT loc_2_83B3
loc_2_83C0:
        imul   ecx, ecx, 6
        pop    ebp
        db  00Fh, 0B6h, 081h, 005h, 000h, 000h, 000h
        ret
sub_2_83CC:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 4
        je     SHORT loc_2_83F2
        cmp    eax, 18h
        jl     SHORT loc_2_83EB
        cmp    eax, 19h
        jle    SHORT loc_2_83F9
        cmp    eax, 22h
        jl     SHORT loc_2_83EB
        cmp    eax, 23h
        jle    SHORT loc_2_83F9
loc_2_83EB:
        mov    eax, 4b0h
        jmp    SHORT loc_2_83FE
loc_2_83F2:
        mov    eax, 960h
        jmp    SHORT loc_2_83FE
loc_2_83F9:
        mov    eax, 2580h
loc_2_83FE:
        pop    ebp
        ret
sub_2_8400:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        jne    SHORT loc_2_8410
        mov    eax, 13h
loc_2_8410:
        pop    ebp
        ret
        db  000h, 000h
sub_2_8414:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_8425:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_8429:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_843A:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_843E:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edx, dword ptr [ebp + 10h]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   edx
        push   1
        push   0
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_8463:
        add    esp, 18h
        pop    edi
        pop    ebp
        ret
sub_2_8469:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 1
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_848D:
        mov    esp, ebp
        pop    ebp
        ret
sub_2_8491:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        sub    esi, esi
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], esi
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 14h]
        call   sub_2_8414
        add    esp, 0ch
        cmp    eax, esi
        jne    SHORT loc_2_84E8
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 4]
        call   sub_2_8414
        add    esp, 0ch
        cmp    eax, esi
        jne    SHORT loc_2_84E8
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 8]
        call   sub_2_843E
        add    esp, 0ch
        cmp    eax, esi
        jne    SHORT loc_2_84E8
        mov    esi, 1
loc_2_84E8:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_84F9
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_84F6:
        add    esp, 4
loc_2_84F9:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_850A
        push   dword ptr [ebp - 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_8507:
        add    esp, 4
loc_2_850A:
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_8511:
        push   ebp
        mov    ebp, esp
        push   80000002h
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_8491
        mov    esp, ebp
        pop    ebp
        ret
sub_2_852B:
        push   ebp
        mov    ebp, esp
        push   80000001h
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_8491
        mov    esp, ebp
        pop    ebp
        ret
sub_2_8545:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        sub    edi, edi
        mov    dword ptr [ebp - 8], edi
        mov    dword ptr [ebp - 0ch], edi
        mov    esi, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 4], esi
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 1ch]
        call   sub_2_8429
        add    esp, 0ch
        cmp    eax, edi
        jne    SHORT loc_2_85AA
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 8]
        call   sub_2_8429
        add    esp, 0ch
        cmp    eax, edi
        jne    SHORT loc_2_85AA
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 0ch]
        call   sub_2_8469
        add    esp, 10h
        cmp    eax, edi
        jne    SHORT loc_2_85AA
        mov    esi, dword ptr [ebp - 4]
        mov    edi, 1
loc_2_85AA:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_85BB
        push   dword ptr [ebp - 0ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_85B8:
        add    esp, 4
loc_2_85BB:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_85CC
        push   dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_85C9:
        add    esp, 4
loc_2_85CC:
        or     edi, edi
        jne    SHORT loc_2_85F1
        push   esi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 14h]
        call   sub_2_6E78
        add    esp, 0ch
        mov    edi, dword ptr [ebp + 14h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebp - 4], ecx
loc_2_85F1:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_2_85FA:
        push   ebp
        mov    ebp, esp
        push   80000001h
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   sub_2_8545
        mov    esp, ebp
        pop    ebp
        ret
_PTEXT ends
        end

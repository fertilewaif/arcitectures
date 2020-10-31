format PE console

; ����������� ���������, ������������ ����������� ��������
; ��������� ����� �������� ������������ ������������������
; t(n) = t(n - 1) + t(n - 2) + t(n - 3) ��� n >= 3 (����� ����������)
; �� ��������� ������� ����� [0,1,1], ������� �� ������� �� ������� ��������� �����

entry _start

include 'win32a.inc'
include 'utils.inc'

section '.data' data readable writable
    ;������� n - 1 ����� ����������
    fib1 dd 1
    ;������� n - 2 ����� ����������
    fib2 dd 1
    ;������� n - 3 ����� ����������
    fib3 dd 0
    ; ������� �������� ����� ����������
    num dd 2
    ; ������ ��� ������ ���������� ���������
    outStr db 'Number of maximum tribonacci number inside machine word is %d', 0
section '.code' code readable writable executable
        _start:
                jmp _main_cycle
        _main_cycle:
                ;eax =0
                xor eax, eax
                add eax, [fib1]
                
                ;������� �� ����� ���� fib1 + fib2 ������� �� ������� ��������� �����
                add eax, [fib2]
                jo _programEnd
                
                ;������� �� ����� ���� fib1 + fib2 + fib3 ������� �� ������� ��������� �����
                add eax, [fib3]
                jo _programEnd

                ;������ � eax ����� ������� ����� ���������� t(num + 1)
                ;fib3 = fib2 ��� ������ ������������� � utils.inc �������
                movMem [fib3], [fib2]
                ;fib2 = fib1 ��� ������ ������������� � utils.inc �������
                movMem [fib2], [fib1]
                ;fib1 = eax ��� ������ ������������� � utils.inc �������
                movMem [fib1], eax

                ;����������� �������� num � ���������� ����
                inc [num]
                jmp _main_cycle

        _programEnd:
                ;������� ��������� �������� ����� ����������
                push [num]
                push outStr
                call [printf]
                call [getch]
                push 0
                call [ExitProcess]

section 'idata' import data readable
        library kernel, 'kernel32.dll', \
                msvcrt, 'msvcrt.dll'
        import kernel, \
               ExitProcess, 'ExitProcess'

        import msvcrt, \
               printf, 'printf', \
               getch, '_getch'

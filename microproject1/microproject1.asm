format PE console

; разработать программу, определяющей максимально значение
; параметра числа линейной рекуррентной последовательности
; t(n) = t(n - 1) + t(n - 2) + t(n - 3) при n >= 3 (числа трибоначчи)
; со стартовой тройкой чисел [0,1,1], которое не выходит за пределы машинного слова

entry _start

include 'win32a.inc'
include 'utils.inc'

section '.data' data readable writable
    ;текущее n - 1 число трибоначчи
    fib1 dd 1
    ;текущее n - 2 число трибоначчи
    fib2 dd 1
    ;текущее n - 3 число трибоначчи
    fib3 dd 0
    ; текущий параметр числа трибоначчи
    num dd 2
    ; строка для вывода найденного параметра
    outStr db 'Number of maximum tribonacci number inside machine word is %d', 0
section '.code' code readable writable executable
        _start:
                jmp _main_cycle
        _main_cycle:
                ;eax =0
                xor eax, eax
                add eax, [fib1]
                
                ;выходим из цикла если fib1 + fib2 выходит за пределы машинного слова
                add eax, [fib2]
                jo _programEnd
                
                ;выходим из цикла если fib1 + fib2 + fib3 выходит за пределы машинного слова
                add eax, [fib3]
                jo _programEnd

                ;теперь в eax лежит текущее число трибоначчи t(num + 1)
                ;fib3 = fib2 при помощи определенного в utils.inc макроса
                movMem [fib3], [fib2]
                ;fib2 = fib1 при помощи определенного в utils.inc макроса
                movMem [fib2], [fib1]
                ;fib1 = eax при помощи определенного в utils.inc макроса
                movMem [fib1], eax

                ;увеличиваем параметр num и продолжаем цикл
                inc [num]
                jmp _main_cycle

        _programEnd:
                ;выводим найденный параметр числа трибоначчи
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

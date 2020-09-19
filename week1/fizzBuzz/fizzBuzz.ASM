format PE console

entry _start

include 'win32a.inc'

section '.data' data readable writable
        fizz db 'Fizz', 13, 10, 0
        buzz db 'Buzz', 13, 10, 0
        fizzbuzz db 'Fizz Buzz', 13, 10, 0
        outputStr db '%s', 0
        promptStr db 'input num: ', 0
        inputStr db '%d', 0
        numStr db '%d', 0
        newLine db 13, 10, 0
        num dd ?
        temp dd 1

section '.code' code readable writable executable
        _start:
                push promptStr
                call [printf]
                ;printf("input num: ")

                push num
                push inputStr
                call [scanf]
                ;scanf("%d"", &num)

                ;mov [temp], 1                            ; set loop counter

                loop_main:
                push [temp]
                push numStr                             ; set print_num arg
                call [printf]
                push newLine
                call [printf]

                xor dx, dx                              ; reset dx
                mov eax, [temp]
                mov cx, 15
                div cx
                cmp dx, 0
                jz div_by_3_5                           ; if divisible by 3 and 5
        
                xor dx, dx                              ; reset dx
                mov eax, [temp]
                mov cx, 3
                div cx
                cmp dx, 0
                jz div_by_3                             ; if divisible by 3
        
                xor dx, dx                              ; reset dx
                mov eax, [temp]
                mov cx, 5
                div cx
                cmp dx, 0
                jz div_by_5                             ; if divisible by 5
        
                ; print number
                ;push temp
                ;push numStr                             ; set print_num arg
                ;call [printf]
                jmp cont
        
                div_by_3_5:                             ; print fizz buzz
                push fizzbuzz
                call [printf]
                jmp cont
        
                div_by_3:                               ; print fizz
                push fizz
                call [printf]
                jmp cont
        
                div_by_5:                               ; print buzz
                push buzz
                call [printf]
                jmp cont
        
                cont:
                mov eax, [temp]
                xor eax, [num]
                cmp eax, 0
                jz exit                                 ; end of loop
                inc [temp]                              ; increment loop counter
                jmp loop_main


                exit:
                call [getch]
                push 0
                call [ExitProcess]

section 'idata' import data readable
        library kernel, 'kernel32.dll', \
                msvcrt, 'msvcrt.dll'
        import kernel, \
               ExitProcess, 'ExitProcess'

        import msvcrt, \
               scanf, 'scanf', \
               printf, 'printf', \
               getch, '_getch'


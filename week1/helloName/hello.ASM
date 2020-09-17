format PE console

entry start

include 'win32a.inc'

section '.data' data readable writable

        promptStr db 'What is your name? ', 0
        inputStr db '%s', 0
        outputStr db 'Hello, %s!', 0

        name dd ?

        NULL = 0


section '.code' code readable executable
        start:
                push promptStr
                call [printf]

                push name
                push inputStr
                call [scanf]

                push name
                push outputStr
                call [printf]

                call [getch]

                push NULL
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

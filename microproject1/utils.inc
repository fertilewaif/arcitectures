; макрос для присовения переменных при помощи регистра(чтобы можно было двигать из "памяти" в "память")
macro movMem mem1, mem2
{
    ;mem1 = mem2

    ;ebx = mem2
    mov ebx, mem2
    ;mem1 = ebx
    mov mem1, ebx
}
; ������ ��� ���������� ���������� ��� ������ ��������(����� ����� ���� ������� �� "������" � "������")
macro movMem mem1, mem2
{
    ;mem1 = mem2

    ;ebx = mem2
    mov ebx, mem2
    ;mem1 = ebx
    mov mem1, ebx
}
[org 0x1000]

dd 0x55aa; ħ���������жϴ���

; ��ӡ�ַ���
mov si, loading
call print

; xchg bx, bx

detect_memory:
    ; �� ebx ��Ϊ 0
    xor ebx, ebx

    ; es:di �ṹ��Ļ���λ��
    mov ax, 0
    mov es, ax
    mov edi, ards_buffer

    mov edx, 0x534d4150; �̶�ǩ��

.next:
    ; �ӹ��ܺ�
    mov eax, 0xe820
    ; ards �ṹ�Ĵ�С (�ֽ�)
    mov ecx, 20
    ; ���� 0x15 ϵͳ����
    int 0x15

    ; ��� CF ��λ����ʾ����
    jc error

    ; ������ָ��ָ����һ���ṹ��
    add di, cx

    ; ���ṹ��������һ
    inc dword [ards_count]

    cmp ebx, 0
    jnz .next

    mov si, detecting
    call print

    ; xchg bx, bx

    ; mov byte [0xb8000], 'P'

    jmp prepare_protected_mode

prepare_protected_mode:
    ; xchg bx, bx; �ϵ�

    cli; �ر��ж�

    ; �� A20 ��
    in al,  0x92
    or al, 0b10
    out 0x92, al

    lgdt [gdt_ptr]; ���� gdt

    ; ��������ģʽ
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; ����ת��ˢ�»��棬���ñ���ģʽ
    jmp dword code_selector:protect_mode


print:
    mov ah, 0x0e; ���� AH �Ĵ�����׼������ BIOS ��Ƶ������ʾ�ַ�
.next:
    mov al, [si]
    cmp al, 0
    jz .done
    int 0x10
    inc si
    jmp .next
.done:
    ret

loading:
    db "Loading Onix...", 10, 13, 0; \n\r
detecting:
    db "Detecting Memory Success...", 10, 13, 0; \n\r

error:
    mov si, .msg
    call print
    hlt; �� CPU ֹͣ
    jmp $
    .msg db "Loading Error!!!", 10, 13, 0


[bits 32]; �Ѿ�����32λ
protect_mode:
    ; xchg bx, bx; �ϵ�
    mov ax, data_selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax; ��ʼ���μĴ���

    mov esp, 0x10000; �޸�ջ��

    mov edi, 0x10000; ��ȡ��Ŀ���ڴ�
    mov ecx, 10; ��ʼ����
    mov bl, 200; ��������

    call read_disk ; ��ȡ�ں�

    mov eax, 0x20220205; �ں�ħ��
    mov ebx, ards_count; ards ����ָ��

    jmp dword code_selector:0x10000

    ud2; ��ʾ����


read_disk:

    ; ���ö�д����������
    mov dx, 0x1f2
    mov al, bl
    out dx, al

    inc dx; 0x1f3
    mov al, cl; ��ʼ������ǰ��λ
    out dx, al

    inc dx; 0x1f4
    shr ecx, 8
    mov al, cl; ��ʼ�������а�λ
    out dx, al

    inc dx; 0x1f5
    shr ecx, 8
    mov al, cl; ��ʼ�����ĸ߰�λ
    out dx, al

    inc dx; 0x1f6
    shr ecx, 8
    and cl, 0b1111; ������λ��Ϊ 0

    mov al, 0b1110_0000;
    or al, cl
    out dx, al; ���� - LBA ģʽ

    inc dx; 0x1f7
    mov al, 0x20; ��Ӳ��
    out dx, al

    xor ecx, ecx; �� ecx ���
    mov cl, bl; �õ���д����������

    .read:
        push cx; ���� cx
        call .waits; �ȴ�����׼�����
        call .reads; ��ȡһ������
        pop cx; �ָ� cx
        loop .read; ÿ��loop��дһ������

    ret

    .waits:
        mov dx, 0x1f7
        .check:
            in al, dx
            jmp $+2; nop ֱ����ת����һ��
            jmp $+2; һ����ӳ�
            jmp $+2
            and al, 0b1000_1000
            cmp al, 0b0000_1000
            jnz .check
        ret

    .reads:
        mov dx, 0x1f0
        mov cx, 256; һ������ 256 ��
        .readw:
            in ax, dx
            jmp $+2; һ����ӳ� 
            jmp $+2
            jmp $+2
            mov [edi], ax
            add edi, 2
            loop .readw; ÿ��loop��ȡһ����
        ret



; ��ѡ���_��Linux�ںˣ���ѡ���ʾ��
code_selector equ (1 << 3)
data_selector equ (2 << 3)

memory_base equ 0; �ڴ濪ʼ��λ�ã�����ַ

; �ڴ���� 4G / 4K - 1
memory_limit equ ((1024 * 1024 * 1024 * 4) / (1024 * 4)) - 1

gdt_ptr: ;NULL ������: 8 �ֽ� ����������� (gdt_code): 8 �ֽ� ���ݶ������� (gdt_data): 8 �ֽ� (gdt_end - gdt_base) = 24 �ֽ�
    dw (gdt_end - gdt_base) - 1
    dd gdt_base
gdt_base:
    dd 0, 0; NULL ������
gdt_code:
    dw memory_limit & 0xffff; �ν��� 0 ~ 15 λ
    dw memory_base & 0xffff; ����ַ 0 ~ 15 λ
    db (memory_base >> 16) & 0xff; ����ַ 16 ~ 23 λ
    ; ���� - dlp 0 - S _ ���� - ������ - �ɶ� - û�б����ʹ�
    db 0b_1_00_1_1_0_1_0;
    ; 4k - 32 λ - ���� 64 λ - �ν��� 16 ~ 19
    db 0b1_1_0_0_0000 | (memory_limit >> 16) & 0xf;
    db (memory_base >> 24) & 0xff; ����ַ 24 ~ 31 λ
gdt_data:
    dw memory_limit & 0xffff; �ν��� 0 ~ 15 λ
    dw memory_base & 0xffff; ����ַ 0 ~ 15 λ
    db (memory_base >> 16) & 0xff; ����ַ 16 ~ 23 λ
    ; ���� - dlp 0 - S _ ���� - ���� - ��д - û�б����ʹ�
    db 0b_1_00_1_0_0_1_0;
    ; 4k - 32 λ - ���� 64 λ - �ν��� 16 ~ 19
    db 0b1_1_0_0_0000 | (memory_limit >> 16) & 0xf;
    db (memory_base >> 24) & 0xff; ����ַ 24 ~ 31 λ
gdt_end:

ards_count:
    dd 0
ards_buffer:


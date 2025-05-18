section .text
global findWord
extern myStrncmp

; ErrorNum findWord(HashTableInfo* hash_table, char* buffer, size_t hash, size_t length, int* value)
findWord:
    push r12
    push r13
    push r14
    push r15
    push rbx

    ; Сохраняем аргументы
    mov r12, rdi        ; hash_table
    mov r13, rsi        ; buffer
    mov r14, rdx        ; hash
    mov r15, rcx        ; length
    mov rbx, r8         ; value (5-й аргумент, теперь в rbx)

    ; Получаем bucket[hash].list.cell[0].next
    mov rax, [r12]      ; hash_table->bucket
    imul rdx, r14, 32   ; hash * sizeof(Bucket_t) = 32
    add rax, rdx        ; bucket[hash]
    mov rsi, [rax]      ; bucket.list.cell (ListCell_t*)
    mov edi, [rsi + 4]  ; cell[0].next (offset 4)

.loop:
    test edi, edi       ; i == 0?
    jz .success

    ; Вычисляем адрес cell[i]
    mov eax, edi        ; i
    imul rax, rax, 12   ; sizeof(ListCell_t) = 12
    lea r10, [rsi + rax] ; &cell[i]
    
    mov r11d, [r10] ; Сохраняем cell[i].data

    push r11
    ; Вызов myStrncmp
    mov rdx, [r12 + 8]  ; hash_table->buffer
    add rdx, r11        ; buffer + cell[i].data
    mov rdi, rdx        ; 1-й аргумент myStrncmp
    mov rsi, r13        ; 2-й аргумент (buffer)
    mov rdx, r15        ; 3-й аргумент (length)
    call myStrncmp
    pop r11

    test eax, eax
    jnz .next_iter

    ; Проверка на '\0'
    mov r8, [r12 + 8]   ; hash_table->buffer
    add r8, r11         ; + cell[i].data
    add r8, r15         ; + length
    cmp byte [r8], 0
    jne .next_iter

    ; Нашли совпадение
    mov rax, [r12]       ; hash_table->bucket
    imul rdx, r14, 32    ; hash * 32
    add rax, rdx         ; bucket[hash]
    inc dword [rax + 20] ; num_occurrences++ (offset 20)
    mov eax, r11d        ; cell[i].data
    mov [rbx], eax       ; *value = data
    jmp .success

.next_iter:
    mov edi, [r10 + 4]  ; cell[i].next (offset 4)
    mov rax, [r12]      ; Восстанавливаем rsi
    imul rdx, r14, 32   
    add rax, rdx
    mov rsi, [rax]      
    jmp .loop

.success:
    xor eax, eax        ; NO_ERROR
    pop rbx
    pop r15
    pop r14
    pop r13
    pop r12
    ret
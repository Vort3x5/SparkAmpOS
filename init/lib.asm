; asmsyntax=fasm

format ELF

public __udivdi3
public __umoddi3

__udivdi3:
    push ebp
    mov ebp, esp
    push ebx
    push esi
    
    ; Load divisor
    mov ebx, [ebp+16]  ; divisor low
    mov ecx, [ebp+20]  ; divisor high
    
    ; Check if divisor is 32-bit (high part is 0)
    test ecx, ecx
    jnz .complex_division
    
    ; Load dividend into EDX:EAX
    mov edx, [ebp+12]  ; dividend high
    mov eax, [ebp+8]   ; dividend low
    
    ; Check if quotient will fit in 32 bits
    cmp edx, ebx
    jae .two_step_division
    
    ; Quotient fits in 32 bits - simple division
    div ebx            ; EDX:EAX / EBX → quotient in EAX, remainder in EDX
    xor edx, edx       ; quotient high = 0
    jmp .done
    
	.two_step_division:
		; Step 1: divide high part
		mov eax, edx       ; dividend high
		xor edx, edx       ; clear EDX
		div ebx            ; dividend_high / divisor → quotient_high in EAX
		mov esi, eax       ; save quotient_high
		
		; Step 2: divide low part with remainder
		mov eax, [ebp+8]   ; dividend low
		div ebx            ; (remainder:dividend_low) / divisor
		
		; Combine results: ESI = high, EAX = low
		mov edx, esi
		jmp .done
		
	.complex_division:
		; 64-bit / 64-bit division - use subtraction loop
		; Initialize quotient to 0
		xor esi, esi       ; quotient low = 0
		xor edi, edi       ; quotient high = 0
		
	.division_loop:
		; Compare dividend with divisor (64-bit comparison)
		mov eax, [ebp+12]  ; dividend high
		cmp eax, ecx       ; compare with divisor high
		jb .division_done  ; dividend < divisor
		ja .do_subtraction ; dividend > divisor
		
		; High parts equal - compare low parts
		mov eax, [ebp+8]   ; dividend low  
		cmp eax, ebx       ; compare with divisor low
		jb .division_done  ; dividend < divisor
		
	.do_subtraction:
		; Subtract divisor from dividend
		mov eax, [ebp+8]   ; dividend low
		sub eax, ebx       ; subtract divisor low
		mov [ebp+8], eax   ; store back
		
		mov eax, [ebp+12]  ; dividend high
		sbb eax, ecx       ; subtract divisor high with borrow
		mov [ebp+12], eax  ; store back
		
		; Increment quotient (64-bit increment)
		inc esi            ; quotient low++
		adc edi, 0         ; quotient high += carry
		
		jmp .division_loop
		
	.division_done:
		mov eax, esi       ; quotient low
		mov edx, edi       ; quotient high
		
	.done:
		pop esi
		pop ebx
		pop ebp
		ret

; u64 __umoddi3(u64 dividend, u64 divisor)  
; Returns dividend % divisor
__umoddi3:
    push ebp
    mov ebp, esp
    push ebx
    push esi
    push edi
    
    mov ebx, [ebp+16]  ; divisor low
    mov ecx, [ebp+20]  ; divisor high
    
    ; Check if divisor is 32-bit
    test ecx, ecx
    jnz .modulo_loop
    
    mov edx, [ebp+12]  ; dividend high
    mov eax, [ebp+8]   ; dividend low
    
    ; Check if we need two-step division
    cmp edx, ebx
    jae .two_step_modulo
    
    div ebx            ; EDX:EAX / EBX, remainder in EDX
    mov eax, edx       ; remainder (32-bit)
    xor edx, edx       ; remainder high = 0
    jmp .modulo_done
    
	.two_step_modulo:
		mov eax, edx       ; dividend high
		xor edx, edx
		div ebx            ; dividend_high / divisor
		
		mov eax, [ebp+8]   ; dividend low
		div ebx            ; (remainder:dividend_low) / divisor
		
		mov eax, edx       ; final remainder
		xor edx, edx       ; remainder high = 0
		jmp .modulo_done
		
	.modulo_loop:
		mov eax, [ebp+12]  ; dividend high
		cmp eax, ecx       ; compare with divisor high
		jb .modulo_done_complex
		ja .do_modulo_subtraction
		
		mov eax, [ebp+8]   ; dividend low
		cmp eax, ebx       ; compare with divisor low  
		jb .modulo_done_complex
		
	.do_modulo_subtraction:
		mov eax, [ebp+8]
		sub eax, ebx
		mov [ebp+8], eax
		
		mov eax, [ebp+12]
		sbb eax, ecx
		mov [ebp+12], eax
		
		jmp .modulo_loop
		
	.modulo_done_complex:
		mov eax, [ebp+8]   ; remainder low
		mov edx, [ebp+12]  ; remainder high
		
	.modulo_done:
		pop edi
		pop esi
		pop ebx
		pop ebp
		ret

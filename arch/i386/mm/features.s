.code32
.section .text
.align 4

.global _detect_sse
.type _detect_sse, @function
_detect_sse:
	mov		$0x1, %eax
	cpuid
	test	$0x4000000, %edx
	je		.no_sse
	mov		$0x1, %eax
	ret
.no_sse:
	mov		$0x0, %eax
	ret

.global _enable_sse
.type _enable_sse, @function
_enable_sse:
	mov    $0x600, %eax
	mov    %eax, %cr4
	mov    %cr0, %eax
	and    $0xfffffffb, %eax
	or     $0x2, %eax
	mov    %eax, %cr0
	ret

.global _sse_memcpy
.type _sse_memcpy, @function
_sse_memcpy:
	mov    0x4(%esp), %edi
	mov    0x8(%esp), %esi
	mov    0xc(%esp), %ecx
.loop:
	movdqu (%esi), %xmm0
	movdqu 0x10(%esi), %xmm1
	movdqu 0x20(%esi), %xmm2
	movdqu 0x30(%esi), %xmm3
	movdqu 0x40(%esi), %xmm4
	movdqu 0x50(%esi), %xmm5
	movdqu 0x60(%esi), %xmm6
	movdqu 0x70(%esi), %xmm7
	movdqu %xmm0, (%edi)
	movdqu %xmm1, 0x10(%edi)
	movdqu %xmm2, 0x20(%edi)
	movdqu %xmm3, 0x30(%edi)
	movdqu %xmm4, 0x40(%edi)
	movdqu %xmm5, 0x50(%edi)
	movdqu %xmm6, 0x60(%edi)
	movdqu %xmm7, 0x70(%edi)
	add    $0x80, %esi
	add    $0x80, %edi
	loop   .loop

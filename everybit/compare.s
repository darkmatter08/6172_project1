	.file	"compare.c"
	.text
	.p2align 4,,15
	.globl	reverse_lookup
	.type	reverse_lookup, @function
reverse_lookup:
.LFB0:
	.cfi_startproc
	movq	%rdi, %rdx
	movabsq	$6148914691236517205, %rax
	movabsq	$3689348814741910323, %rcx
	shrq	%rdx
	andq	%rax, %rdx
	andq	%rdi, %rax
	addq	%rax, %rax
	orq	%rdx, %rax
	movq	%rax, %rdx
	andq	%rcx, %rax
	shrq	$2, %rdx
	salq	$2, %rax
	andq	%rcx, %rdx
	movabsq	$1085102592571150095, %rcx
	orq	%rdx, %rax
	movq	%rax, %rdx
	andq	%rcx, %rax
	shrq	$4, %rdx
	salq	$4, %rax
	andq	%rcx, %rdx
	movabsq	$71777214294589695, %rcx
	orq	%rdx, %rax
	movq	%rax, %rdx
	andq	%rcx, %rax
	shrq	$8, %rdx
	salq	$8, %rax
	andq	%rcx, %rdx
	movabsq	$281470681808895, %rcx
	orq	%rdx, %rax
	movq	%rax, %rdx
	andq	%rcx, %rax
	shrq	$16, %rdx
	salq	$16, %rax
	andq	%rcx, %rdx
	orq	%rdx, %rax
	rorq	$32, %rax
	ret
	.cfi_endproc
.LFE0:
	.size	reverse_lookup, .-reverse_lookup
	.p2align 4,,15
	.globl	reverse_lookup_fast
	.type	reverse_lookup_fast, @function
reverse_lookup_fast:
.LFB1:
	.cfi_startproc
	movzbl	%dil, %eax
	movq	%rdi, %rdx
	movq	%rdi, %rcx
	movzbl	BitReverseTable256(%rax), %eax
	shrq	$56, %rdx
	movzbl	BitReverseTable256(%rdx), %edx
	salq	$56, %rax
	orq	%rdx, %rax
	movzbl	%ch, %edx
	movzbl	BitReverseTable256(%rdx), %edx
	salq	$48, %rdx
	orq	%rdx, %rax
	movq	%rdi, %rdx
	shrq	$16, %rdx
	movzbl	%dl, %edx
	movzbl	BitReverseTable256(%rdx), %edx
	salq	$40, %rdx
	orq	%rdx, %rax
	movq	%rdi, %rdx
	shrq	$24, %rdx
	movzbl	%dl, %edx
	movzbl	BitReverseTable256(%rdx), %edx
	salq	$32, %rdx
	orq	%rdx, %rax
	movq	%rdi, %rdx
	shrq	$32, %rdx
	movzbl	%dl, %edx
	movzbl	BitReverseTable256(%rdx), %edx
	salq	$24, %rdx
	orq	%rdx, %rax
	movq	%rdi, %rdx
	shrq	$48, %rdi
	shrq	$40, %rdx
	movzbl	%dil, %edi
	movzbl	%dl, %edx
	movzbl	BitReverseTable256(%rdx), %edx
	salq	$16, %rdx
	orq	%rdx, %rax
	movzbl	BitReverseTable256(%rdi), %edx
	salq	$8, %rdx
	orq	%rdx, %rax
	ret
	.cfi_endproc
.LFE1:
	.size	reverse_lookup_fast, .-reverse_lookup_fast
	.globl	BitReverseTable256
	.section	.rodata
	.align 32
	.type	BitReverseTable256, @object
	.size	BitReverseTable256, 256
BitReverseTable256:
	.byte	0
	.byte	-128
	.byte	64
	.byte	-64
	.byte	32
	.byte	-96
	.byte	96
	.byte	-32
	.byte	16
	.byte	-112
	.byte	80
	.byte	-48
	.byte	48
	.byte	-80
	.byte	112
	.byte	-16
	.byte	8
	.byte	-120
	.byte	72
	.byte	-56
	.byte	40
	.byte	-88
	.byte	104
	.byte	-24
	.byte	24
	.byte	-104
	.byte	88
	.byte	-40
	.byte	56
	.byte	-72
	.byte	120
	.byte	-8
	.byte	4
	.byte	-124
	.byte	68
	.byte	-60
	.byte	36
	.byte	-92
	.byte	100
	.byte	-28
	.byte	20
	.byte	-108
	.byte	84
	.byte	-44
	.byte	52
	.byte	-76
	.byte	116
	.byte	-12
	.byte	12
	.byte	-116
	.byte	76
	.byte	-52
	.byte	44
	.byte	-84
	.byte	108
	.byte	-20
	.byte	28
	.byte	-100
	.byte	92
	.byte	-36
	.byte	60
	.byte	-68
	.byte	124
	.byte	-4
	.byte	2
	.byte	-126
	.byte	66
	.byte	-62
	.byte	34
	.byte	-94
	.byte	98
	.byte	-30
	.byte	18
	.byte	-110
	.byte	82
	.byte	-46
	.byte	50
	.byte	-78
	.byte	114
	.byte	-14
	.byte	10
	.byte	-118
	.byte	74
	.byte	-54
	.byte	42
	.byte	-86
	.byte	106
	.byte	-22
	.byte	26
	.byte	-102
	.byte	90
	.byte	-38
	.byte	58
	.byte	-70
	.byte	122
	.byte	-6
	.byte	6
	.byte	-122
	.byte	70
	.byte	-58
	.byte	38
	.byte	-90
	.byte	102
	.byte	-26
	.byte	22
	.byte	-106
	.byte	86
	.byte	-42
	.byte	54
	.byte	-74
	.byte	118
	.byte	-10
	.byte	14
	.byte	-114
	.byte	78
	.byte	-50
	.byte	46
	.byte	-82
	.byte	110
	.byte	-18
	.byte	30
	.byte	-98
	.byte	94
	.byte	-34
	.byte	62
	.byte	-66
	.byte	126
	.byte	-2
	.byte	1
	.byte	-127
	.byte	65
	.byte	-63
	.byte	33
	.byte	-95
	.byte	97
	.byte	-31
	.byte	17
	.byte	-111
	.byte	81
	.byte	-47
	.byte	49
	.byte	-79
	.byte	113
	.byte	-15
	.byte	9
	.byte	-119
	.byte	73
	.byte	-55
	.byte	41
	.byte	-87
	.byte	105
	.byte	-23
	.byte	25
	.byte	-103
	.byte	89
	.byte	-39
	.byte	57
	.byte	-71
	.byte	121
	.byte	-7
	.byte	5
	.byte	-123
	.byte	69
	.byte	-59
	.byte	37
	.byte	-91
	.byte	101
	.byte	-27
	.byte	21
	.byte	-107
	.byte	85
	.byte	-43
	.byte	53
	.byte	-75
	.byte	117
	.byte	-11
	.byte	13
	.byte	-115
	.byte	77
	.byte	-51
	.byte	45
	.byte	-83
	.byte	109
	.byte	-19
	.byte	29
	.byte	-99
	.byte	93
	.byte	-35
	.byte	61
	.byte	-67
	.byte	125
	.byte	-3
	.byte	3
	.byte	-125
	.byte	67
	.byte	-61
	.byte	35
	.byte	-93
	.byte	99
	.byte	-29
	.byte	19
	.byte	-109
	.byte	83
	.byte	-45
	.byte	51
	.byte	-77
	.byte	115
	.byte	-13
	.byte	11
	.byte	-117
	.byte	75
	.byte	-53
	.byte	43
	.byte	-85
	.byte	107
	.byte	-21
	.byte	27
	.byte	-101
	.byte	91
	.byte	-37
	.byte	59
	.byte	-69
	.byte	123
	.byte	-5
	.byte	7
	.byte	-121
	.byte	71
	.byte	-57
	.byte	39
	.byte	-89
	.byte	103
	.byte	-25
	.byte	23
	.byte	-105
	.byte	87
	.byte	-41
	.byte	55
	.byte	-73
	.byte	119
	.byte	-9
	.byte	15
	.byte	-113
	.byte	79
	.byte	-49
	.byte	47
	.byte	-81
	.byte	111
	.byte	-17
	.byte	31
	.byte	-97
	.byte	95
	.byte	-33
	.byte	63
	.byte	-65
	.byte	127
	.byte	-1
	.ident	"GCC: (GNU) 4.8.1 20130520 (prerelease)"
	.section	.note.GNU-stack,"",@progbits

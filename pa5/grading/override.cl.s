# start of generated code
	.data
	.align	2
	.globl	class_nameTab
	.globl	Main_protObj
	.globl	Int_protObj
	.globl	String_protObj
	.globl	bool_const0
	.globl	bool_const1
	.globl	_int_tag
	.globl	_bool_tag
	.globl	_string_tag
	.globl	class_parentTab
_int_tag:
	.word	1
_bool_tag:
	.word	2
_string_tag:
	.word	0
	.globl	_MemMgr_INITIALIZER
_MemMgr_INITIALIZER:
	.word	_NoGC_Init
	.globl	_MemMgr_COLLECTOR
_MemMgr_COLLECTOR:
	.word	_NoGC_Collect
	.globl	_MemMgr_TEST
_MemMgr_TEST:
	.word	0
	.word	-1
str_const16:
	.word	0
	.word	6
	.word	String_dispTab
	.word	int_const3
	.ascii	"Main"
	.byte	0	
	.align	2
	.word	-1
str_const15:
	.word	0
	.word	5
	.word	String_dispTab
	.word	int_const0
	.ascii	"D"
	.byte	0	
	.align	2
	.word	-1
str_const14:
	.word	0
	.word	5
	.word	String_dispTab
	.word	int_const0
	.ascii	"C"
	.byte	0	
	.align	2
	.word	-1
str_const13:
	.word	0
	.word	5
	.word	String_dispTab
	.word	int_const0
	.ascii	"B"
	.byte	0	
	.align	2
	.word	-1
str_const12:
	.word	0
	.word	5
	.word	String_dispTab
	.word	int_const0
	.ascii	"A"
	.byte	0	
	.align	2
	.word	-1
str_const11:
	.word	0
	.word	6
	.word	String_dispTab
	.word	int_const5
	.ascii	"String"
	.byte	0	
	.align	2
	.word	-1
str_const10:
	.word	0
	.word	6
	.word	String_dispTab
	.word	int_const3
	.ascii	"Bool"
	.byte	0	
	.align	2
	.word	-1
str_const9:
	.word	0
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"Int"
	.byte	0	
	.align	2
	.word	-1
str_const8:
	.word	0
	.word	5
	.word	String_dispTab
	.word	int_const1
	.ascii	"IO"
	.byte	0	
	.align	2
	.word	-1
str_const7:
	.word	0
	.word	6
	.word	String_dispTab
	.word	int_const5
	.ascii	"Object"
	.byte	0	
	.align	2
	.word	-1
str_const6:
	.word	0
	.word	7
	.word	String_dispTab
	.word	int_const7
	.ascii	"_prim_slot"
	.byte	0	
	.align	2
	.word	-1
str_const5:
	.word	0
	.word	7
	.word	String_dispTab
	.word	int_const8
	.ascii	"SELF_TYPE"
	.byte	0	
	.align	2
	.word	-1
str_const4:
	.word	0
	.word	7
	.word	String_dispTab
	.word	int_const8
	.ascii	"_no_class"
	.byte	0	
	.align	2
	.word	-1
str_const3:
	.word	0
	.word	8
	.word	String_dispTab
	.word	int_const9
	.ascii	"<basic class>"
	.byte	0	
	.align	2
	.word	-1
str_const2:
	.word	0
	.word	5
	.word	String_dispTab
	.word	int_const6
	.byte	0	
	.align	2
	.word	-1
str_const1:
	.word	0
	.word	5
	.word	String_dispTab
	.word	int_const0
	.ascii	"\n"
	.byte	0	
	.align	2
	.word	-1
str_const0:
	.word	0
	.word	8
	.word	String_dispTab
	.word	int_const9
	.ascii	"./override.cl"
	.byte	0	
	.align	2
	.word	-1
int_const9:
	.word	1
	.word	4
	.word	Int_dispTab
	.word	13
	.word	-1
int_const8:
	.word	1
	.word	4
	.word	Int_dispTab
	.word	9
	.word	-1
int_const7:
	.word	1
	.word	4
	.word	Int_dispTab
	.word	10
	.word	-1
int_const6:
	.word	1
	.word	4
	.word	Int_dispTab
	.word	0
	.word	-1
int_const5:
	.word	1
	.word	4
	.word	Int_dispTab
	.word	6
	.word	-1
int_const4:
	.word	1
	.word	4
	.word	Int_dispTab
	.word	5
	.word	-1
int_const3:
	.word	1
	.word	4
	.word	Int_dispTab
	.word	4
	.word	-1
int_const2:
	.word	1
	.word	4
	.word	Int_dispTab
	.word	3
	.word	-1
int_const1:
	.word	1
	.word	4
	.word	Int_dispTab
	.word	2
	.word	-1
int_const0:
	.word	1
	.word	4
	.word	Int_dispTab
	.word	1
	.word	-1
bool_const0:
	.word	2
	.word	4
	.word	Bool_dispTab
	.word	0
	.word	-1
bool_const1:
	.word	2
	.word	4
	.word	Bool_dispTab
	.word	1
class_nameTab:
	.word	str_const11
	.word	str_const9
	.word	str_const10
	.word	str_const16
	.word	str_const15
	.word	str_const14
	.word	str_const13
	.word	str_const12
	.word	str_const8
	.word	str_const7
Main_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	IO.out_string
	.word	IO.out_int
	.word	IO.in_string
	.word	IO.in_int
	.word	Main.main
D_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	D.f
	.word	D.g
C_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	C.f
	.word	B.g
B_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	A.f
	.word	B.g
A_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	A.f
	.word	A.g
String_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	String.length
	.word	String.concat
	.word	String.substr
Bool_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
Int_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
IO_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	IO.out_string
	.word	IO.out_int
	.word	IO.in_string
	.word	IO.in_int
Object_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
class_parentTab:
	.word	9
	.word	9
	.word	9
	.word	8
	.word	5
	.word	6
	.word	7
	.word	9
	.word	9
	.word	-2
	.word	-1
Main_protObj:
	.word	3
	.word	7
	.word	Main_dispTab
	.word	0
	.word	0
	.word	0
	.word	0
	.word	-1
D_protObj:
	.word	4
	.word	3
	.word	D_dispTab
	.word	-1
C_protObj:
	.word	5
	.word	3
	.word	C_dispTab
	.word	-1
B_protObj:
	.word	6
	.word	3
	.word	B_dispTab
	.word	-1
A_protObj:
	.word	7
	.word	3
	.word	A_dispTab
	.word	-1
String_protObj:
	.word	0
	.word	5
	.word	String_dispTab
	.word	int_const6
	.word	0
	.word	-1
Bool_protObj:
	.word	2
	.word	4
	.word	Bool_dispTab
	.word	0
	.word	-1
Int_protObj:
	.word	1
	.word	4
	.word	Int_dispTab
	.word	0
	.word	-1
IO_protObj:
	.word	8
	.word	3
	.word	IO_dispTab
	.word	-1
Object_protObj:
	.word	9
	.word	3
	.word	Object_dispTab
	.globl	heap_start
heap_start:
	.word	0
	.text
	.globl	Main_init
	.globl	Int_init
	.globl	String_init
	.globl	Bool_init
	.globl	Main.main
Main_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	move	$s0 $a0
	jal	IO_init
	la	$a0 A_protObj
	jal	Object.copy
	jal	A_init
	sw	$a0 12($s0)
	la	$a0 B_protObj
	jal	Object.copy
	jal	B_init
	sw	$a0 16($s0)
	la	$a0 C_protObj
	jal	Object.copy
	jal	C_init
	sw	$a0 20($s0)
	la	$a0 D_protObj
	jal	Object.copy
	jal	D_init
	sw	$a0 24($s0)
	move	$a0 $s0
	lw	$s0 4($sp)
	lw	$ra 8($sp)
	lw	$fp 12($sp)
	addiu	$sp $sp 12
	jr	$ra	
D_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	move	$s0 $a0
	jal	C_init
	move	$a0 $s0
	lw	$s0 4($sp)
	lw	$ra 8($sp)
	lw	$fp 12($sp)
	addiu	$sp $sp 12
	jr	$ra	
C_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	move	$s0 $a0
	jal	B_init
	move	$a0 $s0
	lw	$s0 4($sp)
	lw	$ra 8($sp)
	lw	$fp 12($sp)
	addiu	$sp $sp 12
	jr	$ra	
B_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	move	$s0 $a0
	jal	A_init
	move	$a0 $s0
	lw	$s0 4($sp)
	lw	$ra 8($sp)
	lw	$fp 12($sp)
	addiu	$sp $sp 12
	jr	$ra	
A_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$s0 4($sp)
	lw	$ra 8($sp)
	lw	$fp 12($sp)
	addiu	$sp $sp 12
	jr	$ra	
String_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$s0 4($sp)
	lw	$ra 8($sp)
	lw	$fp 12($sp)
	addiu	$sp $sp 12
	jr	$ra	
Bool_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$s0 4($sp)
	lw	$ra 8($sp)
	lw	$fp 12($sp)
	addiu	$sp $sp 12
	jr	$ra	
Int_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$s0 4($sp)
	lw	$ra 8($sp)
	lw	$fp 12($sp)
	addiu	$sp $sp 12
	jr	$ra	
IO_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	move	$s0 $a0
	jal	Object_init
	move	$a0 $s0
	lw	$s0 4($sp)
	lw	$ra 8($sp)
	lw	$fp 12($sp)
	addiu	$sp $sp 12
	jr	$ra	
Object_init:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$fp $sp
	move	$s0 $a0
	move	$a0 $s0
	lw	$s0 4($sp)
	lw	$ra 8($sp)
	lw	$fp 12($sp)
	addiu	$sp $sp 12
	jr	$ra	
Main.main:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	addiu	$fp $sp 8
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$s0 $a0
	lw	$a0 12($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label0
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label0:
	lw	$t1 8($a0)
	lw	$t2 12($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label1
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label1:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	lw	$a0 12($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label2
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label2:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label3
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label3:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	lw	$a0 16($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label4
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label4:
	lw	$t1 8($a0)
	lw	$t2 12($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label5
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label5:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	lw	$a0 16($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label6
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label6:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label7
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label7:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	lw	$a0 20($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label8
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label8:
	lw	$t1 8($a0)
	lw	$t2 12($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label9
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label9:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	lw	$a0 20($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label10
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label10:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label11
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label11:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	lw	$a0 24($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label12
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label12:
	lw	$t1 8($a0)
	lw	$t2 12($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label13
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label13:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	lw	$a0 24($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label14
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label14:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label15
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label15:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	li	$t1 0
	bne	$a0 $t1 label16
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label16:
	lw	$a0 12($s0)
	la	$t1 class_objTab
#code reached 01
	li	$t2 7
	sll	$t2 $t2 3
	add	$t1 $t1 $t2
	lw	$t1 0($t1)
	lw	$t1 8($t1)
	lw	$t1 12($t1)
	jalr		$t1
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label17
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label17:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	li	$t1 0
	bne	$a0 $t1 label18
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label18:
	lw	$a0 12($s0)
	la	$t1 class_objTab
#code reached 01
	li	$t2 7
	sll	$t2 $t2 3
	add	$t1 $t1 $t2
	lw	$t1 0($t1)
	lw	$t1 8($t1)
	lw	$t1 16($t1)
	jalr		$t1
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label19
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label19:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	li	$t1 0
	bne	$a0 $t1 label20
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label20:
	lw	$a0 16($s0)
	la	$t1 class_objTab
#code reached 01
	li	$t2 7
	sll	$t2 $t2 3
	add	$t1 $t1 $t2
	lw	$t1 0($t1)
	lw	$t1 8($t1)
	lw	$t1 12($t1)
	jalr		$t1
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label21
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label21:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	li	$t1 0
	bne	$a0 $t1 label22
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label22:
	lw	$a0 16($s0)
	la	$t1 class_objTab
#code reached 01
	li	$t2 7
	sll	$t2 $t2 3
	add	$t1 $t1 $t2
	lw	$t1 0($t1)
	lw	$t1 8($t1)
	lw	$t1 16($t1)
	jalr		$t1
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label23
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label23:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	li	$t1 0
	bne	$a0 $t1 label24
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label24:
	lw	$a0 20($s0)
	la	$t1 class_objTab
#code reached 01
	li	$t2 6
	sll	$t2 $t2 3
	add	$t1 $t1 $t2
	lw	$t1 0($t1)
	lw	$t1 8($t1)
	lw	$t1 12($t1)
	jalr		$t1
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label25
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label25:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	li	$t1 0
	bne	$a0 $t1 label26
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label26:
	lw	$a0 20($s0)
	la	$t1 class_objTab
#code reached 01
	li	$t2 6
	sll	$t2 $t2 3
	add	$t1 $t1 $t2
	lw	$t1 0($t1)
	lw	$t1 8($t1)
	lw	$t1 16($t1)
	jalr		$t1
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label27
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label27:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	li	$t1 0
	bne	$a0 $t1 label28
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label28:
	lw	$a0 24($s0)
	la	$t1 class_objTab
#code reached 01
	li	$t2 5
	sll	$t2 $t2 3
	add	$t1 $t1 $t2
	lw	$t1 0($t1)
	lw	$t1 8($t1)
	lw	$t1 12($t1)
	jalr		$t1
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label29
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label29:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	li	$t1 0
	bne	$a0 $t1 label30
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label30:
	lw	$a0 24($s0)
	la	$t1 class_objTab
#code reached 01
	li	$t2 5
	sll	$t2 $t2 3
	add	$t1 $t1 $t2
	lw	$t1 0($t1)
	lw	$t1 8($t1)
	lw	$t1 16($t1)
	jalr		$t1
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label31
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label31:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	la	$a0 B_protObj
	jal	Object.copy
	jal	B_init
	sw	$a0 12($s0)
	la	$a0 C_protObj
	jal	Object.copy
	jal	C_init
	sw	$a0 16($s0)
	la	$a0 D_protObj
	jal	Object.copy
	jal	D_init
	sw	$a0 20($s0)
	lw	$a0 12($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label32
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label32:
	lw	$t1 8($a0)
	lw	$t2 12($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label33
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label33:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	lw	$a0 12($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label34
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label34:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label35
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label35:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	lw	$a0 16($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label36
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label36:
	lw	$t1 8($a0)
	lw	$t2 12($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label37
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label37:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	lw	$a0 16($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label38
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label38:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label39
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label39:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	lw	$a0 20($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label40
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label40:
	lw	$t1 8($a0)
	lw	$t2 12($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label41
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label41:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	lw	$a0 20($s0)
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label42
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label42:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label43
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label43:
	lw	$t1 8($a0)
	lw	$t2 16($t1)
	jalr		$t2
	la	$a0 str_const1
	sw	$a0 0($sp)
	addiu	$sp $sp -4
# outputing self
	move	$a0 $s0
#code reached 02
	li	$t1 0
	bne	$a0 $t1 label44
	la	$a0 str_const0
	li	$t1 1
	jal	_dispatch_abort
label44:
	lw	$t1 8($a0)
	lw	$t2 12($t1)
	jalr		$t2
	lw	$s0 -12($fp)
	lw	$ra -8($fp)
	lw	$fp -4($fp)
	addiu	$sp $sp 12
	jr	$ra	
D.f:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	addiu	$fp $sp 8
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$s0 $a0
	la	$a0 int_const4
	lw	$s0 -12($fp)
	lw	$ra -8($fp)
	lw	$fp -4($fp)
	addiu	$sp $sp 12
	jr	$ra	
D.g:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	addiu	$fp $sp 8
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$s0 $a0
	la	$a0 int_const5
	lw	$s0 -12($fp)
	lw	$ra -8($fp)
	lw	$fp -4($fp)
	addiu	$sp $sp 12
	jr	$ra	
C.f:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	addiu	$fp $sp 8
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$s0 $a0
	la	$a0 int_const3
	lw	$s0 -12($fp)
	lw	$ra -8($fp)
	lw	$fp -4($fp)
	addiu	$sp $sp 12
	jr	$ra	
B.g:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	addiu	$fp $sp 8
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$s0 $a0
	la	$a0 int_const2
	lw	$s0 -12($fp)
	lw	$ra -8($fp)
	lw	$fp -4($fp)
	addiu	$sp $sp 12
	jr	$ra	
A.f:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	addiu	$fp $sp 8
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$s0 $a0
	la	$a0 int_const0
	lw	$s0 -12($fp)
	lw	$ra -8($fp)
	lw	$fp -4($fp)
	addiu	$sp $sp 12
	jr	$ra	
A.g:
	sw	$fp 0($sp)
	addiu	$sp $sp -4
	addiu	$fp $sp 8
	sw	$ra 0($sp)
	addiu	$sp $sp -4
	sw	$s0 0($sp)
	addiu	$sp $sp -4
	move	$s0 $a0
	la	$a0 int_const1
	lw	$s0 -12($fp)
	lw	$ra -8($fp)
	lw	$fp -4($fp)
	addiu	$sp $sp 12
	jr	$ra	
class_objTab:
	.word	String_protObj
	.word	String_init
	.word	Int_protObj
	.word	Int_init
	.word	Bool_protObj
	.word	Bool_init
	.word	Main_protObj
	.word	Main_init
	.word	D_protObj
	.word	D_init
	.word	C_protObj
	.word	C_init
	.word	B_protObj
	.word	B_init
	.word	A_protObj
	.word	A_init
	.word	IO_protObj
	.word	IO_init
	.word	Object_protObj
	.word	Object_init

# end of generated code

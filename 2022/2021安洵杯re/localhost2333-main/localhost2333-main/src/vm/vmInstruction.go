package vm

import "html/template"

// push
func (reg *register) sub_0 () {
	// 一个参数，堆栈指针
	_regPtr := reg.selectReg(opcode[reg.eip+1])
	encInputStack = append(encInputStack, *_regPtr)
	reg.eip += 2
}

// pop
func (reg *register) sub_1 (){
	// 一个参数，寄存器指针
	_regPtr := reg.selectReg(opcode[reg.eip+1])
	*_regPtr = dataStack[len(dataStack) - 1]
	dataStack = dataStack[:len(dataStack) - 1]
	reg.eip += 2
}

// xor
func (reg *register) sub_2 (){
	_1regPtr := reg.selectReg(opcode[reg.eip+1])
	_2regPtr := reg.selectReg(opcode[reg.eip+2])
	*_1regPtr ^=*_2regPtr
	reg.eip += 3
}

// sub
func (reg *register) sub_3 (){
	// 两个参数，两个寄存器标志

	_1regPtr := reg.selectReg(opcode[reg.eip+1])
	_2regPtr := reg.selectReg(opcode[reg.eip+2])
	*_1regPtr -= *_2regPtr
	reg.eip += 3
}

// add
func (reg *register) sub_4() {
	_1regPtr := reg.selectReg(opcode[reg.eip+1])
	_2regPtr := reg.selectReg(opcode[reg.eip+2])
	*_1regPtr += *_2regPtr
	reg.eip += 3
}





// or
func (reg *register) sub_5 () {
	_1regPtr := reg.selectReg(opcode[reg.eip+1])
	_2regPtr := reg.selectReg(opcode[reg.eip+2])
	*_1regPtr |= *_2regPtr
	reg.eip += 3
}

// shl
func (reg *register) sub_6() {
	_1regPtr := reg.selectReg(opcode[reg.eip+1])
	*_1regPtr <<= opcode[reg.eip+2]
	reg.eip += 3
}

// shr
func (reg *register) sub_7() {
	_1regPtr := reg.selectReg(opcode[reg.eip+1])
	*_1regPtr >>= opcode[reg.eip+2]
	reg.eip += 3
}

// mov
func (reg *register) sub_8() {
	_1regPtr := reg.selectReg(opcode[reg.eip+1])
	_2regPtr := reg.selectReg(opcode[reg.eip+2])
	*_1regPtr = *_2regPtr
	reg.eip += 3
}




// cmp
func (reg *register) sub_9 (){
	reg.eax = 1
	reg.eip += 1
	for i, val := range encInputStack {
		if val != cipher[i] {
			reg.eax = 0
			return
		}
	}
}
// ret
func (reg *register) sub_10() (str template.HTML){
	if reg.eax == 1 {
		return "good job!!"
	} else {
		return "oh no..."
	}
}
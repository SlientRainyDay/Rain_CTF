package vm

type register struct {
	eax uint8
	ebx uint8
	ecx uint8
	edx uint8
	eip uint32
}

func (reg *register)selectReg(optionReg uint8) (regPtr *uint8){
	var retVal *uint8
	switch optionReg {
	case 1:
		retVal = &((*reg).eax)
	case 2:
		retVal = &((*reg).ebx)
	case 3:
		retVal = &((*reg).ecx)
	case 4:
		retVal = &((*reg).edx)
	}
	return retVal
}
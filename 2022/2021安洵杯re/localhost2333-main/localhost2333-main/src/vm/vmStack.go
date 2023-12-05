package vm

func rev(stack []byte) {
	for i, j := 0, len(stack)-1; i < j; i, j = i+1, j-1 {
		stack[i], stack[j] = stack[j], stack[i]
	}
}

func makeStack(flag string) ([]byte){
	var stack []byte
	byteFlag := []byte(flag)
	group1 := byteFlag[:10]
	group2 := byteFlag[10:20]
	group3 := byteFlag[20:]

	// 第一组前后连续异或
	for i, val := range group1 {
		stack = append(stack, val)
		stack = append(stack, byte(i))
	}

	// 第二组逐位异或
	for i, val := range group2 {
		stack = append(stack, val)
		stack = append(stack, key[i])
	}

	// 第三组做位运算
	for _, val := range group3 {
		stack = append(stack, val)
	}
	rev(stack)
	return stack
}
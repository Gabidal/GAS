use "../../IO/cstd.e"

export int Start_Test(){
	int ptr a = Allocate<int ptr>(10)
	int ptr ptr b = a
	b[0, 1] = 1
	int c = b[0, 1]
	return c
}

export int main(){
	return 1
}
def calc_char(char, key):
	
	int_val = ord(char) - ord('a')
	int_val += key
	if int_val > 25:
		int_val -= 26
		
	return chr(ord('a') + int_val)
	

def decrypt(ciphertext):

	res = []
	for k in range(26):
		
		plain = ''
		for char in ciphertext:
			plain += calc_char(char, k)
			
		res.append(plain)
		
	return res


if __name__ == '__main__':
	
	ciphertext = 'xultpaajcxitltlxaarpjhtiwtgxktghidhipxciwtvgtpilpitghlxiwiwtxgqadds'
	
	plain = decrypt(ciphertext)
	
	for p in plain:
		print(p)
	
	#solution: ifweallunitewewillcausetheriverstostainthegreatwaterswiththeirblood
all:	xor 	rc4	vigenere 

xor:
	@echo "XOR: Compilation"
	@g++ -shared -fPIC xorCryptoModule.cpp -o xor.cryptomodule -std=c++17
	@echo "XOR: SEC"

rc4:
	@echo "RC4: Compilation"
	@g++ -shared -fPIC rc4CryptoModule.cpp -o rc4.cryptomodule -std=c++17
	@echo "RC4: SEC"

vigenere:	
	@echo "VIGENERE: Compilation"
	@g++ -shared -fPIC vigenereCryptoModule.cpp -o vigenere.cryptomodule -std=c++17
	@echo "VIGENERE: SEC"






clear:
	@rm -rf *.cryptomodule

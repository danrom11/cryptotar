all:	xor 

xor:
	@echo "XOR: Compilation"
	@g++ -shared -fPIC xorCryptoModule.cpp -o xor.cryptomodule -std=c++17
	@echo "XOR: SEC"

rc4:
	@echo "RC4: Compilation"
	@g++ -shared -fPIC rc4CryptoModule.cpp -o rc4.cryptomodule -std=c++17
	@echo "RC4: SEC"




clear:
	@rm -rf *.cryptomodule

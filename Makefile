all:	xor

xor:
	@echo "XOR: Compilation"
	@g++ -shared -fPIC xorCryptoModule.cpp -o xor.cryptomodule -std=c++17
	@echo "XOR: SEC"

clear:
	@rm -rf *.cryptomodule

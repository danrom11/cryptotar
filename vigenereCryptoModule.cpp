#include <iostream>
#include <string>
#include "vigenereCryptoModule.hpp"

using namespace std;

class crypto : public cryptoModule{


	string alfavit = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
private:
	int keya(char s)
	{
		for (int i = 0; i < alfavit.length(); i++){
			if(s == alfavit[i]) return i;
		}

		return 0;
	}
public:
	void* cryptoData(unsigned char* data, size_t size) override 
	{
		//string a;
		for(size_t i = 0; i < size; i++)
		{
			data += alfavit[(keya(data[i]) + keya(key[i % sizeKey])) % alfavit.length()];
		}
	
		return data;
	}

	void* uncryptoData(unsigned char* data, size_t size) override
	{
		//string a;
		for (size_t i = 0; i < size; i++)
		{
			data += alfavit[(keya(data[i]) - keya(key[i % sizeKey]) + alfavit.length()) % alfavit.length()];
		}

		return data;
	}
};

cryptoModule* makechild(){
	return new crypto();
}
//int main(){
//	string txt, key;
//	char x;
//	cout << "enter number encoding(1) or decoding(2)" << endl;
//	cin >> x;

//	cout << "enter text: ";
//	cin >> txt;
//	cout << "enter key: ";
//	cin >> key;


//	if(x == '1') cout << "encrypted: " << Encode(txt, key) << endl;
//	if(x == '2') cout << "decrypted: " << Decode(txt, key) << endl;
//}


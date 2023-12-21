#include <iostream>
#include <string>
#include <fstream>
#include <ncurses.h>
#include <getopt.h>
#include <vector>

#include "cryptotarlib.hpp"

int countPercentage = 10, blocks = 0;

void customProgress(size_t bytesRead, size_t fileSize, char* fileName){ 

	double Read = (double) bytesRead, Size = (double) fileSize; int finishingPercentage = Read/Size*100;

	//const int Percentage[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100}; 

	if ( bytesRead <= 4096 ) std::cout << "File: " << fileName << "\t[";

	if (finishingPercentage == countPercentage ){ std::cout << "=="; countPercentage += 10; if ( countPercentage == 100 ) countPercentage = 10;} /*clear(); printw("%d", Percentage[i]); refresh();*/
	
	blocks++;

	if ( bytesRead == fileSize ) { std::cout << "] " << finishingPercentage << "%\nINFO:\nWritenB: " << bytesRead << "\tblocks: " << blocks << std::endl; blocks = 0; }	

}
/*
char readMethod(){

	std::cout << "Enter number: ";
	char method;
	std::cin >> method;
	if (method == '1' || method == '2') { if (method == '1') std::cout << "You choosed: 1.rc4" << std::endl; else std::cout << "You choosed: 2.xor" << std::endl; }
       	else { std::cout << "You must enter only method number!!!\n" << std::endl; readMethod(); }
	return method;
}
*/
std::string readKey(){

	std::cout << "\nEnter key: ";
	std::string key;
	std::cin >> key;
	std::cout << "You entered: " << key << std::endl << std::endl;
	return key;
}

int main(int argc, char *argv[]){

	bool has_h = false, has_u = false, has_d = false, has_c = false, has_m = false, has_n = false, has_f = false, has_o = false;

	std::vector <std::string> paths;
	std::string output_file_name, ctar = ".ctar", methodPath, key;  
	
	char*  unpack_ctar;
	
	int r, option_index = 0;

	const char *short_options = "hcu:ndm:f:o:";

	const struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"compress", no_argument, NULL, 'c'},
		{"unpack", required_argument, NULL, 'u'},
		{"encrypt", no_argument, NULL, 'n'},
		{"decrypt", no_argument, NULL, 'd'},
		{"method", required_argument, NULL, 'm'},
		{"files", required_argument, NULL, 'f'},
		{"output", required_argument, NULL, 'o'},
		{NULL, 0, NULL, 0}
	};

	while((r = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1){
		
		switch(r) {

			case 'h':
				has_h = true;
				break;

			case 'u':
				has_u = true;
				if (optarg != NULL){
					std::ifstream file_unpack (argv[optind-1]);
					if (!file_unpack) { std::cout << "file is not exists" << std::endl; return -1; }
					else unpack_ctar = argv[optind-1];
				}
				break;

			case 'd':
				has_d = true;
				break;

			case 'c':
				has_c = true;
				break;

			case 'n':
				has_n = true;
				break;

			case 'm':
				has_m = true;
				if (optarg != NULL){
					std::ifstream file_method (argv[optind-1]);
					if (!file_method) { std::cout << "file is not exists" << std::endl; return -1; }
					else methodPath = argv[optind-1];
				}
				break;

			case 'f':
				has_f = true;
				if (optarg != NULL){
					for (int i = optind - 1; i < argc && *argv[i] != '-'; i++) { std::ifstream file (argv[i]);
						if (!file)  { std::cout << "file is not exists" << std::endl; return -1; }
						else paths.push_back(argv[i]); 
					}
				}
				break;

			case 'o':
				has_o = true;
				if (optarg != NULL){
					output_file_name = argv[optind-1];	
				}
				break;
	 	}
	}




	//logic
		
	if ( (has_c == false && has_u == false) || has_h == true ) { 

		has_u = false; has_d = false; has_c = false; has_n == false; has_f = false; has_o = false;

		std::cout << "\nIf you want compress your file(s):" << std::endl;
		std::cout << "You can use and should flags: -c (--compress) use for compress file(s)  -f (--files) and file(s) name(s)  -o (--output) and output file\n" << std::endl;
		std::cout << "OR ( if you want encrypt your archive ):" << std::endl;
		std::cout << "You can use and should flags: -c (--compress)  -n (--encrypt) for encrypt your archive  -m (--method) and path to encrypt method  -f (--files)  -o (--output)\n" <<std::endl;
		std::cout << "If you want unpack your ctar archive:" << std::endl;
		std::cout << "You can use and should flags: -u (--unpack) with argumet your ctar archive, use for unpack your ctar archive  -o (--output) and output directory for unpack, else your archive unpack in current directory\n" << std::endl; 
		std::cout << "OR ( if your archive encrypt ):" << std::endl;
		std::cout << "You can use and should flags: -u (--unpack)  -d (--decrypt) for decrypt your encrypt archive  -m (--method) and path to encrypt method -o (--output) else you tar archive unpack in current directory\n" << std::endl;

	} else if ( has_c == true ) {

		if ( has_d == true ) { std::cout << "You cant use flag -d (--decrypt) with flag -c (--compress)" << std::endl; return -1;

		} else if ( has_f == false && has_n == false ) { std::cout << "You can use flag -n (--encrypt) for encrypt your archive or you can skip flag -n (--encrypt) and use flag -f (--flag) with argument(s) path(s) to your file(s)" << std::endl; return -1;
		
		} else if ( has_f == false ) { std::cout << "You should use flag -f (--files) and name(s) file(s)" << std::endl; return -1;
		
		} else if ( has_o == false ) { std::cout << "You should use flag -o (--output) and name output file" << std::endl; return -1;
		
		} else if ( has_n == false )  std::cout << "You can use -e (--encrypt) for encrypt your ctar archive" << std::endl;

		else if ( has_n == true ) { if ( has_m == true ) key = readKey(); else { std::cout << "You cant use flag -n (--encrypt) without flag -m (--method) with argument path to encrypt method" << std::endl; return -1;} }

	} else if ( has_u == true ) {

		std::cout << "You must use -d (--decrypt), if your archive encrypt" << std::endl;

		if ( has_n == true ) { std::cout << "You cant use flag -n (--encrypt) with flag -u (--unpack)" << std::endl; return -1;
 
		} else if ( has_f == true ) { std::cout << "You cant use flag -f (--files) with flag -u (--unpack)" << std::endl; return -1;
		
		} else if ( has_d == true ) { if ( has_m == true ) key = readKey(); else { std::cout << "You cant use flag -d (--decrypt) without flag -m (--method) with argument path to encrypt method" << std::endl; return -1;}
			
		} else if ( has_o == false )  std::cout << "You can use -o (--output) for manually entering the directory for unpacking" << std::endl; 	

	} else { std::cout << has_c << "|" <<  has_u << std::endl; std::cout << "You cant use flags -c (--compress) and -u (--unpack) together" << std::endl; return -1;}


	//pack
	
	if ( has_c == true && has_f == true && has_o == true ) { 

		//initscr();
		
		cryptotar newTar(output_file_name + ctar);

	       	newTar.globalProgressCallback = customProgress; 

		if ( has_n == true && has_m == true )  newTar.setCryptoModule(methodPath, key, key.size()); 

		while ( paths.empty() != true ) { newTar.addPath(paths.back()); paths.pop_back(); }

		newTar.closeTar();

		//getch();

		//endwin();

		std::cout << "--------------------------------------------------\n" << "CTAR archive is complited!" << std::endl;
	}		
	

	//unpack

	if ( has_u == true ) {

		initscr();

		cryptotar tarEx;

		tarEx.globalProgressCallback = customProgress;

		if ( has_d == true && has_m == true ) tarEx.setCryptoModule(methodPath, key, key.size());

		if ( has_o == true ) {
			
			output_file_name.push_back('/');
			tarEx.unpackTar(unpack_ctar, output_file_name);
		}

		else tarEx.unpackTar(unpack_ctar, ".");

		getch();

		endwin();

		std::cout << "--------------------------------------------------\n" <<"Unpacking is complete!" << std::endl;

	}


	return 0;
}

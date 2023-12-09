#include <iostream>
#include <fstream>
#include <getopt.h>
#include <vector>

#include "cryptotarlib.hpp"

void customProgress(size_t bytesRead, size_t fileSize) {

	std::cout << "Bytes read: " << bytesRead << std::endl;
}

int  main(int argc, char *argv[]){

	bool has_h = false, has_u = false, has_c = false, has_f = false, has_o = false;

	std::vector <std::string> paths;
	char* output_file_name; char*  unpack_ctar;

	int r, option_index = 0;


	const char *short_options = "hu:cf:o:";

	const struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"unpack", required_argument, NULL, 'u'},
		{"compress", no_argument, NULL, 'c'},
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
					unpack_ctar = argv[optind-1];
				}
				break;


			case 'c':
				has_c = true;
				break;

			case 'f':
				has_f = true;
				if (optarg != NULL){
					for (int i = optind - 1; i < argc && *argv[i] != '-'; i++) { std::ifstream file (argv[i]);
						if (!file)  { std::cout << "file is not exists" << std::endl; return 1; }
						else paths.push_back(argv[i]); }
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


	//errors
	


	if ( (has_c == false && has_u == false) || has_h == true ) { 

		has_c == false; has_u == false; has_f == false; has_o == false;

		std::cout << "You can use and should flags: -c (--compress) use for compress file(s)  -f (--files) and file(s) name(s)  -o (--output) and output file" << std::endl;
		std::cout << "You can use and should flags: -u (--unpack) use fo unpack file  -f (--files) and name only one file  -o (--output) and output directory for unpacking" << std::endl; 
	
	} else if ( has_c == true ) {

		if ( has_f == false ) { std::cout << "You should use flag -f (--files) and name(s) file(s)" << std::endl; return -1;}

		else if ( has_o == false ) { std::cout << "You should use flag -o (--output) and name output file" << std::endl; return -1;}

	} else if ( has_u == true ) {

		if ( has_f == true ) { std::cout << "You cant use flag -f (--files) with flag -u (--compress)" << std::endl; return -1;}

		else if ( has_o == false )  std::cout << "You can use -o (--output) for manually entering the directory for unpacking" << std::endl; 

	} else { std::cout << has_c << "|" <<  has_u << std::endl; std::cout << "You cant use flags -c (--compress) and -u (--unpack) together" << std::endl; return -1;}


	//ctar_creating_with_vector
	
	if ( has_c == true && has_f == true && has_o == true ) { cryptotar cr(output_file_name, paths); cr.closeTar();}
	


	//unpack
	
	if ( has_u == true ) {

		cryptotar tarEx;
		
		tarEx.globalProgressCallback = customProgress;

		if (has_o == true ) tarEx.unpackTar(unpack_ctar, output_file_name);
		else tarEx.unpackTar(unpack_ctar, ".");

		tarEx.closeTar();
	}

	return 0;
}

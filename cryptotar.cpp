#include <iostream>
#include <getopt.h>
#include <vector>

#include "cryptotarlib.hpp"

int  main(int argc, char *argv[]){

	std::cout << "popa" << std::endl;
	bool has_h, has_u, has_c, has_f, has_o = false;
	std::vector <std::string> paths;
	char* output_file_name; char*  unpack_ctar;

	int r, option_index = 0;


	const char *short_options = "hucf:o:";

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
				std::cout << "Help: -u (--unpack)\n-c (--compress) \n-f (--files) \n-o (--output)" << std::endl;
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
					for (int i = optind - 1; i < argc && *argv[i] != '-'; i++) paths.push_back(argv[i]);
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
	
	if (has_h == true) {has_c == false; has_u == false; has_f == false; has_o == false;}
	
	if (has_u == true || (has_c == true && has_f == true && has_o)) {std::cout << "You cannot use -u (--unpack) with other flags" << std::endl; return -1;}
	
	if (has_h == false || has_u == false){
		if (has_c == false) { std::cout << "Not use flag -c (--copmress)" << std::endl; return -1;}
		else if (has_f == false) {std::cout << "Not use flag -f (--files)" << std::endl; return -1;}
		else if (has_o == false) {std::cout << "Not use flag -o (--output)" << std::endl; return -1;}
	}

	//ctar_creating_with_vector
	
	if ( has_c = true ) {cryptotar cr(output_file_name, paths); cr.closeTar();}
	

	//unpack
	
	if ( has_u = true ) {cryptotar tarEx(unpack_ctar, ".");}
	

	return 0;
}

#include <iostream>
#include <getopt.h>
#include <vector>

int  main(int argc, char *argv[]){

	bool has_c, has_f, has_o = false;
	std::vector <char*> files = {};
	char* output_file_name = NULL;

	int r, option_index = 0;


	const char *short_options = "cf:o:";

	const struct option long_options[] = {
		{"compress", no_argument, NULL, 'c'},
		{"files", required_argument, NULL, 'f'},
		{"output", required_argument, NULL, 'o'},
		{NULL, 0, NULL, 0}
	};

	while((r = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1){
		
		switch(r) {

			case 'c':
				has_c = true;
				std::cout << "File be compress\n" << std::endl;
				break;

			case 'f':
				has_f = true;
				if (optarg != NULL){
					for (int i = optind - 1; i < argc && *argv[i] != '-'; i++) files.push_back(argv[i]);
				}

				break;

			case 'o':
				has_o = true;
				if (optarg != NULL){
					output_file_name = argv[optind-1];	
				}
	 	}
	}

	//errors
	if (has_c == false) { std::cout << "Not use flag -c (--copmress)" << std::endl; return -1;}
	else if (has_f == false) {std::cout << "Not use flag -f (--files)" << std::endl; return -1;}
	else if (has_o == false) {std::cout << "Not use flag -o (--output)" << std::endl; return -1;}

	//output
	for ( int i = 0; i < files.size(); i++) std::cout << files[i] << std::endl;
	std::cout << "\n" << output_file_name << std::endl;


	return 0;
}

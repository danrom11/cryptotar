#include <iostream>
#include <getopt.h>



int main(int argc, char** argv){



    int r = 0;
    int option_index = 0;

    const char *short_options = "f:o:";
    
    const struct option long_options[] = {
        {"files", required_argument, NULL, 'f'},
        {"output", required_argument, NULL, 'o'},
        {NULL, 0, NULL, 0}
    };

    while((r = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1){
        switch (r) {
            case 'f':
                if(optarg != NULL){
                    for(int i = optind - 1; i < argc && *argv[i] != '-'; i++){
                        std::cout << argv[i] << std::endl;
                    }
                }
                break;
            case 'o':
                if(optarg != NULL){
                    // std::cout << argv[optind - 1] << std::endl;
                    std::cout << optarg << std::endl;
                }
                break;
        }
    }

    return 0;
}

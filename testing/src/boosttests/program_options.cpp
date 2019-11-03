#include "boost/program_options.hpp"
#include <iostream>
#include <fstream>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    // FIRST EXAMPLE

    // po::options_description desc("Allowed options");
    // desc.add_options()
    // ("help", "produce help message")
    // ("compression", po::value<int>(), "set compression level")
    // ("status", po::value<char>(), "set status")
    // ;

    // po::variables_map vm;
    // po::store(po::parse_command_line(argc, argv, desc), vm);
    // po::notify(vm);    

    // if (vm.empty()) { std::cout << "No arguments.\nTry --help for usage.\n"; return EXIT_FAILURE; }

    // if (vm.count("help")) {
    //     std::cout << desc << "\n";
    //     return 1;
    // }

    // if (vm.count("compression")) {
    //     std::cout << "Compression level was set to " 
    // << vm["compression"].as<int>() << ".\n";
    // }
    // if (vm.count("status")) {
    //     std::cout << "Status set to " << vm["compression"].as<char>() << "\n";
    // }


    // SECOND EXAMPLE -- OPTIONS DETAILS
    // int opt;
    // po::options_description desc("Allowed options");
    // desc.add_options()
    // ("help", "produce help message")
    // ("optimization", po::value<int>(&opt)->default_value(10), // specifies address of variable to store the option, and a default value in case none is specified
    // "optimization level")
    // ("include-path,I", po::value< std::vector<std::string> >(), 
    // "include path")
    // ("input-file", po::value< std::vector<std::string> >(), "input file")
    // ;

    // po::positional_options_description posi;
    // posi.add("input-file", -1);

    // po::variables_map args;
    // po::store(po::command_line_parser(argc, argv).options(desc).positional(posi).run(), args);
    // po::notify(args);

    // if (args.empty()) std::cout << desc << "\n";

    // if (args.count("include-path")) {
    //     std::cout << "Include paths are:\n";
    //     for (auto& ips : args["include-path"].as<std::vector<std::string>>())
    //         std::cout << "- " << ips << "\n";
    // }
    // if (args.count("input-file")) {
    //     std::cout << "Input files are:\n";
    //     for (auto& ips : args["input-file"].as<std::vector<std::string>>())
    //         std::cout << "- " << ips << "\n";
    // }

    // THIRD EXAMPLE -- MULTIPLE SOURCES (stdin + config file)
    int opt;
    std::string fname;
    std::ifstream ifile;
    po::options_description generic("Generic Options");
    generic.add_options()
    ("help", "display help message")
    ("version,v", "version info")
    ;

    po::options_description file("Configuration");
    file.add_options()
    ("optimization,O", po::value<int>(&opt)->default_value(10), "set optimization level")
    ("include-path,I", po::value<std::vector<std::string>>()->composing(), "include paths")
    ;

    po::options_description hidden("hidden opts");
    hidden.add_options()
    ("input-file", po::value<std::vector<std::string>>(), "input files")
    ;

    po::options_description configfile;
    configfile.add_options()
    ("config-file", po::value<std::string>(&fname), "configuration file")
    ;

    // setting both config file and input-files as positional args
    po::positional_options_description posit_args;
    posit_args.add("config-file", 1);
    posit_args.add("input-file", 4); // second argument means the acceptable number of arguments for this option
    // a -1 means "as many as user wants".
    // the .add follows a queue-like order.

    // setting up descs
    po::options_description genuse("Usage:\n" + std::string(argv[0]) + " [options] config-file input-files\n\nOptions");
    genuse.add(generic).add(file);
    po::options_description cfile;
    cfile.add(genuse).add(configfile).add(hidden);

    po::variables_map args;
    po::store(po::command_line_parser(argc, argv).options(cfile).positional(posit_args).run(), args);
    po::notify(args);

    if (args.count("help")) { std::cout << genuse << "\n"; exit(1); }
    if (args.count("version")) { std::cout << "Learning Boost.Program_Options v1.0\n"; exit(1); }
    if (!args.count("config-file")) { std::cout << "Config file must be provided.\n"; exit(-1); }
    else {
        ifile.open(fname);
        if (!ifile.is_open()) { std::cout << "Couldn't open config file named " << fname << "\n"; exit(-1); }
        po::store(po::parse_config_file(ifile, cfile), args);
    }
    if (!args.count("input-file")) { std::cout << "Missing input files.\n"; exit(-1); }
    else {
        std::cout << "Input files are:\n";
        for (auto& e : args["input-file"].as<std::vector<std::string>>())
            std::cout << "- " << e << "\n";
    }
    if (args.count("include-path")) {
        std::cout << "Include paths are:\n";
        for (auto& e : args["include-path"].as<std::vector<std::string>>())
            std::cout << "- " << e << "\n";
    }    
    std::cout << "Optimization level is set to " << args["optimization"].as<int>() << "\n";

    

}
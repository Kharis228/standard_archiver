#include <iostream>
#include <vector>
#include <map>
#include "Pack.h"
#include "Unpack.h"
#include "Parser.h"
#include "Exceptions.h"

void Pack(const std::string& archive_name, const std::vector<std::string>& files) {
    Packer packer(archive_name, files);
    packer.Pack();
}

void Unpack(const std::string& archive_name) {
    Unpacker unpacker(archive_name);
    unpacker.Unpack();
}

void Help() {
    const std::string info = "archiver -c archive_name file1 [file2 ...]\narchiver -d archive_name\narchiver -h";
    std::cout << info << '\n';
}

void PackArgs(const std::vector<std::string>& args) {
    if (args.size() <= 1) {
        throw LackArgumentsException();
    }
    std::string archive_name = args[0];
    std::vector<std::string> tmp(args.begin() + 1, args.end());
    Pack(archive_name, tmp);
}

void UnpackArgs(const std::vector<std::string>& args) {
    if (args.empty()) {
        throw LackArgumentsException();
    }
    Unpack(args[0]);
}

void HelpArgs(const std::vector<std::string>& args) {
    Help();
}

extern const std::map<std::string, Oper*> PARAMETERS_GOTO = {{"-c", PackArgs}, {"-d", UnpackArgs}, {"-h", HelpArgs}};

void CatchlessArchiver(int argc, char** argv) {
    Parser parser(argc, argv);
    auto [func, args] = parser.GetParsed();
    func(args);
}

int main(int argc, char** argv) {
    try {
        CatchlessArchiver(argc, argv);
    } catch (const ParserException& e) {
        std::cerr << e.what() << '\n';
    } catch (...) {
    }

    return 0;
}
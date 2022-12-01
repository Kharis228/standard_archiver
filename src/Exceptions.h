#pragma once
#include <exception>

class ParserException : public std::exception {};

class EmptyException : ParserException {
public:
    const char* what() const throw() override {
        return "What you want to parse?\ntry 'archiver -h' for more information";
    }
};
class OptionException : ParserException {
public:
    const char* what() const throw() override {
        return "bad options\ntry 'archiver -h' for more information";
    }
};
class LackArgumentsException : ParserException {
public:
    const char* what() const throw() override {
        return "lack of arguments\ntry 'archiver -h' for more information";
    }
};
class BadArgumentsException : ParserException {
public:
    const char* what() const throw() override {
        return "incorrect arguments\ntry 'archiver -h' for more information";
    }
};
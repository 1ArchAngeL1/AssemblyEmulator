#ifndef _WELCOME_H
#define _WELCOME_H

#include<string>
#include<iostream>

using namespace std;


void init_funcs();

void welcome(string &filename);

void start();

void startup(string &file_name);

void upload_code(ifstream &input);

string upper_code(string code);

void validate_lines();


#endif
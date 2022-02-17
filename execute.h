#ifndef _EXECUTE_H
#define _EXECUTE_H

#include<unordered_set>
#include<iostream>
#include <unordered_map>
#include <vector>
#include<fstream>


using namespace std;

void program_cursor(string instr, long &index);

bool is_load(string instr);

bool is_store(string instr);

bool is_alu(string instr);

bool is_JMP(string instr);

bool is_branch(string instr);

bool is_call(string instr);

void exec();

void load(string instr);

void alu(string instr);

void store(string instr);

void branch(string instr, long &index);

void jmp(string instr, long &index);

void call(string function_name);


#endif
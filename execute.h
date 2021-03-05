#ifndef _EXECUTE_H
#define _EXECUTE_H

#include<unordered_set>
#include<bits/stdc++.h>
#include<iostream>
#include <unordered_map>
#include <vector>


using namespace std;

void programCursor(string instr,int & index);

bool is_Load(string instr);
bool is_Store(string instr);
bool is_ALU(string instr);
bool is_JMP(string instr);
bool is_BRANCH(string instr);
bool is_CALL(string instr);

void EXEC();

void LOAD(string instr);

void ALU(string instr);

void STORE(string instr);

void BRANCH(string instr,int & index);

void JMP(string instr,int & index);

void CALL(string functuon_name);





#endif
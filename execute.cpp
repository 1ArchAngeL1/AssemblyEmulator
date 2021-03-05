#include "execute.h"

extern unordered_map<string,int> registers;
int memory[200000];
extern unordered_map  <string,vector<string> > functions;
int *sp = memory + 199999;
extern vector<string> assembler_code;
unordered_set<string> BRANCH_EXMP = {"BLT", "BLE", "BEQ", "BNE", "BGT", "BGE"};
unordered_set<char> operators = {'+','-','/','*'};


using namespace std;


int search_out(string val){
    if(registers.find(val) != registers.end()){
        return registers[val];
    }
    return stoi(val);
}


void LOAD(string instr){
    int * temp_ptr;
    bool complex = false;
    char oper;
    int type = 0;
    stringstream str(instr);
    string temp;
    vector<string> parts; // there is register name on zero index and adress on 1
    while(getline(str,temp,'=')){
        parts.push_back(temp);
    }
    string mem = parts[1];
    string where_to_load = parts[0];
    if(mem[0] == '.'){
         type = mem[1]-'0';
         mem = mem.substr(3);
    }else{
        mem = mem.substr(1);
    }
    for(int i = mem.length()-1;i >= 0;--i){
        if(mem[i] == ']' || mem[i] == '[')mem.erase(i,1);
    }
    for(int i = 0;i < mem.length();++i){
        if(operators.find(mem[i]) != operators.end()){
            complex = true;
            oper = mem[i];
        }
    }
    if(complex){
        int num;
        stringstream dividor(mem);
        string adress;
        string number;
        string empt;
        while(getline(dividor,empt,oper)){
            if(registers.find(empt) != registers.end()){
                adress = empt;
            }else{
                number = empt;
            }
        }
        num = stoi(number);
        temp_ptr = (int*)(registers[adress]);
        if(oper == '-'){
            temp_ptr = (int*)((char*)temp_ptr - num);
        }else if(oper == '+'){
            temp_ptr =(int*)((char*)temp_ptr + num);
        }
    }else{
        string adress = mem;
        temp_ptr = (int*)(registers[mem]);
    }
    switch(type){
            case 0:
                registers[where_to_load] = *(temp_ptr);
                break;
            case 1:
                registers[where_to_load] = *((char*)temp_ptr);
                break;
            case 2:
                registers[where_to_load] = *((short*)temp_ptr);
                break;
            case 4:
                registers[where_to_load] = *(temp_ptr);
                break;
            case 8:
                registers[where_to_load] = *((long long *)temp_ptr);
                break;
    }   
}



void ALU(string instr){
    int type = 0;
    bool is_complex = false;
    char equa_operator;
    stringstream dividor(instr);
    string saver_reg;
    string equation;
    string saver;
    int counter = 0;
    while(getline(dividor,saver,'=')){
        if(!counter)saver_reg = saver;
        if(counter)equation = saver;
        counter++;
    }
    for(int i = 0; i < equation.length();++i){
        if(operators.find(equation[i]) != operators.end()){
            is_complex = true;
            equa_operator = equation[i];
        }
    }
    if(is_complex){
        vector<string> parts;
        stringstream div(equation);
        while(getline(div,saver,equa_operator)){
            parts.push_back(saver);
        }
        int val1 = search_out(parts[0]);
        int val2 = search_out(parts[1]);

        int final_val = 0;
        if(equa_operator == '+'){
            final_val = val1 + val2;
        }else if(equa_operator == '*'){
            final_val = val1 * val2;
        }else if(equa_operator == '-'){
            final_val = val1 - val2;
        }else if(equa_operator == '/'){
            final_val = val1 / val2;
        }
        registers[saver_reg] = final_val; 
    }else{
        if(equation[0] == '.'){
            type = equation[1]-'0';
            equation = equation.substr(2);
        }
        switch(type){
            case 0:
                registers[saver_reg] = search_out(equation);
                break;
            case 1:
                registers[saver_reg] = (char)search_out(equation);
                break;
            case 2:
                 registers[saver_reg] = (short)search_out(equation);
                 break;
            case 4:
                 registers[saver_reg] = search_out(equation);
                 break;
            case 8:
                 registers[saver_reg] = (long long)search_out(equation);
                 break;
        }        

    }
}

void STORE(string instr){
    char adress_operator;
    bool is_complex_adress = false;
    int * temp_ptr;
    int type = 0;
    stringstream dividor(instr);
    string temp;
    vector<string> parts; // on 0 index we save adress and on index 1 we save information to be uploaded
    while(getline(dividor,temp,'=')){
        parts.push_back(temp);
    }
    string adress = parts[0];
    string val = parts[1];
    adress = adress.substr(1);\
    for(int i = adress.length()-1;i >= 0;--i){
        if(adress[i] == ']' || adress[i] == '[')adress.erase(i,1);
    }
    for(int i = 0;i < adress.length();++i){
        if(operators.find(adress[i]) != operators.end()){
            is_complex_adress = true;
            adress_operator = adress[i];
        }
    }
    if(is_complex_adress){
        string saver;
        stringstream div(adress);
        string registr;
        string number;
        while(getline(div,saver,adress_operator)){
            if(registers.find(saver) != registers.end()){
                registr = saver;
            }else{
                number = saver;
            }
        }
        temp_ptr = (int*)registers[registr];
        int num = stoi(number);
        switch(adress_operator){
            case '+':
                temp_ptr = (int*)(((char*)temp_ptr) + num);
            case '-':  
                temp_ptr = (int*)(((char*)temp_ptr) - num);
        }
        }else{
            temp_ptr = (int*)registers[adress];
        }
        //ak iwkeba infos ganxilva
        if(val[0] == '.'){
            int type = val[1]-'0';
            val = val.substr(2);
        }
        int real_val;
        if(val[0] == 'R'){
            real_val = registers[val];
        }else{
            real_val = stoi(val); 
        }
        switch(type){
            case 0:
                *temp_ptr = real_val;
            case 1:
                *((char*)temp_ptr) = (char)real_val;
            case 2:
                *((short*)temp_ptr) = (short)real_val;
            case 4:
                *temp_ptr = real_val;
            case 8:
                *((long long*)temp_ptr) = (long long)real_val;
        }
}

//{"BLT", "BLE", "BEQ", "BNE", "BGT", "BGE"};
void BRANCH(string instr,int & index){
    string type = instr.substr(0,3);
    vector<string> parts;
    stringstream dividor(instr.substr(3));
    string saver;
    while(getline(dividor,saver,',')){
        parts.push_back(saver);
    }
    int val1 = search_out(parts[0]);
    int val2 = search_out(parts[1]);
    //BLE r1, 7,pc + 100
    string equation = parts[2];
    if(type == "BLT"){
        if(val1 < val2){JMP("JMP" + string() + equation,index);}
    }else if(type == "BLE"){
        if(val1 <= val2){JMP("JMP" + string() + equation,index);}
    }else if(type == "BEQ"){
        if(val1 == val2){JMP("JMP" + string() + equation,index);}
    }else if(type == "BNE"){
        if(val1 != val2){JMP("JMP" + string() + equation,index);}
    }else if(type == "BGT"){
        if(val1 > val2){JMP("JMP" + string() + equation,index);}
    }else if(type == "BGE"){
        if(val1 >= val2){JMP("JMP" + string() + equation,index);}
    }else{
        exit(1);
    }
}


void JMP(string instr,int & index){
    instr = instr.substr(3);
    char oper;
    for(int i = 0;i < instr.length();++i){
        if(operators.find(instr[i]) != operators.end()){
            oper = instr[i];
        }
    }
    stringstream dividor(instr);
    string num_saver;
    string number;
    while(getline(dividor,num_saver,oper)){
        number = num_saver;
    }
    int num = stoi(number);
    if(oper == '+'){
        index +=  num/4-1;
    }else if(oper == '-'){
        index -=  num/4+1;
    }
}


void CALL(string function_name){
    string namee = function_name.substr(4);
    for(int i = namee.length()-1;i >= 0;--i){
        if(namee[i] == '<' || namee[i] == '>')namee.erase(i,1);
    }
    registers["SP"] -=  4;
    vector<string> temp = functions[namee];
    for(int i = 0;i < temp.size();++i){
        cout << "Working on..: " <<  temp[i] << endl;
        if(temp[i] == "RET"){
             registers["SP"] += 4;
            return;
        }
        programCursor(temp[i],i);
    }
    registers["SP"] += 4;
}



void programCursor(string instr,int & index){
    if(sp <= memory + 100){
       for(int i = 0;i < 3;i++){
           cout <<""<< endl;
       }
       cout << "ooops... stackoverflow :)" << endl;
        exit(1);
    }
    if(is_Load(instr)){
        LOAD(instr);
    }else if(is_Store(instr)){
        STORE(instr);
    }else if(is_JMP(instr)){
        JMP(instr,index);
    }else if(is_BRANCH(instr)){
        BRANCH(instr,index);
    }else if(is_CALL(instr)){
        CALL(instr);
    }else if(is_ALU(instr)){
        ALU(instr);
    }else{
        exit(1);
    }
}


bool is_Load(string instr){
    for(int i = 0;i < instr.length();++i){
        if(instr[i] == '='){
           for(int j = i;j < instr.length();j++){
               if(instr[j] == 'M')return true;
           }
           return false;
        }
    }
    return false;
}




bool is_Store(string instr){
    if(instr[0] == 'M')return true;
    return false;
}

bool is_ALU(string instr){
    for(int i = 0;i < instr.length();++i){
        if(instr[i] == '='){
            for(int j = i;j < instr.length();j++){
                if(instr[j] == 'M')return false;
            }
            return true;
        }
    }
    return false;
}  



bool is_JMP(string instr){
    if(instr.substr(0,3) == "JMP")return true;
    return false;
}

bool is_BRANCH(string instr){
    if(BRANCH_EXMP.find(instr.substr(0,3)) != BRANCH_EXMP.end())return true;
    return false;
}

bool is_CALL(string instr){
    if(instr.substr(0,4) == "CALL")return true;
    return false;
}



void EXEC(){
    vector<string> code = functions["START"];
    for(int i = 0;i < code.size();++i){
        cout << "Working on....: " << code[i] << endl;
        if(code[i] == "RET")break;
        programCursor(code[i],i);
    }
    cout << "Final Result: " << registers["RV"] << endl;
}










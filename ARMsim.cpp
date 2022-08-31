/* On my honor, I have neither given nor received unauthorized aid on this assignment. */
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>

void ReadIn(const std::string &fileName, std::vector<std::string>& instructions){
    std::ifstream in;
    std::string inst;

    in.open(fileName);
    if(in.is_open()) {
        while(getline(in, inst)){//While still receiving input lines from file
            instructions.push_back(inst);
        }
    }
}

int NumDecode(std::string num){
    int val = 0;
    int multiple = 1;
    while (!num.empty()) {
        if (num.at(num.size() - 1) == '1') {
            val += multiple;
        }
        multiple *= 2;
        num.erase(num.size() - 1);
    }
    return val;
}

int NumDecodeTwos(std::string num){ //Registers and offsets only
    int val = 0;
    int multiple = 1;
    if(num.at(0) == '0') { //Positive number
        while (!num.empty()) {
            if (num.at(num.size() - 1) == '1') {
                val += multiple;
            }
            multiple *= 2;
            num.erase(num.size() - 1);
        }
    }
    else{ //Negative number
        while (!num.empty()) {
            if (num.at(num.size() - 1) == '0') {
                val -= multiple;
            }
            multiple *= 2;
            num.erase(num.size() - 1);
        }
        val -= 1;
    }
    return val;
}

std::vector<std::string> Type1(const std::string& code){
    std::vector<std::string> results;
    int op = NumDecode(code.substr(3, 5));
    int src1 = NumDecode(code.substr(8, 5));
    int offset = NumDecodeTwos(code.substr(13, 19));
    switch(op){
        case 16: //CBZ
            results.push_back("CBZ");
            break;
        case 17: //CBNZ
            results.push_back("CBNZ");
            break;
        default:
            break;
    }
    results.push_back(std::to_string(src1));
    results.push_back(std::to_string(offset));
    return results;
}

int Type1Sim(std::string& code, std::vector<int>& registers, int index){
    std::vector<std::string> info = Type1(code);
    if(info.at(0).size() == 3){ //CBZ
        if(registers.at(std::stoi(info.at(1))) == 0){
            index += std::stoi(info.at(2));
        }
        else{
            index += 1;
        }
    }
    else{ //CBNZ
        if(registers.at(std::stoi(info.at(1))) == 0){
            index += 1;
        }
        else{
            index += std::stoi(info.at(2));
        }
    }
    return index;
}

std::vector<std::string> Type2(const std::string& code){
    std::vector<std::string> results;
    int op = NumDecode(code.substr(3, 7));
    int dest = NumDecode(code.substr(10, 5));
    int src1 = NumDecode(code.substr(15, 5));
    int immediate = NumDecodeTwos(code.substr(20, 12));
    switch(op){
        case 64:
            results.push_back("ORRI");
            break;
        case 65:
            results.push_back("EORI");
            break;
        case 66:
            results.push_back("ADDI");
            break;
        case 67:
            results.push_back("SUBI");
            break;
        case 68:
            results.push_back("ANDI");
            break;
        default:
            break;
    }
    if(results.at(0) == "ADDI" || results.at(0) == "SUBI"){
        immediate = NumDecodeTwos(code.substr(20,12));
    }
    results.push_back(std::to_string(dest));
    results.push_back(std::to_string(src1));
    results.push_back(std::to_string(immediate));
    return results;
}

void Type2Sim(std::string& code, std::vector<int>& registers){
    std::vector<std::string> info = Type2(code);
    switch(info.at(0).at(0)){
        case 'O':
            registers.at(stoi(info.at(1))) = (registers.at(stoi(info.at(2))) | stoi(info.at(3)));
            break;
        case 'E':
            registers.at(stoi(info.at(1))) = (registers.at(stoi(info.at(2))) ^ stoi(info.at(3)));
            break;
        case 'S':
            registers.at(stoi(info.at(1))) = (registers.at(stoi(info.at(2))) - stoi(info.at(3)));
            break;
        case 'A':
            if(info.at(0).at(1) == 'D'){
                registers.at(stoi(info.at(1))) = (registers.at(stoi(info.at(2))) + stoi(info.at(3)));
            }
            else{
                registers.at(stoi(info.at(1))) = (registers.at(stoi(info.at(2))) & stoi(info.at(3)));
            }
            break;
        default:
            break;
    }
}

std::vector<std::string> Type3(const std::string& code){
    std::vector<std::string> results;
    int op = NumDecode(code.substr(3, 8));
    int dest = NumDecode(code.substr(11, 5));
    int src1 = NumDecode(code.substr(16, 5));
    int src2 = NumDecode(code.substr(21, 5));
    switch(op){
        case 160:
            results.push_back("EOR");
            break;
        case 162:
            results.push_back("ADD");
            break;
        case 163:
            results.push_back("SUB");
            break;
        case 164:
            results.push_back("AND");
            break;
        case 165:
            results.push_back("ORR");
            break;
        case 166:
            results.push_back("LSR");
            break;
        case 167:
            results.push_back("LSL");
            break;
        default:
            break;
    }
    results.push_back(std::to_string(dest));
    results.push_back(std::to_string(src1));
    results.push_back(std::to_string(src2));
    return results;
}

void Type3Sim(std::string& code, std::vector<int>& registers){
    std::vector<std::string> info = Type3(code);
    switch(info.at(0).at(0)){
        case 'O':
            registers.at(stoi(info.at(1))) = (registers.at(stoi(info.at(2))) | registers.at(stoi(info.at(3))));
            break;
        case 'E':
            registers.at(stoi(info.at(1))) = (registers.at(stoi(info.at(2))) ^ registers.at(stoi(info.at(3))));
            break;
        case 'S':
            registers.at(stoi(info.at(1))) = (registers.at(stoi(info.at(2))) - registers.at(stoi(info.at(3))));
            break;
        case 'A':
            if(info.at(0).at(1) == 'D'){
                registers.at(stoi(info.at(1))) = (registers.at(stoi(info.at(2))) + registers.at(stoi(info.at(3))));
            }
            else{
                registers.at(stoi(info.at(1))) = (registers.at(stoi(info.at(2))) & registers.at(stoi(info.at(3))));
            }
            break;
        case 'L':
            int factor = 1;
            for(int i = 0; i < registers.at(stoi(info.at(3))); i++){
                factor *= 2;
            }
            if(info.at(0).at(2) == 'L'){
                registers.at(stoi(info.at(1))) = (unsigned)registers.at(stoi(info.at(2))) << (registers.at(stoi(info.at(3))));
            }
            else{
                registers.at(stoi(info.at(1))) = (unsigned)registers.at(stoi(info.at(2))) >> (registers.at(stoi(info.at(3))));
            }
    }
}

std::vector<std::string> Type4(const std::string& code){
    std::vector<std::string> results;
    int op = NumDecode(code.substr(3, 8));
    int srcdest = NumDecode(code.substr(11, 5));
    int src1 = NumDecode(code.substr(16, 5));
    int imm = NumDecodeTwos(code.substr(21, 11));
    switch(op){
        case 170:
            results.push_back("LDUR");
            break;
        case 171:
            results.push_back("STUR");
            break;
        default:
            break;
    }
    results.push_back(std::to_string(srcdest));
    if(src1 == 31){
        results.push_back("ZR");
    }
    else {
        results.push_back(std::to_string(src1));
    }
    results.push_back(std::to_string(imm));
    return results;
}

void Type4Sim(std::string& code, std::vector<int>& registers, std::map<int, int>& memory){
    std::vector<std::string> info = Type4(code);
    if(info.at(0).at(0) == 'L'){
        if(info.at(2) == "ZR"){
            registers.at(stoi(info.at(1))) = memory[stoi(info.at(3))];
        }
        else{
            registers.at(stoi(info.at(1))) = memory[registers.at(stoi(info.at(2))) + stoi(info.at(3))];
        }
    }
    else{
        memory[registers.at(stoi(info.at(2))) + stoi(info.at(3))] = registers.at(stoi(info.at(1)));
    }
}

void CyclePrint(int cycles, int category, std::vector<int>& registers, std::vector<std::string>& instruction, std::map<int, int>& memory){

}

void Simulate(std::string fileName, std::vector<int>& registers, std::vector<std::string>& instructions, std::map<int, int>& memory){
    std::ofstream output;
    output.open(fileName);
    int cycles = 1;
    int index = 0;
    if(output.is_open()){
        std::string current = instructions.at(index);
        do{//While not dummy instructions (type 5)
            output << "--------------------" << std::endl;
            output << "Cycle " << cycles << ": \t" + current.substr(33, current.size() - 33) << std::endl;
            output << std::endl;
            switch(NumDecode(current.substr(0, 3))){
                case 1: //Branch -> op, src, offset, binary, category
                    index = Type1Sim(current, registers, index);
                    break;
                case 2:
                    Type2Sim(current, registers);
                    index += 1;
                    break;
                case 3:
                    Type3Sim(current,registers);
                    index += 1;
                    break;
                case 4:
                    Type4Sim(current, registers, memory);
                    index += 1;
                    break;
                case 5:
                    index += 1;
                default:
                    break;
            }
            cycles++;
            output << "Registers";
            for(int i = 0; i < 32; i++){
                if(i % 8 == 0){
                    output << std::endl;
                    if(i < 10){
                        output << "X0" << i << ":";
                    }
                    else{
                        output << "X" << i << ":";
                    }
                }
                output << "\t" << registers.at(i);
            }
            output << std::endl;
            output << std::endl;
            output << "Data";
            for(int i = 0; i < memory.size(); i++){
                if(i % 8 == 0){
                    output << std::endl;
                    output << (i * 4 + memory.begin()->first) << ": ";
                }
                output << "\t" << memory[i * 4 + memory.begin()->first];
            }
            output << std::endl;
            output << std::endl;
            current = instructions.at(index);
        }
        while(NumDecode(current.substr(0, 3)) != 5);
        output << "--------------------" << std::endl;
        output << "Cycle " << cycles << ": \t" + current.substr(33, current.size() - 33) << std::endl;
        output << std::endl;
        output << "Registers";
        for(int i = 0; i < 32; i++){
            if(i % 8 == 0){
                output << std::endl;
                if(i < 10){
                    output << "X0" << i << ":";
                }
                else{
                    output << "X" << i << ":";
                }
            }
            output << "\t" << registers.at(i);
        }
        output << std::endl;
        output << std::endl;
        output << "Data";
        for(int i = 0; i < memory.size(); i++){
            if(i % 8 == 0){
                output << std::endl;
                output << (i * 4 + memory.begin()->first) << ": ";
            }
            output << "\t" << memory[i * 4 + memory.begin()->first];
        }
        output << std::endl;
        output << std::endl;
    }
}


std::vector<std::string> WriteDis(std::string fileName, std::vector<std::string>& instructions, std::map<int, int>& mem){
    std::ofstream output;
    output.open(fileName);
    std::vector<std::string> temp;
    if(output.is_open()) {
        int category;
        int instNumber = 64;
        for (std::string code : instructions) {
            category = NumDecode(code.substr(0, 3));
            std::vector<std::string> info;
            switch (category) {
                case 1 :
                    info = (Type1(code)); //op,src,offset
                    temp.push_back(code + "\t" + std::to_string(instNumber) + "\t" + info.at(0) + " X" + info.at(1) + ", #" + info.at(2));
                    break;
                case 2:
                    info = Type2(code);//op, dest, src1, imm
                    temp.push_back(code + "\t" + std::to_string(instNumber) + "\t" + info.at(0) + " X" + info.at(1) + ", X" + info.at(2) + ", #" + info.at(3));
                    break;
                case 3:
                    info = Type3(code); //op, dest, src1, src 2
                    temp.push_back(code + "\t" + std::to_string(instNumber) + "\t" + info.at(0) + " X" + info.at(1) + ", X" + info.at(2) + ", X" + info.at(3));
                    break;
                case 4:
                    info = Type4(code); //op, dest, src1, imm
                    temp.push_back(code + "\t" + std::to_string(instNumber) + "\t" + info.at(0) + " X" + info.at(1) + ", [X" + info.at(2) + ", #" + info.at(3) + "]");
                    break;
                case 5:
                    temp.push_back(code + "\t" + std::to_string(instNumber) + "\t" + "DUMMY");
                    break;
                default://Possible memory locations
                    mem[instNumber] = NumDecodeTwos(code);
                    temp.push_back(code + "\t" + std::to_string(instNumber) + "\t" + std::to_string(NumDecodeTwos(code)));
            }
            output << temp.at(temp.size() - 1) << std::endl;
            instNumber +=4;
        }
    }
    return temp; //Full disassemble command
}

int main(int argc, char** argv) {
    std::vector<int> registers(32, 0);
    std::vector<std::string> instructions;
    std::map<int, int> memory;
    ReadIn(argv[1], instructions);//Change later to read in file name
    std::vector<std::string> translated = WriteDis("disassembly.txt", instructions, memory);
    Simulate("simulation.txt", registers, translated, memory);
}

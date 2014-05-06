//
//  main.cpp
//  Halstead Token-Count Metrics Analyzer
//
//  Created by Terry Chun Wong on 5/1/14.
//  Copyright (c) 2014 San Francisco State University. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <map>
#include <math.h>
#include <iomanip>
using namespace std;

const int MAX_CHARS_PER_LINE = 1024;
const int MAX_TOKENS_PER_LINE = 500;
const char* const DELIMITER = " ";
int LN, LLOC, PLOC, NPU, NSC, NEQ, Nif, Nswitch, Nwhile, Nfor;
// supported data types
const char* data_types[19] = {"char", "char*", "short", "short*", "int", "int*", "long", "long*", "float", "float*", "double", "double*", "void", "void*", "decltype", "unsigned", "signed", "string", "string*"};
/*
 n1 = the number of distinct operators used in a program (operators specify actions)
 n2 = the number of distinct operands used in a program (operands specify data)
 N1 = the total number of operators in a program
 N2 = the total number of operands in a program
 n1* = the limit value of n1 for the most compact notation of the program (n1*=2, i.e. a procedure name and a separator: <name>, <parameter_1>, …, <parameter_n>)
 n2* = the limit value of n2 for the most compact notation of the program (the number of distinct input and output parameters of the program)
 */
double n1=0, n2=0, N1=0, N2=0, n1_limit = 0, n2_limit = -1, n, N, N_estimate, V, V_min, L, L_estimate, D, Lambda, Lambda_estimate, I, E, T_estimate, S = 18, M, B_estimate, EFM_size = 0;
// supported operators excluding control structure operators
const char* operators[] = {"main", "printf", "scanf", "goto", "{", "(", "[", "*", ",", ";", ".", ":", "::", ">", "<", ">=", "<=", "!=", "==", "+", "-", "/", "%", "=", "+=", "-=", "*=", "/=", "%=", "&", "&&", "|", "||", "!", "<<", ">>", "sqrt", "log", "exp", "pow", "sin", "cos", "++", "--", "^", "->", "new", "new[]", "delete", "delete[]", "sizeof", "throw", "catch", "malloc", "free"};
const char* validated_operators[12] = { "+", "-", "*", "/", "%", "=", ">", "<", "&", "|", "!", ":"};
const char removed_chars[] = "\'})]\r\n\t";
map<string, int> operators_hash_map;
map<string, int> operands_hash_map;
typedef map<string, int>::iterator iter;
typedef map<size_t, string>::iterator checkIter;
int total_num_quotes = 0;
bool operator_found = false, str_ended = true, data_type_found = false;

// function to validate single-character operators
bool validateOperator(string token, char op) {

    int c = (int) op;
    size_t op_pos = token.find(op);
    
    switch (c) {
        case 43:
            // "+" operator
            if (op_pos!=string::npos && op_pos!=token.find("+=") && op_pos!=token.find("++"))
                return true;
            else
                return false;
            break;
        case 45:
            // "-" operator
            if (op_pos!=string::npos && op_pos!=token.find("-=") && op_pos!=token.find("--"))
                return true;
            else
                return false;
            break;
        case 42:
            // "*" operator
            if (op_pos!=string::npos && op_pos!=token.find("*="))
                return true;
            else
                return false;
            break;
        case 47:
            // "/" operator
            if (op_pos!=string::npos && op_pos!=token.find("/="))
                return true;
            else
                return false;
            break;
        case 37:
            // "%" operator
            if (op_pos!=string::npos && op_pos!=token.find("%="))
                return true;
            else
                return false;
            break;
        case 61:
            // "=" operator
            if (op_pos!=string::npos && op_pos!=token.find("=="))
                return true;
            else
                return false;
            break;
        case 62:
            // ">" operator
            if (op_pos!=string::npos && op_pos!=token.find(">=") && op_pos!=token.find(">>") && op_pos!=(token.find("->")+1))
                return true;
            else
                return false;
            break;
        case 60:
            // "<" operator
            if (op_pos!=string::npos && op_pos!=token.find("<=") && op_pos!=token.find("<<"))
                return true;
            else
                return false;
            break;
        case 33:
            // "!" operator
            if (op_pos!=string::npos && op_pos!=token.find("!="))
                return true;
            else
                return false;
            break;
        case 38:
            // "&" operator
            if (op_pos!=string::npos && op_pos!=token.find("&&"))
                return true;
            else
                return false;
            break;
        case 124:
            // "|" operator
            if (op_pos!=string::npos && op_pos!=token.find("||"))
                return true;
            else
                return false;
            break;
        case 58:
            // ":" operator
            if (op_pos!=string::npos && op_pos!=token.find("::") && op_pos!=token.find("::")+1 && token.find(":")!=token.length()-1) {
                cout << ":" << token << endl;
                return true;
            }
            else
                return false;
            break;
        default:
            cout << "The operator is not supported!";
            return false;
    }
    
    return false;
}

// function to check if token has more operators
bool hasMoreOperators(string tmp, size_t pos) {
    for (int i = 0; i < sizeof(operators)/sizeof(char*); i++) {
        if (tmp.find(operators[i], pos+1)!=string::npos)
            return true;
    }
    
    return false;
}

// function to check whether an operator is within a string
bool OpInString(size_t op_pos, string tmp) {
    size_t double_quotes_begin=string::npos, double_quotes_end=string::npos;
    
    double_quotes_begin = tmp.find("\"");
    double_quotes_end = tmp.find_last_of("\"");
    if (double_quotes_begin!=string::npos && total_num_quotes%2==0 && tmp.find(":")!=string::npos) {
        if (double_quotes_begin!=double_quotes_end && op_pos>=double_quotes_begin && op_pos<=double_quotes_end)
            return true;
        else if (double_quotes_begin==double_quotes_end && op_pos<=double_quotes_end)
            return true;
    }
    else if (double_quotes_begin!=string::npos && total_num_quotes%2!=0 && tmp.find(":")!=string::npos) {
        if (double_quotes_begin!=double_quotes_end && op_pos<=double_quotes_begin && op_pos>=double_quotes_end)
            return true;
        else if (double_quotes_begin==double_quotes_end && op_pos>=double_quotes_begin)
            return true;
    }
    
    return false;
}

// function to check for operators in a token
void checkOperator(string tmp) {
    size_t is_operator=string::npos, operator_len, operand_start_pos;
    map<size_t, string> operators_pos_hash_map;
    
    for (int i = 0; i < sizeof(operators)/sizeof(char*); i++) {
        is_operator = tmp.find(operators[i]);
        if (is_operator!=string::npos && !OpInString(is_operator, tmp)) {
            if (strlen(operators[i])==1 && find(validated_operators, validated_operators+12, operators[i])!=validated_operators+12) {
                const char* pos = operators[i];
                if (validateOperator(tmp, *pos)) {
                    operators_pos_hash_map[is_operator] = operators[i];
                    operators_hash_map[operators[i]]++;
                    operator_found = true;
                }
            }
            else {
                operators_pos_hash_map[is_operator] = operators[i];
                operators_hash_map[operators[i]]++;
                operator_found = true;
            }
        }
    }
    
    if (!data_type_found && tmp.find("\"")==string::npos && tmp.find("main(")==string::npos && operators_pos_hash_map.size()!=0) {
        for (checkIter iterator = operators_pos_hash_map.begin(); iterator != operators_pos_hash_map.end(); iterator++) {
            //cout << setw(10) << "Position: " << setw(25) << left << iterator->first << "\tOperator: " << iterator->second << endl;
            if (iterator == operators_pos_hash_map.begin()) {
                string operand = tmp.substr(0, iterator->first);
                for (int i = 0; i < strlen(removed_chars); i++)
                    operand.erase(remove(operand.begin(), operand.end(), removed_chars[i]), operand.end());
                if (operand.length()>0) {
                    operands_hash_map[operand]++;
                    //cout << "Getting before operand: " <<  operand << endl;
                }
            }
            else if (hasMoreOperators(tmp, iterator->first) || iterator->first==tmp.length()-1)
            {
                checkIter iteratorHolder = iterator;
                iteratorHolder--;
                operator_len = iteratorHolder->second.length();
                operand_start_pos = iteratorHolder->first+operator_len;
                string operand = tmp.substr(operand_start_pos, iterator->first-operand_start_pos);
                for (int i = 0; i < strlen(removed_chars); i++)
                    operand.erase(remove(operand.begin(), operand.end(), removed_chars[i]), operand.end());
                if (operand.length()>0) {
                    operands_hash_map[operand]++;
                    //cout << "Getting in-between operand: " <<  operand << endl;
                }
            }
            else {
                operator_len = iterator->second.length();
                string operand = tmp.substr(iterator->first+operator_len, tmp.length());
                for (int i = 0; i < strlen(removed_chars); i++)
                    operand.erase(remove(operand.begin(), operand.end(), removed_chars[i]), operand.end());
                if (operand.length()>0) {
                    operands_hash_map[operand]++;
                    //cout << "Getting after operand: " <<  operand << endl;
                }
            }
        }
    }
}

int main()
{
    size_t bc_start_found=string::npos, bc_end_found=string::npos, found=string::npos;
    size_t str_begin=string::npos, str_end=string::npos, char_begin=string::npos, char_end=string::npos;
    LN = 0;
    LLOC = PLOC = NPU = NSC = NEQ = Nif = Nswitch = Nwhile = Nfor =0;
    string filename;
    
    printf("Working Dir: %s\n", getcwd(NULL, 0));
    cout << "Please enter the filename for analysis from the Halstead Token-Count Metrics Analyzer:\n";
    cin >> filename;
    while (n1_limit<2) {
        cout << "n1 is the number of distinct operators used in a program.\n";
        cout << "Please enter the limit value of n1, n1* (at least 2 by default, the procedure name and a separator):\n";
        cin >> n1_limit;
    }
    while (n2_limit<0) {
        cout << "n2 is the number of distinct operands used in a program.\n";
        cout << "Please enter the limit value of n2, n2* (the number of distinct input and output parameters):\n";
        cin >> n2_limit;
    }
    
    // create a file-reading object
    ifstream fin;
    fin.open(filename); // open a file
    if (!fin.good())
        return -1; // exit if file not found
    
    // read each line of the file
    while (!fin.eof())
    {
        LN++;
        // read an entire line into memory
        char buf[MAX_CHARS_PER_LINE];
        char saved_buf[MAX_CHARS_PER_LINE];
        fin.getline(buf, MAX_CHARS_PER_LINE);
        strcpy(saved_buf,buf);
        // exclude block comment
        string str(saved_buf);
        if (bc_start_found!=string::npos) {
            //cout << buf << endl;
            bc_end_found = str.find("*/");
        }
        if (bc_start_found!=string::npos && bc_end_found!=string::npos) {
            bc_start_found=string::npos;
            bc_end_found=string::npos;
            continue;
        }
        
        // check how many string operands the line contains
        str_begin = string::npos;
        str_end = string::npos;
        str_begin = str.find_first_of("\"");
        str_end = str.find_last_of("\"");
        while (str_begin!=string::npos && str_begin <= str_end) {
            operands_hash_map[str.substr(str_begin, str.find("\"", str_begin+1)-str_begin+1)]++;
            //cout << "string: " << str.substr(str_begin, str.find("\"", str_begin+1)-str_begin+1) << endl;
            str_begin = str.find("\"", str_begin+1);
            str_begin = str.find("\"", str_begin+1);
        }
        
        // check how many chars the line contains
        char_begin = string::npos;
        char_end = string::npos;
        char_begin = str.find_first_of("\'");
        char_end = str.find_last_of("\'");
        while (char_begin!=string::npos && char_begin <= str_end) {
            operators_hash_map["\'"]++;
            //cout << "char: " << str.substr(char_begin, str.find("\'", char_begin+1)-char_begin+1) << endl;
            char_begin = str.find("\'", char_begin+1);
            char_begin = str.find("\'", char_begin+1);
        }
        
        // parse the line into blank-delimited tokens
        int n = 0; // a for-loop index
        
        // array to store memory addresses of the tokens in buf
        const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0
        
        // parse the line
        token[0] = strtok(saved_buf, DELIMITER); // first token
        if (token[0]) // zero if line is blank
        {
            string tmp(token[0]);
            // exclude block comment
            if (bc_start_found==string::npos)
                bc_start_found = tmp.find("/*");
            if (bc_start_found!=string::npos) {
                //cout << buf << endl;
                continue;
            }

            // exclude line comment
            found = tmp.find("//");
            if (found==string::npos) {
                size_t dir_found = tmp.find("#include");
                size_t ns_found = tmp.find("using");
                if (dir_found==string::npos && ns_found==string::npos)
                    PLOC++;
                else
                    continue;
            }
            else {
                //cout << buf << endl;
                continue;
            }
            // count control structures
            found = tmp.find("if");
            if (found!=string::npos) {
                Nif++;
                LLOC++;
                operators_hash_map["if"]++;
                operator_found = true;
            }
            found = tmp.find("else");
            if (found!=string::npos) {
                operators_hash_map["else"]++;
            }
            found = tmp.find("do");
            if (found!=string::npos) {
                operators_hash_map["do"]++;
                operator_found = true;
            }
            found = tmp.find("switch");
            if (found!=string::npos) {
                Nswitch++;
                LLOC++;
                operators_hash_map["switch"]++;
                operator_found = true;
            }
            found = tmp.find("while");
            if (found!=string::npos) {
                Nwhile++;
                LLOC++;
                operators_hash_map["while"]++;
                operator_found = true;
            }
            found = tmp.find("for");
            if (found!=string::npos) {
                Nfor++;
                LLOC++;
                operators_hash_map["for"]++;
                operator_found = true;
                found = tmp.find(";");
                if (found!=string::npos) {
                    NSC++;
                    LLOC++;
                }
            }
            if (tmp.compare("case")==0) {
                found = str.find_last_of(":");
                if (found!=string::npos && found==str.length()-1) {
                    operators_hash_map["case"]++;
                    operator_found = true;
                }
            }
            else if (str.find_last_of(":")!=string::npos && str.find_last_of(":")==tmp.length()-1) {
                operands_hash_map["label"]++;
            }
            
            // check if statement is data specification or has return type
            const char **foo = find(data_types, data_types+19, tmp);
            if (foo != data_types+19 && str.find("\"")==string::npos) {
                operators_hash_map[tmp]++;
                operator_found = true;
                data_type_found = true;
            }
            
            // check how many string operators the token contains
            str_begin = string::npos;
            str_end = string::npos;
            str_begin = tmp.find_first_of("\"");
            str_end = tmp.find_last_of("\"");
            while (str_begin!=string::npos && str_begin <= str_end) {
                total_num_quotes++;
                if (str_ended == false && total_num_quotes%2 == 0) {
                    operators_hash_map["\""]++;
                    operator_found = true;
                    str_ended = true;
                }
                else {
                    str_ended = false;
                }
                str_begin = tmp.find("\"", str_begin+1);
            }
            
            // check if an operator exist in first token
            checkOperator(tmp);
            
            if (operator_found == false && str_ended == true && tmp.compare("}")!=0) {
                string operand = tmp;
                for (int i = 0; i < strlen(removed_chars); i++)
                    operand.erase(remove(operand.begin(), operand.end(), removed_chars[i]), operand.end());
                if (operand.length()>0) {
                    operands_hash_map[operand]++;
                    //cout << "Getting unmatched operand: " << operand << endl;
                }
            } else {
                operator_found = false;
                data_type_found = false;
            }
            
            size_t lp_found=string::npos, rp_found=string::npos;
            for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
            {
                token[n] = strtok(NULL, DELIMITER); // subsequent tokens
                if (!token[n]) {
                    if (foo != data_types+19) {
                        string tmp(token[n-1]);
                        rp_found = tmp.find_last_of(")");
                        if (lp_found!=string::npos && rp_found==tmp.length()-2) {
                            NPU++;
                            LLOC++;
                        }
                    }
                    break; // no more tokens
                }
                // statement is data spec or has a return type
                if (foo != data_types+19) {
                    string tmp(token[n]);
                    // count NPU
                    if (n==1)
                        lp_found = tmp.find("(");
                    // count NEQ
                    found = tmp.find("=");
                    if (found!=string::npos) {
                        //cout << "found =" << LN << endl;
                        NEQ++;
                        LLOC++;
                    }
                }
                // count NSC
                string tmp(token[n]);
                found = tmp.find(";");
                if (found!=string::npos) {
                    NSC++;
                    LLOC++;
                }
                
                const char **is_data_type = find(data_types, data_types+19, tmp);
                if (is_data_type != data_types+19 && tmp.find("\"")==string::npos) {
                    operators_hash_map[tmp]++;
                    operator_found = true;
                    data_type_found = true;
                }
                
                // check how many string operators the token contains
                str_begin = string::npos;
                str_end = string::npos;
                str_begin = tmp.find_first_of("\"");
                str_end = tmp.find_last_of("\"");
                while (str_begin!=string::npos && str_begin <= str_end) {
                    total_num_quotes++;
                    if (str_ended == false && total_num_quotes%2 == 0) {
                        operators_hash_map["\""]++;
                        operator_found = true;
                        str_ended = true;
                    }
                    else {
                        str_ended = false;
                    }
                    str_begin = tmp.find("\"", str_begin+1);
                }
                
                // check if an operator exist in other token
                checkOperator(tmp);

                if (operator_found == false && str_ended == true && str.find_last_of(":")!=str.length()-1) {
                    string operand = tmp;
                    for (int i = 0; i < strlen(removed_chars); i++)
                        operand.erase(remove(operand.begin(), operand.end(), removed_chars[i]), operand.end());
                    if (operand.length()>0) {
                        operands_hash_map[operand]++;
                        //cout << "Getting unmatched operand: " << operand << endl;
                    }
                } else {
                    operator_found = false;
                    data_type_found = false;
                }
            }
        }
        
        // process (print) the tokens
        /*for (int i = 0; i < n; i++) // n = #of tokens
         cout << "Token[" << i << "] = " << token[i] << endl;
         cout << endl;*/
    }

    for (int i =0; i < 70; i++)
        cout << "*";
    cout << endl;
    for (iter iterator = operators_hash_map.begin(); iterator != operators_hash_map.end(); iterator++) {
        if (iterator->second != 0) {
            n1++;
            N1+=iterator->second;
        }
        cout << setw(10) << "Operator: " << setw(25) << left << iterator->first << "\tFrequency: " << iterator->second << endl;
    }
    for (iter iterator = operands_hash_map.begin(); iterator != operands_hash_map.end(); iterator++) {
        if (iterator->second != 0) {
            n2++;
            N2+=iterator->second;
        }
        cout << setw(10) << "Operand: " << setw(25) << left << iterator->first << "\tFrequency: " << iterator->second << endl;
    }
    for (int i =0; i < 70; i++)
        cout << "*";
    cout << endl;
    
    // Calculate the Halstead SW Science
    n = n1 + n2; // tokens
    N = N1 + N2;
    N_estimate = ceil(n1*log2(n1) + n2*log2(n2)); // tokens
    V = ceil(N*log2(n)); // bits
    V_min = ceil((2 + n2_limit) * log2(2+n2_limit)); // bits
    L = V_min/V; // Lmax = 1
    L_estimate = (2*n2)/(n1*N2);
    D = 1/L; // D~ = V/V*~, V*=VL~
    Lambda = L*V_min;
    Lambda_estimate = L_estimate*V_min;
    I = L_estimate*V; // Note: LV = V*
    E = D*V; // S~ = 18 sec^-1
    T_estimate = E/S; // sec
    M = n2_limit/6; // modules
    B_estimate = V/3000; // bugs

    cout << "\nLLOC: " << LLOC << endl;
    cout << "The number of distinct operators, n1: " << n1 << endl;
    cout << "The number of distinct operands, n2: " << n2 << endl;
    cout << "The total number of operators, N1: " << N1 << endl;
    cout << "The total number of operands, N2: " << N2 << endl;
    cout << "The limit value of n1, n1*: " << n1_limit << endl;
    cout << "The limit value of n2, n2*: " << n2_limit << endl;
    cout << "The vocabulary of a program, n: " << n << " [tokens]" << endl;
    cout << "The length of a program, N: " << N << " [tokens]" << endl;
    cout << "The program length estimate, N~: " << N_estimate << " [tokens]" << endl;
    cout << "The program volume, V: " << V << " [bits]" << endl;
    cout << "The potential (minimum) volume: " << V_min << " [bits]" << endl;
    cout << "The program level, L: " << L << " (Lmax = 1)" << endl;
    cout << "The program level estimate, L~: " << L_estimate << endl;
    cout << "The program difficulty, D: " << D << endl;
    cout << "The language level, Lambda: " << Lambda << endl;
    cout << "The language level estimate, Lambda~: " << Lambda_estimate << endl;
    cout << "The intelligence content, I: " << I << endl;
    cout << "The programming effort, E: " << E << " [elementary mental discriminations]" << endl;
    cout << "The programming (coding) time estimate, T~: " << T_estimate << " [sec]" << endl;
    cout << "The modularity formula, M: " << M << " [modules]" << endl;
    cout << "The error formula, B~: " << B_estimate << " [bugs]" << endl;
    cout << "The error-free modulus size (B < 1/2): " << (B_estimate<0.5?"50":"Greater than 50") << " [lines]" << endl;
    
    return 0;
}

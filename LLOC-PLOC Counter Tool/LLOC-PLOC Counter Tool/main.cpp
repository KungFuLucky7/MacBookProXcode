//
//  main.cpp
//  LLOC-PLOC Counter Tool
//
//  Created by Terry Chun Wong on 3/19/14.
//  Copyright (c) 2014 San Francisco State University. All rights reserved.
//  Reference: http://cs.dvc.edu/HowTo_Cparse.html
//

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
using namespace std;

const int MAX_CHARS_PER_LINE = 1024;
const int MAX_TOKENS_PER_LINE = 500;
const char* const DELIMITER = " ";
int LN, LLOC, PLOC, NPU, NSC, NEQ, Nif, Nswitch, Nwhile, Nfor;
// supported data types
const char* data_types[10] = {"char","short","int","long","float","double","void","decltype", "unsigned", "signed"};

int main()
{
    size_t bc_start_found=string::npos, bc_end_found=string::npos, found=string::npos;
    LN = 0;
    LLOC = PLOC = NPU = NSC = NEQ = Nif = Nswitch = Nwhile = Nfor =0;
    string filename;
    
    printf("Working Dir: %s\n", getcwd(NULL, 0));
    cout << "Please enter the filename for analysis from the LLOC/PLOC Counter Tool:\n";
    cin >> filename;
    
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
        fin.getline(buf, MAX_CHARS_PER_LINE);
        // exclude block comment
        string str(buf);
        if (bc_start_found==string::npos)
            bc_start_found = str.find("/*");
        if (bc_start_found!=string::npos) {
            cout << buf << endl;
            bc_end_found = str.find("*/");
            if (bc_end_found!=string::npos)
                cout << buf << endl;
        }
        if (bc_start_found!=string::npos && bc_end_found!=string::npos) {
            bc_start_found=string::npos;
            bc_end_found=string::npos;
            continue;
        }
        if (bc_start_found!=string::npos) {
            continue;
        }
        // parse the line into blank-delimited tokens
        int n = 0; // a for-loop index
        
        // array to store memory addresses of the tokens in buf
        const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0
        
        // parse the line
        token[0] = strtok(buf, DELIMITER); // first token
        if (token[0]) // zero if line is blank
        {
            string tmp(token[0]);
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
                cout << buf << endl;
                continue;
            }
            // count control structures
            found = tmp.find("if");
            if (found!=string::npos) {
                Nif++;
                LLOC++;
            }
            found = tmp.find("switch");
            if (found!=string::npos) {
                Nswitch++;
                LLOC++;
            }
            found = tmp.find("while");
            if (found!=string::npos) {
                Nwhile++;
                LLOC++;
            }
            found = tmp.find("for");
            if (found!=string::npos) {
                Nfor++;
                LLOC++;
                found = tmp.find(";");
                if (found!=string::npos) {
                    NSC++;
                    LLOC++;
                }
            }
            // check if statement is data specification or has return type
            const char **foo = find(data_types, data_types+10, tmp);
            size_t lp_found=string::npos, rp_found=string::npos;
            for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
            {
                token[n] = strtok(0, DELIMITER); // subsequent tokens
                if (!token[n]) {
                    if (foo != data_types+10) {
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
                if (foo != data_types+10) {
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
            }
        }

        // process (print) the tokens
        /*for (int i = 0; i < n; i++) // n = #of tokens
            cout << "Token[" << i << "] = " << token[i] << endl;
        cout << endl;*/
    }
    cout << "\nLN: " << LN << endl;
    cout << "NPU: " << NPU << endl;
    cout << "NSC: " << NSC << endl;
    cout << "NEQ: " << NEQ << endl;
    cout << "Nif: " << Nif << endl;
    cout << "Nswitch: " << Nswitch << endl;
    cout << "Nwhile: " << Nwhile << endl;
    cout << "Nfor: " << Nfor << endl << endl;
    cout << "LLOC: " << LLOC << endl;
    cout << "PLOC: " << PLOC << endl;
    
    return 0;
}


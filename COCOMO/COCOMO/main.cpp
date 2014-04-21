//
//  main.cpp
//  COCOMO
//
//  Created by Terry Chun Wong on 4/16/14.
//  Copyright (c) 2014 San Francisco State University. All rights reserved.
//

#include <iostream>
#include <cmath>
using namespace std;

int Level;
double m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15, m16;

// Display the Cost Drivers table
void DisplayTable() {
    printf("\n\n|%-35s|%-65s|\n", "Cost Drivers", "Rating");
    printf("--------------------------------------------------------------------------------------------------------\n");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "", "Very Low", "Low", "Nominal", "High", "Very High", "Extra High");
    printf("--------------------------------------------------------------------------------------------------------\n");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Product Attributes", "", "", "", "", "", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Required software reliability: m1", m1==0.75?"0.75*":"0.75", m1==0.88?"0.88*":"0.88", m1==1.00?"1.00*":"1.00", m1==1.15?"1.15*":"1.15", m1==1.40?"1.40*":"1.40", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Data base size: m2", "", m2==0.94?"0.94*":"0.94", m2==1.00?"1.00*":"1.00", m2==1.08?"1.08*":"1.08", m2==1.16?"1.16*":"1.16", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Product complexity: m3", m3==0.70?"0.70*":"0.70", m3==0.85?"0.85*":"0.85", m3==1.00?"1.00*":"1.00", m3==1.15?"1.15*":"1.15", m3==1.30?"1.30*":"1.30", m3==1.65?"1.65*":"1.65");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Computer Attributes", "", "", "", "", "", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Executio time constraint: m4", "", "", m4==1.00?"1.00*":"1.00", m4==1.11?"1.11*":"1.11", m4==1.30?"1.30*":"1.30", m4==1.66?"1.66*":"1.66");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Main storage constraint: m5", "", "", m5==1.00?"1.00*":"1.00", m5==1.06?"1.06*":"1.06", m5==1.21?"1.21*":"1.21", m5==1.56?"1.56*":"1.56");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Virtual machine volatility: m6", "", m6==0.87?"0.87*":"0.87", m6==1.00?"1.00*":"1.00", m6==1.15?"1.15*":"1.15", m6==1.30?"1.30*":"1.30", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Computer turnaround time: m7", "", m7==0.87?"0.87*":"0.87", m7==1.00?"1.00*":"1.00", m7==1.07?"1.07*":"1.07", m7==1.15?"1.15*":"1.15", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Personnel Attributes", "", "", "", "", "", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Analyst capabilities: m8", m8==1.46?"1.46*":"1.46", m8==1.19?"1.19*":"1.19", m8==1.00?"1.00*":"1.00", m8==0.86?"0.86*":"0.86", m8==0.71?"0.71*":"0.71", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Applications experience: m9", m9==1.29?"1.29*":"1.29", m9==1.13?"1.13*":"1.13", m9==1.00?"1.00*":"1.00", m9==0.91?"0.91*":"0.91", m9==0.82?"0.82*":"0.82", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Programmer capability: m10", m10==1.42?"1.42*":"1.42", m10==1.17?"1.17*":"1.17", m10==1.00?"1.00*":"1.00", m10==0.86?"0.86*":"0.86", m10==0.70?"0.70*":"0.70", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Virtual machine experience: m11", m11==1.21?"1.21*":"1.21", m11==1.10?"1.10*":"1.10", m11==1.00?"1.00*":"1.00", m11==0.90?"0.90*":"0.90", "", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Programming language exp.: m12", m12==1.14?"1.14*":"1.14", m12==1.07?"1.07*":"1.07", m12==1.00?"1.00*":"1.00", m12==0.95?"0.95*":"0.95", "", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Project Attributes", "", "", "", "", "", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Use of modern prog. practices: m13", m13==1.24?"1.24*":"1.24", m13==1.10?"1.10*":"1.10", m13==1.00?"1.00*":"1.00", m13==0.91?"0.91*":"0.91", m13==0.82?"0.82*":"0.82", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Use of software tools: m14", m14==1.24?"1.24*":"1.24", m14==1.10?"1.10*":"1.10", m14==1.00?"1.00*":"1.00", m14==0.91?"0.91*":"0.91", m14==0.83?"0.83*":"0.83", "");
    printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Required development schedule: m15", m15==1.23?"1.23*":"1.23", m15==1.08?"1.08*":"1.08", m15==1.00?"1.00*":"1.00", m15==1.04?"1.04*":"1.04", m15==1.10?"1.10*":"1.10", "");
    if (Level == 3) {
        printf("|%-35s|%-10s|%-10s|%-10s|%-10s|%-10s|%-10s|\n", "Requirements volatility: m16", "", m16==0.91?"0.91*":"0.91", m16==1.00?"1.00*":"1.00", "", "", m16==1.62?"1.62*":"1.62");
    }
    printf("--------------------------------------------------------------------------------------------------------\n");
    printf("\n\n");
}

// Update the Cost Drivers
void UpdateCostDrivers() {
    int cd = 0, cdr = 0;
    string update = "", cd_choice = "", done = "";
    bool reset = false;
    
    DisplayTable();
    while (update != "Y" && update != "y" && update != "N" && update != "n") {
        cout << "Do you want to update the cost drivers? (y/n)\n";
        cin >> update;
    }
    if (update == "N" || update == "n")
        return;

    do {
        done = "";
        do {
            cout << "Please enter the cost driver that you want to change (m1 to " << (Level==3?"m16":"m15") << "):\n";
            cin >> cd_choice;
            
            if (Level == 2 && cd_choice != "m1" && cd_choice != "m2" && cd_choice != "m3" && cd_choice != "m4" && cd_choice != "m5" && cd_choice != "m6" && cd_choice != "m7" && cd_choice != "m8" && cd_choice != "m9" && cd_choice != "m10" && cd_choice != "m11" && cd_choice != "m12" && cd_choice != "m13" && cd_choice != "m14" && cd_choice != "m15") {
                reset = true;
                continue;
            }
            else if (Level == 3 && cd_choice != "m1" && cd_choice != "m2" && cd_choice != "m3" && cd_choice != "m4" && cd_choice != "m5" && cd_choice != "m6" && cd_choice != "m7" && cd_choice != "m8" && cd_choice != "m9" && cd_choice != "m10" && cd_choice != "m11" && cd_choice != "m12" && cd_choice != "m13" && cd_choice != "m14" && cd_choice != "m15" && cd_choice != "m16") {
                reset = true;
                continue;
            }
            reset = false;
            if (cd_choice == "m1")
                cd = 1;
            else if (cd_choice == "m2")
                cd = 2;
            else if (cd_choice == "m3")
                cd = 3;
            else if (cd_choice == "m4")
                cd = 4;
            else if (cd_choice == "m5")
                cd = 5;
            else if (cd_choice == "m6")
                cd = 6;
            else if (cd_choice == "m7")
                cd = 7;
            else if (cd_choice == "m8")
                cd = 8;
            else if (cd_choice == "m9")
                cd = 9;
            else if (cd_choice == "m10")
                cd = 10;
            else if (cd_choice == "m11")
                cd = 11;
            else if (cd_choice == "m12")
                cd = 12;
            else if (cd_choice == "m13")
                cd = 13;
            else if (cd_choice == "m14")
                cd = 14;
            else if (cd_choice == "m15")
                cd = 15;
            else if (cd_choice == "m16")
                cd = 16;
            
            switch (cd) {
                case 1:
                    do {
                        cout << "1. Very Low 0.75; 2. Low 0.88; 3. Nominal 1.00; 4. High 1.15; 5. Very High 1.40;\n";
                        cout << "Please enter the m1 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4 && cdr != 5);
                    if (cdr == 1)
                        m1 = 0.75;
                    else if (cdr == 2)
                        m1 = 0.88;
                    else if (cdr == 3)
                        m1 = 1.00;
                    else if (cdr == 4)
                        m1 = 1.15;
                    else if (cdr == 5)
                        m1 = 1.40;
                    break;
                case 2:
                    do {
                        cout << "1. Low 0.94; 2. Nominal 1.00; 3. High 1.08; 4. Very High 1.16;\n";
                        cout << "Please enter the m2 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4);
                    if (cdr == 1)
                        m2 = 0.94;
                    else if (cdr == 2)
                        m2 = 1.00;
                    else if (cdr == 3)
                        m2 = 1.08;
                    else if (cdr == 4)
                        m2 = 1.16;
                    break;
                case 3:
                    do {
                        cout << "1. Very Low 0.70; 2. Low 0.85; 3. Nominal 1.00; 4. High 1.15; 5. Very High 1.30; 6. Extra High 1.65;\n";
                        cout << "Please enter the m3 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4 && cdr != 5 && cdr != 6);
                    if (cdr == 1)
                        m3 = 0.70;
                    else if (cdr == 2)
                        m3 = 0.85;
                    else if (cdr == 3)
                        m3 = 1.00;
                    else if (cdr == 4)
                        m3 = 1.15;
                    else if (cdr == 5)
                        m3 = 1.30;
                    else if (cdr == 6)
                        m3 = 1.65;
                    break;
                case 4:
                    do {
                        cout << "1. Nominal 1.00; 2. High 1.11; 3. Very High 1.30; 4. Extra High 1.66;\n";
                        cout << "Please enter the m4 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4);
                    if (cdr == 1)
                        m4 = 1.00;
                    else if (cdr == 2)
                        m4 = 1.11;
                    else if (cdr == 3)
                        m4 = 1.30;
                    else if (cdr == 4)
                        m4 = 1.66;
                    break;
                case 5:
                    do {
                        cout << "1. Nominal 1.00; 2. High 1.06; 3. Very High 1.21; 4. Extra High 1.56;\n";
                        cout << "Please enter the m5 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4);
                    if (cdr == 1)
                        m5 = 1.00;
                    else if (cdr == 2)
                        m5 = 1.06;
                    else if (cdr == 3)
                        m5 = 1.21;
                    else if (cdr == 4)
                        m5 = 1.56;
                    break;
                case 6:
                    do {
                        cout << "1. Low 0.87; 2. Nominal 1.00; 3. High 1.15; 4. Very High 1.30;\n";
                        cout << "Please enter the m6 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4);
                    if (cdr == 1)
                        m6 = 0.87;
                    else if (cdr == 2)
                        m6 = 1.00;
                    else if (cdr == 3)
                        m6 = 1.15;
                    else if (cdr == 4)
                        m6 = 1.30;
                    break;
                case 7:
                    do {
                        cout << "1. Low 0.87; 2. Nominal 1.00; 3. High 1.07; 4. Very High 1.15;\n";
                        cout << "Please enter the m7 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4);
                    if (cdr == 1)
                        m7 = 0.87;
                    else if (cdr == 2)
                        m7 = 1.00;
                    else if (cdr == 3)
                        m7 = 1.07;
                    else if (cdr == 4)
                        m7 = 1.15;
                    break;
                case 8:
                    do {
                        cout << "1. Very Low 1.46; 2. Low 1.19; 3. Nominal 1.00; 4. High 0.86; 5. Very High 0.71;\n";
                        cout << "Please enter the m8 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4 && cdr != 5);
                    if (cdr == 1)
                        m8 = 1.46;
                    else if (cdr == 2)
                        m8 = 1.19;
                    else if (cdr == 3)
                        m8 = 1.00;
                    else if (cdr == 4)
                        m8 = 0.86;
                    else if (cdr == 5)
                        m8 = 0.71;
                    break;
                case 9:
                    do {
                        cout << "1. Very Low 1.29; 2. Low 1.13; 3. Nominal 1.00; 4. High 0.91; 5. Very High 0.82;\n";
                        cout << "Please enter the m9 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4 && cdr != 5);
                    if (cdr == 1)
                        m9 = 1.29;
                    else if (cdr == 2)
                        m9 = 1.13;
                    else if (cdr == 3)
                        m9 = 1.00;
                    else if (cdr == 4)
                        m9 = 0.91;
                    else if (cdr == 5)
                        m9 = 0.82;
                    break;
                case 10:
                    do {
                        cout << "1. Very Low 1.42; 2. Low 1.17; 3. Nominal 1.00; 4. High 0.86; 5. Very High 0.70;\n";
                        cout << "Please enter the m10 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4 && cdr != 5);
                    if (cdr == 1)
                        m10 = 1.42;
                    else if (cdr == 2)
                        m10 = 1.17;
                    else if (cdr == 3)
                        m10 = 1.00;
                    else if (cdr == 4)
                        m10 = 0.86;
                    else if (cdr == 5)
                        m10 = 0.70;
                    break;
                case 11:
                    do {
                        cout << "1. Very Low 1.21; 2. Low 1.10; 3. Nominal 1.00; 4. High 0.90;\n";
                        cout << "Please enter the m11 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4);
                    if (cdr == 1)
                        m11 = 1.21;
                    else if (cdr == 2)
                        m11 = 1.10;
                    else if (cdr == 3)
                        m11 = 1.00;
                    else if (cdr == 4)
                        m11 = 0.90;
                    break;
                case 12:
                    do {
                        cout << "1. Very Low 1.14; 2. Low 1.07; 3. Nominal 1.00; 4. High 0.95;\n";
                        cout << "Please enter the m12 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4);
                    if (cdr == 1)
                        m12 = 1.14;
                    else if (cdr == 2)
                        m12 = 1.07;
                    else if (cdr == 3)
                        m12 = 1.00;
                    else if (cdr == 4)
                        m12 = 0.95;
                    break;
                case 13:
                    do {
                        cout << "1. Very Low 1.24; 2. Low 1.10; 3. Nominal 1.00; 4. High 0.91; 5. Very High 0.82;\n";
                        cout << "Please enter the m13 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4 && cdr != 5);
                    if (cdr == 1)
                        m13 = 1.24;
                    else if (cdr == 2)
                        m13 = 1.10;
                    else if (cdr == 3)
                        m13 = 1.00;
                    else if (cdr == 4)
                        m13 = 0.91;
                    else if (cdr == 5)
                        m13 = 0.82;
                    break;
                case 14:
                    do {
                        cout << "1. Very Low 1.24; 2. Low 1.10; 3. Nominal 1.00; 4. High 0.91; 5. Very High 0.83;\n";
                        cout << "Please enter the m14 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4 && cdr != 5);
                    if (cdr == 1)
                        m14 = 1.24;
                    else if (cdr == 2)
                        m14 = 1.10;
                    else if (cdr == 3)
                        m14 = 1.00;
                    else if (cdr == 4)
                        m14 = 0.91;
                    else if (cdr == 5)
                        m14 = 0.83;
                    break;
                case 15:
                    do {
                        cout << "1. Very Low 1.23; 2. Low 1.08; 3. Nominal 1.00; 4. High 1.04; 5. Very High 1.10;\n";
                        cout << "Please enter the m15 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3 && cdr != 4 && cdr != 5);
                    if (cdr == 1)
                        m15 = 1.23;
                    else if (cdr == 2)
                        m15 = 1.08;
                    else if (cdr == 3)
                        m15 = 1.00;
                    else if (cdr == 4)
                        m15 = 1.04;
                    else if (cdr == 5)
                        m15 = 1.10;
                    break;
                case 16:
                    do {
                        cout << "1. Low 0.91; 2. Nominal 1.00; 3. Extra High 1.62;\n";
                        cout << "Please enter the m16 rating:\n";
                        cin >> cdr;
                    } while (cdr != 1 && cdr != 2 && cdr != 3);
                    if (cdr == 1)
                        m16 = 0.91;
                    else if (cdr == 2)
                        m16 = 1.00;
                    else if (cdr == 3)
                        m16 = 1.62;
                    break;
                default:
                    cout << "Invalid cost driver selection!";
            }
        } while (reset);
        DisplayTable();
        while (done != "Y" && done != "y" && done != "N" && done != "n") {
            cout << "Are you done setting the cost drivers? (y/n)\n";
            cin >> done;
        }
    } while (done == "N" || done == "n");
}

int main()
{
    int M, L = -1, q, m_0 = 24324, m_1 = 295;
    double m, E, P, T, N, C, a, b, r = 2.5, s;
    char Mode;
    string project_size;
    
    m1 = m1 = m2 = m3 = m4 = m5 = m6 = m7 = m8 = m9 = m10 = m11 = m12 = m13 = m14 = m15 = m16 = 1;
    while (L < 0) {
        cout << "Please enter L, the number of thousands of delivered source instructions [KDSI/KLOC] or 0 to skip:\n";
        cin >> L;
    }
    if (L == 0) {
        while (M <= 0) {
            cout << "Please enter M ,memory consumption (the size of executable program) in Bytes:\n";
            cin >> M;
        }
        L = (M - m_0)/(1000*m_1);
    }
    if (L < 8)
        project_size = "Small";
    else if (L >= 8)
        project_size = "Intermediate";
    else if (L >= 32)
        project_size = "Medium";
    else if (L >= 128)
        project_size = "Large";
    else if (L >= 512)
        project_size = "Very Large";
    
    cout << "Please enter q, the monthly cost of an average FSP (Full-time Software Personnel): (dollars)\n";
    cin >> q;
    cout << "COCOMO levels of detail:\n";
    cout << "1. Basic (m = 1)\n2. Intermediate 1: m[1..15]\n3. Intermediate 2: m[1..16]\n\n";
    while (Level != 1 && Level != 2 && Level != 3) {
        cout << "What's your software project's level of details (1, 2 or 3)?\n";
        cin >> Level;
    }
    cout << "Software Project Modes:\n";
    cout << "Organic(O): relatively small size, little innovation, relaxed delivery requirements, development in a stable in-house environment;\nEmbedded(E): relatively large, operating within tight constraints, greater innovation needs, high hardware and customer interface complexity, rigid requirements;\nSemidetached(S): located in between the organic and embedded modes.\n\n";
    while (Mode != 'O' && Mode != 'o' && Mode != 'E' && Mode != 'e' && Mode != 'S' && Mode != 's') {
        cout << "What's your software project mode? (O, S or E)\n";
        cin >> Mode;
    }
    cout << Mode << endl;
    switch (Level) {
        case 1:
            m = 1;
            if (Mode == 'O' || Mode == 'o') {
                a = 2.4;
                b = 1.05;
                s = 0.38;
            }
            else if (Mode == 'E' || Mode == 'e') {
                a = 3.0;
                b = 1.12;
                s = 0.35;
            }
            else if (Mode == 'S' || Mode == 's') {
                a = 3.6;
                b = 1.20;
                s = 0.32;
            }
            break;
        case 2:
            UpdateCostDrivers();
            m = m1*m2*m3*m4*m5*m6*m7*m8*m9*m10*m11*m12*m13*m14*m15;
            if (Mode == 'O' || Mode == 'o') {
                a = 3.2;
                b = 1.05;
                s = 0.38;
            }
            else if (Mode == 'E' || Mode == 'e') {
                a = 3.0;
                b = 1.12;
                s = 0.35;
            }
            else if (Mode == 'S' || Mode == 's') {
                a = 2.8;
                b = 1.20;
                s = 0.32;
            }
            break;
        case 3:
            UpdateCostDrivers();
            m = m1*m2*m3*m4*m5*m6*m7*m8*m9*m10*m11*m12*m13*m14*m15*m16;
            if (Mode == 'O' || Mode == 'o') {
                a = 2.6;
                b = 1.08;
                s = 0.38;
            }
            else if (Mode == 'E' || Mode == 'e') {
                a = 2.9;
                b = 1.12;
                s = 0.35;
            }
            else if (Mode == 'S' || Mode == 's') {
                a = 2.9;
                b = 1.20;
                s = 0.32;
            }
            break;
        default:
            cout << "Invalid COCOMO level of detail!";
    }
    
    // Apply COCOMO formulas
    E = ceil(a*pow(L, b)*m);
    P = (double)L/E;
    T = ceil(r*pow(E, s));
    N = ceil(E/T);
    C = q*E;
    cout << "\nThe software project size is " << project_size << ".\n";
    cout << "L = " << L << ", the number of thousands of delivered source instructions [KDSI/KLOC]\n";
    cout << "E = " << E << ", effort for the SW development phase of the life-cycle [PM, person-months]\n";
    cout << "P = " << P << ", productivity [KDSI/PM]\n";
    cout << "T = " << T << ", software development time (sometimes denoted TDEV) in months\n";
    cout << "N = " << N << ", average number of Full-time-equivalent Software Personnel [FSP]\n";
    cout << "C = " << C << ", total development cost, expressed in same units as q (dollars)\n";
    
    return 0;
}


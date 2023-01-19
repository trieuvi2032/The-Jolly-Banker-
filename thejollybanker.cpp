#include "bank.h"
#include <iostream>
#include<fstream>
using namespace std;

int main(int argc, char* argv[]) {
    Bank jolly;
    jolly.ReadFile("bank_trans_in.txt");
    jolly.ProcessQueue();
    jolly.Display();

}

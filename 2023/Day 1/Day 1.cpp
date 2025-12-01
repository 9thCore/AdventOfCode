#include <iostream>
#include <fstream>
std::ifstream fin("input.txt");

std::string num[10] = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

void getspelleddigit(std::string s, int i, int &digit) {
    for (int j = 0; j < 10; j++) {
        if (s.compare(i, num[j].size(), num[j]) == 0) {
            digit = j;
            return;
        }
    }
}

int main()
{
    std::string s;
    int sum = 0;

    while (fin >> s) {
        int n1 = -1, n2 = -1;

        for (int i = 0; i < s.size(); i++) {
            char c = s[i];
            int digit = -1;
            if (isdigit(c)) {
                digit = c - '0';
            }

            getspelleddigit(s, i, digit);

            if (digit != -1) {
                if (n1 == -1) { n1 = digit; }
                n2 = digit;
            }
        }

        if (n1 == -1 || n2 == -1) {
            std::cout << "Fatal error! " << s << " has no digits ??? \n";
        }
        else {
            sum += n1 * 10 + n2;
        }
    }

    std::cout << sum;
}
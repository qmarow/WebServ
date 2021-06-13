#include "TransferNumberSystem.hpp"

static std::string mass_simvol = "0123456789abcdef";

int     pow(int number, int pow) {
    if (pow == 0) {
        return (1);
    }

    int result = number;
    
    while (--pow) {
        result *= number;
    }
    return (result);
}

int     transfer_number_system(std::string number) {
    int count_simvols = number.size();
    int revers_count_simvols = count_simvols - 1;
    int result = 0;

    while (--count_simvols > -1) {
        result += (mass_simvol.find(number[count_simvols]) * pow(16, revers_count_simvols - count_simvols));
    }
    return (result);
}

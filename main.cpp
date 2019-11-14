#include "tracksystem.hh"

#include <iostream>


int main()
{
    // Fetch the input
    unsigned trainAmount;
    std::cout << "Enter the amount of trains [1, 5]: ";
    std::cin >> trainAmount;

    if (trainAmount > 5 || trainAmount < 1)
    {
        std::cout << "Invalid amount (whole number between 1 and 5)."
                  << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "\n*** Type stop to exit ***\n" << std::endl;

    TrackSystem trackS(trainAmount);

    // Stop mechanism
    std::string cmd;
    std::cin >> cmd;

    while (cmd != "stop")
    {
        std::cin >> cmd;
    }
}

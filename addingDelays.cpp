#include <iostream>
#include <thread> // for sleep_for
#include <chrono> // for chrono::seconds

int main()
{
  std::cout << "Player attacks..." << std::endl;

  // delay for 1 second
  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::cout << "Critical hit!" << std::endl;
  return 0;
}

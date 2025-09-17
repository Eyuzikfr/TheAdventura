#include <iostream>
#include <random>

int main()
{
  // Create a random device (non-deterministic seed)
  std::random_device rd;

  // Use Mersenne Twister engine with the seed
  std::mt19937 gen(rd());

  // Define the range [1, 100]
  std::uniform_int_distribution<> dist(1, 100);

  // Generate a random number
  int randomNumber = dist(gen);

  std::cout << "Random number: " << randomNumber << std::endl;

  return 0;
}

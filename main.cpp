#include <iostream>
#include <random>
#include <thread>

using namespace std;

const int CRIT_MULTIPLIER = 3;
const int PLAYER_BASE_ATTACK = 20;

// random number generator
random_device rd;
mt19937 gen(rd());

enum PlayerAction
{
  Attack = 1,
  Defend = 2,
  Run = 3
};

class Enemy;

class Player
{
public:
  std::string m_name;
  int m_health;
  int m_attack;
  int m_defense;

  Player()
  {
    m_health = 100;
    m_attack = PLAYER_BASE_ATTACK;
    m_defense = 0;
  }
  void TakeDamage(int damage)
  {
    m_health -= damage;
  }
  void AttackEnemy(class Enemy &enemy, bool isCriticalHit);
};

class Enemy
{
public:
  std::string m_type;
  int m_health;
  int m_attack;

  // constructor to initalize enemy health and attack
  Enemy(const char *type, int health, int attack)
  {
    m_type = type;
    m_health = health;
    m_attack = attack;
  }

  void DealDamage(class Player &player)
  {
    player.m_health -= m_attack;
  }
};

void Player::AttackEnemy(class Enemy &enemy, bool isCriticalHit)
{
  if (isCriticalHit)
    enemy.m_health -= (m_attack * CRIT_MULTIPLIER);
  else
    enemy.m_health -= m_attack;
}

// clear screen function according to platform
void ClearScreen()
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

// display player and enemy hp
void showStats(Player &p, Enemy &e)
{
  cout << p.m_name << "'s HP: " << p.m_health << "   |   "
       << e.m_type << "'s HP: " << e.m_health << endl;
}

void GameIntro(Player &p, int &playerChoice)
{
  // ask for player's name and start the game
  cout << "Hey Adventurer, what's your name?\n -> ";
  cin >> p.m_name;
  cout << "\nGreat name! Alright " << p.m_name << ", are you ready to begin the adventure?" << endl
       << "1. Yes" << endl
       << "2. No" << endl;
  cin >> playerChoice;

  cout << "\nLoading Adventure";
  for (int i = 0; i < 3; i++)
  {
    cout << ".";
    this_thread::sleep_for(chrono::milliseconds(750));
  }
  ClearScreen();

  // if player anything except 1, exit the program
  if (playerChoice != 1)
  {
    cout << "Anytime you're ready, Chief!" << endl;
    exit(0);
  }
}

void Battle(Player &p, Enemy &e)
{
  int pAction;
  cout << "1. Attack" << endl
       << "2. Block Attack" << endl
       << "3. Run" << endl;
  cin >> pAction;
  if (pAction == Run)
  {
    cout << "You chose to run for some reason" << endl;
    for (int i = 0; i < 3; i++)
    {
      cout << ".";
      this_thread::sleep_for(chrono::milliseconds(750));
    }
    return;
  }
  while (p.m_health > 0 && e.m_health > 0)
  {
    bool canBlock = false, choseDefend = false;
    switch (pAction)
    {
    case Attack:
    {
      cout << "Player attacks." << endl;

      // generate a random number for critical hit
      uniform_int_distribution<> dist(1, 10);

      int critHitProb = dist(gen);

      // 1 / 10 chance of a crtitical hit
      if (critHitProb == 7)
      {
        cout << "That's a critical hit, " << p.m_name << ". Awesome!" << endl;
        p.AttackEnemy(e, true);
      }
      else
      {
        cout << "Good hit, " << p.m_name << "." << endl;
        p.AttackEnemy(e, false);
      }

      break;
    }

    case Defend:
    {
      choseDefend = true;
      cout << "Player chooses to block the enemy's attack" << endl;
      // generate a random number for whether player blocks the attack
      uniform_int_distribution<> dist(1, 2);

      int defendProb = dist(gen);

      canBlock = (defendProb == 1);

      break;
    }

    default:
      cout << "Bad choice! Go again!" << endl;
      break;
    }

    for (int i = 0; i < 3; i++)
    {
      cout << ".";
      this_thread::sleep_for(chrono::milliseconds(750));
    }
    cout << "\n\n";

    // if enemy dies, break the while loop
    if (e.m_health <= 0)
    {
      cout << "You defeated the " << e.m_type << "! That was craaaazyyyyy!" << endl;
      this_thread::sleep_for(chrono::seconds(2));

      cout << endl
           << "It dropped a piece of parchment. You pick it up and open it. It reads: " << endl
           << endl;
      this_thread::sleep_for(chrono::seconds(3));
      cout << "\"You have won, adventurer...For now, that is...\n-The Creator of Destruction\"";
      break;
    }

    // enemy's turn to attack
    cout << "The " << e.m_type << " attacks!" << endl;
    if (canBlock)
    {
      cout << "You managed to block the attack, " << p.m_name << "! Sweeeet!" << endl;
    }
    else
    {
      if (choseDefend)
        cout << "You fail to block the attack and take the hit. That's baaaaaddddd!" << endl;
      else
        cout << "You take the hit. Ugh, that's gotta hurt!" << endl;
      e.DealDamage(p);
    }

    for (int i = 0; i < 3; i++)
    {
      cout << ".";
      this_thread::sleep_for(chrono::milliseconds(750));
    }
    cout << endl;
    cout << endl;

    this_thread::sleep_for(chrono::seconds(1));
    ClearScreen();

    showStats(p, e);

    cout << "\nAlright, it's your turn again! Choose what to do:" << endl
         << "1. Attack" << endl
         << "2. Block Attack" << endl;
    cin >> pAction;
  }
}

int main()
{
  // create a player and an enemy
  Player p;
  Enemy bsParasite("Blood Sucking Parasite", 50, 10);

  // holds the playerChoice
  int playerChoice;

  // player action
  int pAction;

  GameIntro(p, playerChoice);

  // adventure begins
  // encounters blood sucking parasite
  showStats(p, bsParasite);
  cout << "\nLet's begin. In front of you stands your first enemy, a " << bsParasite.m_type << ". It has its fangs out. Choose your action:" << endl;

  // battle blood sucking parasite
  Battle(p, bsParasite);

  cout << "\n\nPress Enter to exit...";
  cin.ignore(); // waits for user to press Enter
  cin.get();    // makes sure it pauses
  return 0;
}
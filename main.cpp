#include <iostream>
#include <random>
#include <thread>
#include <vector>

using namespace std;

const int CRIT_MULTIPLIER = 3;
const int PLAYER_BASE_ATTACK = 20;
const int WEAPON_DAMAGE[] = {0, 20, 30};

// random number generator
random_device rd;
mt19937 gen(rd());

enum PlayerAction
{
  Attack = 1,
  Defend = 2,
  ChangeWeapon = 3,
  Run = 4
};

class Enemy;

class Player
{
public:
  std::string m_name;
  int m_health;
  int m_attack;
  int m_defense;
  vector<string> m_weapon_inventory;
  int m_selected_weapon;

  Player()
  {
    m_health = 100;
    m_attack = PLAYER_BASE_ATTACK;
    m_defense = 0;
    m_weapon_inventory.push_back("stick");
    m_selected_weapon = 0;
  }
  void TakeDamage(int damage)
  {
    m_health -= damage;
  }
  void AttackEnemy(class Enemy &enemy, bool isCriticalHit);
  void ShowWeapons()
  {
    int i = 1;
    for (string weapon : m_weapon_inventory)
    {
      cout << i << ". " << weapon << endl;
      i++;
    }
  }
  void Heal()
  {
    m_health = 100;
  }
  void UpdateAttack()
  {
    m_attack = PLAYER_BASE_ATTACK + WEAPON_DAMAGE[m_selected_weapon];
  }
};

class Enemy
{
public:
  std::string m_type;
  int m_health;
  int m_attack;
  string m_drop;

  // constructor to initalize enemy health and attack
  Enemy(const char *type, int health, int attack, string drop)
  {
    m_type = type;
    m_health = health;
    m_attack = attack;
    m_drop = drop;
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

// hold screen function
void HoldScreen(const char *message)
{
  cout << "\n\n"
       << message;
  cin.ignore(); // waits for user to press Enter
  cin.get();    // makes sure it pauses
}

// print loading dots
void LoadingDots()
{
  for (int i = 0; i < 3; i++)
  {
    cout << ".";
    this_thread::sleep_for(chrono::milliseconds(750));
  }
}

// display player and enemy hp
void showStats(Player &p, Enemy &e)
{
  cout << p.m_name << "'s HP: " << p.m_health << "   |   "
       << e.m_type << "'s HP: " << e.m_health << endl;
  cout << "Weapon in hand: " << p.m_weapon_inventory[p.m_selected_weapon] << endl;
}

// switch weapon function
void SwitchWeapon(Player &p)
{
  int weaponChoice;
  ClearScreen();
  cout << "Choose Your Weapon: " << endl;
  p.ShowWeapons();
  cin >> weaponChoice;

  // if player doesn't have the selected weapon, throw an error
  if (weaponChoice < 1 || weaponChoice > p.m_weapon_inventory.size())
  {
    cout << "Oops! Seems like you don't own that weapon yet. Too late!" << endl;
  }
  else
  {
    cout << "Switching weapon";
    LoadingDots();
    p.m_selected_weapon = weaponChoice - 1;
    p.UpdateAttack();
  }
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
  LoadingDots();
  ClearScreen();

  // if player anything except 1, exit the program
  if (playerChoice != 1)
  {
    cout << "Anytime you're ready, Chief!" << endl;
    exit(0);
  }
}

void GameOutro(const char *playerName)
{
  cout << "Thank you, " << playerName << ", for saving us from this chaos. You have been of tremendous help. You came to our world, conquered the evil, and set us free! You must be HIM!" << endl;
}

void Battle(Player &p, Enemy &e)
{
  int pAction;
  cout << "1. Attack" << endl
       << "2. Block Attack" << endl
       << "3. Change Weapon" << endl
       << "4. Run" << endl;
  cin >> pAction;

  if (pAction == Run)
  {
    cout << "You chose to run for some reason" << endl;
    LoadingDots();
    return;
  }

  // main battle loop
  while (p.m_health > 0 && e.m_health > 0)
  {
    bool canBlock = false, choseDefend = false;
    switch (pAction)
    {
    case Attack:
    {
      cout << "You attack it with your " << p.m_weapon_inventory[p.m_selected_weapon] << "." << endl;

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

    case ChangeWeapon:
    {
      SwitchWeapon(p);
      break;
    }

    default:
      cout << "Bad choice! Go again!" << endl;
      break;
    }

    LoadingDots();
    cout << "\n\n";

    // if enemy dies, break the while loop
    if (e.m_health <= 0)
    {
      // to decide if player wants to switch weapon after killing an enemy
      int switchWeaponYN;

      cout << "You defeated the " << e.m_type << "! That was craaaazyyyyy!" << endl;
      LoadingDots();

      p.m_weapon_inventory.push_back(e.m_drop);
      p.Heal();

      cout << "\nCongratulations, you have obtained a " << e.m_drop << ". It has been added to your inventory. And of course, your health has replenished!" << endl;
      cout << "\nWould you like to switch your weapon?" << endl
           << "1. Yes" << endl
           << "2. No" << endl;
      cin >> switchWeaponYN;

      if (switchWeaponYN == 1)
      {
        SwitchWeapon(p);
      }

      HoldScreen("Alright, hit Enter to move to the next challenge!");
      ClearScreen();
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

    LoadingDots();
    cout << endl;
    cout << endl;

    this_thread::sleep_for(chrono::seconds(1));
    ClearScreen();

    showStats(p, e);

    cout << "\nAlright, it's your turn again! Choose what to do:" << endl
         << "1. Attack" << endl
         << "2. Block Attack" << endl
         << "3. Change Weapon" << endl;
    cin >> pAction;
  }
}

int main()
{
  // create a player and an enemy
  Player p;
  Enemy bsParasite("Blood Sucking Parasite", 50, 10, "dagger");
  Enemy beZombie("Brain Eating Zombie", 80, 20, "sword");

  // holds the playerChoice
  int playerChoice;

  GameIntro(p, playerChoice);

  // adventure begins
  // encounters blood sucking parasite
  showStats(p, bsParasite);
  cout << "\nLet's begin. In front of you stands your first enemy, a " << bsParasite.m_type << ". It has its fangs out. Choose your action:" << endl;

  // battle blood sucking parasite
  Battle(p, bsParasite);

  // encounters a brain eating zombie
  showStats(p, beZombie);
  cout << "Oh, look out " << p.m_name << "! It's a Brain Eating Zombie!" << endl;

  // battle brain eating zombie
  Battle(p, beZombie);

  // game outro
  GameOutro(p.m_name);

  HoldScreen("Press Enter key to exit...");
  return 0;
}
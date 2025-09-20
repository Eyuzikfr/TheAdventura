// ✅ TODO: After killing an enemy, the player levels up and the HP increases by 25
// ✅ TODO: Use setter/getter function and make member variables private
// TODO: Enemies drop a weapon and a potion
// TODO: Add to game intro: game rules and mechanics (attack vs block and attack)
// TODO: Add feature: Player misses an attack
#include <iostream>
#include <random>
#include <thread>
#include <vector>

// using namespace std;

const int PLAYER_CRIT_MULTIPLIER = 3;
const int ENEMY_CRIT_MULTIPLIER = 2;
const int PLAYER_BASE_ATTACK = 20;
const int WEAPON_DAMAGE[] = {0, 20, 30};
const int CRIT_CHANCE = 10;
const int DEFEND_CHANCE = 1;

// random number generator
std::random_device rd;
std::mt19937 gen(rd());

// function that returns a random number
int GetRandomNumber(int min, int max)
{
  std::uniform_int_distribution<> dist(min, max);
  return dist(gen);
}

enum PlayerAction
{
  Attack = 1,
  Defend = 2,
  ChangeWeapon = 3,
};

class Enemy;

class Player
{
private:
  std::string m_name;
  int m_health;
  int m_attack;
  int m_defense;
  std::vector<std::string> m_weapon_inventory;
  int m_selected_weapon;

public:
  // constructor
  Player()
  {
    m_health = 100;
    m_attack = PLAYER_BASE_ATTACK;
    m_defense = 0;
    m_weapon_inventory.push_back("stick");
    m_selected_weapon = 0;
  }

public:
  std::string GetPlayerName()
  {
    return m_name;
  }
  void SetPlayerName(std::string name)
  {
    m_name = name;
  }
  int GetPlayerHealth()
  {
    return m_health;
  }
  void DecreasePlayerHealth(int damage)
  {
    m_health -= damage;
  }
  std::string GetSelectedWeapon()
  {
    return m_weapon_inventory[m_selected_weapon];
  }
  void SetSelectedWeapon(int index)
  {
    m_selected_weapon = index;
  }
  int GetInventorySize()
  {
    return m_weapon_inventory.size();
  }
  void AddWeaponToInventory(std::string weapon)
  {
    m_weapon_inventory.push_back(weapon);
  }
  void TakeDamage(int damage)
  {
    m_health -= damage;
  }
  void ShowWeapons()
  {
    int i = 1;
    for (std::string weapon : m_weapon_inventory)
    {
      std::cout << i << ". " << weapon << std::endl;
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
  void UpdatePlayerDefense()
  {
    m_defense += 25;
    m_health += m_defense;
  }
  void AttackEnemy(class Enemy &enemy, bool isCriticalHit);
};

class Enemy
{
private:
  std::string m_type;
  int m_health;
  int m_attack;
  std::string m_drop;
  std::string m_move;
  bool m_isFinalBoss;
  int m_critHitMax; // lower the number, higher the chances of crit hit

public:
  // constructor to initalize enemy health and attack
  Enemy(const char *type, int health, int attack, std::string drop, std::string move, bool isFinalBoss, int critHitMax)
  {
    m_type = type;
    m_health = health;
    m_attack = attack;
    m_drop = drop;
    m_move = move;
    m_isFinalBoss = isFinalBoss;
    m_critHitMax = critHitMax;
  }

public:
  // getter/setters
  std::string GetEnemyType()
  {
    return m_type;
  }
  int GetEnemyHealth()
  {
    return m_health;
  }
  std::string GetEnemyMove()
  {
    return m_move;
  }
  int GetEnemyCritHitMax()
  {
    return m_critHitMax;
  }
  std::string GetEnemyDrop()
  {
    return m_drop;
  }
  bool IsFinalBoss()
  {
    return m_isFinalBoss;
  }
  void DecreaseEnemyHealth(int damage)
  {
    m_health -= damage;
  }
  void DealDamage(class Player &player, bool &isCriticalHit)
  {
    if (isCriticalHit)
    {
      player.DecreasePlayerHealth(m_attack * ENEMY_CRIT_MULTIPLIER);
    }
    else
    {
      player.DecreasePlayerHealth(m_attack);
    }
  }
};

void Player::AttackEnemy(class Enemy &enemy, bool isCriticalHit)
{
  if (isCriticalHit)
    enemy.DecreaseEnemyHealth(m_attack * PLAYER_CRIT_MULTIPLIER);
  else
    enemy.DecreaseEnemyHealth(m_attack);
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
  std::cout << "\n\n"
            << message;
  std::cin.ignore(); // waits for user to press Enter
  std::cin.get();    // makes sure it pauses
}

// print loading dots
void LoadingDots()
{
  for (int i = 0; i < 3; i++)
  {
    std::cout << ".";
    std::this_thread::sleep_for(std::chrono::milliseconds(750));
  }
}

// display player and enemy hp
void showStats(Player &p, Enemy &e)
{
  std::cout << p.GetPlayerName() << "'s HP: " << p.GetPlayerHealth() << "   |   "
            << e.GetEnemyType() << "'s HP: " << e.GetEnemyHealth() << std::endl;
  std::cout << "Weapon in hand: " << p.GetSelectedWeapon() << std::endl
            << std::endl;
}

// switch weapon function
void SwitchWeapon(Player &p)
{
  int weaponChoice;
  ClearScreen();
  std::cout << "Choose Your Weapon: " << std::endl;
  p.ShowWeapons();
  std::cin >> weaponChoice;

  // if player doesn't have the selected weapon, throw an error
  if (weaponChoice < 1 || weaponChoice > p.GetInventorySize())
  {
    std::cout << "Oops! Seems like you don't own that weapon yet. Too late!" << std::endl;
  }
  else
  {
    std::cout << "Switching weapon";
    LoadingDots();
    p.SetSelectedWeapon(weaponChoice - 1);
    p.UpdateAttack();
  }
}

void GameIntro(Player &p, int &playerChoice)
{
  // ask for player's name and start the game
  std::string name;
  std::cout << "Hey Adventurer, what's your name?\n -> ";
  std::cin >> name;
  p.SetPlayerName(name);
  std::cout << "\nGreat name! Alright " << p.GetPlayerName() << ", are you ready to begin the adventure?" << std::endl
            << "1. Yes" << std::endl
            << "2. No" << std::endl;
  std::cin >> playerChoice;

  std::cout << "\nLoading Adventure";
  LoadingDots();
  ClearScreen();

  // if player anything except 1, exit the program
  if (playerChoice != 1)
  {
    std::cout << "Anytime you're ready, Chief!" << std::endl;
    exit(0);
  }
}

void GameOutro(Player &p)
{
  std::cout << "Thank you, " << p.GetPlayerName() << ", for saving us from this chaos. You have been of tremendous help. You came to our world, conquered the evil, and set us free! You must be HIM!" << std::endl;
}

// player's attack turn
void PlayerTurn(Player &p, Enemy &e, bool &canBlock, bool &choseDefend)
{
  int pAction;
  std::cout << "It's your turn to choose a move. Select an action:" << std::endl
            << "1. Attack" << std::endl
            << "2. Block Attack" << std::endl
            << "3. Change Weapon" << std::endl;
  std::cin >> pAction;
  switch (pAction)
  {
  case Attack:
  {
    std::cout << "You attack it with your " << p.GetSelectedWeapon() << "." << std::endl;

    // generate a random number for critical hit
    int critHitProb = GetRandomNumber(1, 10);

    // 1 / 10 chance of a crtitical hit
    if (critHitProb == CRIT_CHANCE)
    {
      std::cout << "That's a critical hit, " << p.GetPlayerName() << ". Awesome!" << std::endl;
      p.AttackEnemy(e, true);
    }
    else
    {
      std::cout << "Good hit, " << p.GetPlayerName() << "." << std::endl;
      p.AttackEnemy(e, false);
    }

    break;
  }

  case Defend:
  {
    choseDefend = true;
    std::cout << "Player chooses to block the enemy's attack." << std::endl;

    // generate a random number for whether player blocks the attack
    int defendProb = GetRandomNumber(1, 2);

    canBlock = (defendProb == DEFEND_CHANCE);

    break;
  }

  case ChangeWeapon:
  {
    SwitchWeapon(p);
    break;
  }

  default:
    std::cout << "Bad choice! Go again!" << std::endl;
    break;
  }
}

// enemy's attack turn
void EnemyTurn(Player &p, Enemy &e, bool &canBlock, bool &choseDefend)
{
  std::cout << "The " << e.GetEnemyType() << " tries to " << e.GetEnemyMove() << "!" << std::endl;
  if (canBlock)
  {
    std::cout << "You managed to block the attack, " << p.GetPlayerName() << "! Sweeeet!" << std::endl;
  }
  else
  {
    int critHitProb = GetRandomNumber(1, e.GetEnemyCritHitMax());
    bool isCriticalHit = (critHitProb == CRIT_CHANCE);
    if (isCriticalHit)
    {
      std::cout << "The enemy has aimed for a critical hit!" << std::endl;
      LoadingDots();
    }
    if (choseDefend)
      std::cout << "You fail to block the attack and take the hit. That's baaaaaddddd!" << std::endl;
    else
      std::cout << "You take the hit. Ugh, that's gotta hurt!" << std::endl;
    e.DealDamage(p, isCriticalHit);
  }
}

// check Player death
bool CheckPlayerDeath(Player &p)
{
  if (p.GetPlayerHealth() <= 0)
  {
    std::cout << p.GetPlayerName() << ", you died";
    LoadingDots();
    std::cout << "\n\nWith your demise, our only hope of survival has been demolished";
    LoadingDots();
    std::cout << "We hope you return soon";
    LoadingDots();
    HoldScreen("Press Enter to exit...");
    return true;
  }
  return false;
}

// check enemy death
bool CheckEnemyDeath(Player &p, Enemy &e)
{
  if (e.GetEnemyHealth() <= 0)
  {
    // to decide if player wants to switch weapon after killing an enemy
    int switchWeaponYN;

    std::cout << "You defeated the " << e.GetEnemyType() << "! That was craaaazyyyyy!" << std::endl;
    LoadingDots();

    p.AddWeaponToInventory(e.GetEnemyDrop());
    p.Heal();
    p.UpdatePlayerDefense();

    // if the defeated enemy is not the final boss, let the player change weapon and move to the next challenge
    if (!e.IsFinalBoss())
    {
      std::cout << "\n\nCongratulations, you have obtained a " << e.GetEnemyDrop() << ". It has been added to your inventory. You have leveled up and your health has replenished!" << std::endl;
      std::cout << "\nWould you like to switch your weapon?" << std::endl
                << "1. Yes" << std::endl
                << "2. No" << std::endl;
      std::cin >> switchWeaponYN;

      if (switchWeaponYN == 1)
      {
        SwitchWeapon(p);
      }

      HoldScreen("Alright, hit Enter to move to the next challenge!");
    }
    else
    {
      std::cout << "\n\n";
      LoadingDots();
      std::cout << "With this, you have defeated the final boss. Crazy combat skills!" << std::endl;
      LoadingDots();
      HoldScreen("Hit Enter to proceed, Champion!");
    }
    ClearScreen();
    return true;
  }
  return false;
}

void Battle(Player &p, Enemy &e)
{

  // main battle loop
  while (p.GetPlayerHealth() > 0 && e.GetEnemyHealth() > 0)
  {
    showStats(p, e);
    bool canBlock = false, choseDefend = false;

    PlayerTurn(p, e, canBlock, choseDefend);

    LoadingDots();
    std::cout << "\n\n";

    // if enemy dies, break the while loop
    bool isEnemyDead = CheckEnemyDeath(p, e);
    if (isEnemyDead)
      break;

    // enemy's turn to attack
    EnemyTurn(p, e, canBlock, choseDefend);

    LoadingDots();
    std::cout << std::endl;
    std::cout << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    ClearScreen();

    // if player dies, leave the game
    bool isPlayerDead = CheckPlayerDeath(p);
    if (isPlayerDead)
      exit(0);
  }
}

int main()
{
  // create a player and an enemy
  Player p;
  Enemy bsParasite("Blood Sucking Parasite", 50, 10, "dagger", "sting you", false, 12);
  Enemy beZombie("Brain Eating Zombie", 100, 20, "sword", "bite you", false, 8);
  Enemy ftDragon("Flame Throwing Dragon", 150, 50, "dragon scales", "burn you", true, 5);

  // holds the playerChoice
  int playerChoice;

  // game intro
  GameIntro(p, playerChoice);

  // adventure begins
  // encounters blood sucking parasite
  std::cout << "Let's begin. In front of you stands your first enemy, a " << bsParasite.GetEnemyType() << ". It has its fangs out.\n\n";
  LoadingDots();
  HoldScreen("Press Enter to battle it!");
  ClearScreen();

  // battle blood sucking parasite
  Battle(p, bsParasite);

  // encounters a brain eating zombie
  std::cout << "Oh, look out " << p.GetPlayerName() << "! It's a Brain Eating Zombie!\n\n";
  LoadingDots();
  HoldScreen("Press Enter to battle it!");
  ClearScreen();

  // battle brain eating zombie
  Battle(p, beZombie);

  // encounters a flame throwing dragon
  std::cout << "It couldn't get worse, or could it? It's a freaking DRAGON! A Flame Throwing Dragon!!\n\n";
  LoadingDots();
  HoldScreen("Press Enter to battle it!");
  ClearScreen();

  // battle the dragon
  Battle(p, ftDragon);

  // game outro
  GameOutro(p);

  HoldScreen("Press Enter key to exit...");
  return 0;
}
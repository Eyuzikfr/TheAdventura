// ✅ TODO: After killing an enemy, the player levels up and the HP increases by 25
// ✅ TODO: Use setter/getter function and make member variables private
// ✅ TODO: Add feature: Player misses an attack
// ✅ TODO: Add feature: Potion drop instead of automatic heal after enemy defeating
// ✅ TODO: Add feature: Attack Booster Potion
// ✅ TODO: Add a branching path: Player can choose to battle or explore a cave after battling the first enemy
// TODO: Add to game intro: game rules and mechanics
// TODO: Clean up the UI and make outro more rewarding
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <cctype>

// using namespace std;

const int PLAYER_CRIT_MULTIPLIER = 3;
const int ENEMY_CRIT_MULTIPLIER = 2;
const int PLAYER_BASE_ATTACK = 20;
const int WEAPON_DAMAGE[] = {0, 20, 30};
const int CRIT_CHANCE = 10;
const int DEFEND_CHANCE = 1;
const int MISS_CHANCE = 1;
const int DEFENSE_INCREMENT = 25;
const int ATTACK_BOOST_MULTIPLIER = 2;

// random number generator
std::random_device rd;
std::mt19937 gen(rd());

// function that returns a random number
int GetRandomNumber(int min, int max)
{
  std::uniform_int_distribution<> dist(min, max);
  return dist(gen);
}

// function to convert string to uppercase
std::string ToUpperCase(std::string text)
{
  for (char &c : text)
  {
    c = std::toupper(static_cast<unsigned char>(c));
  }

  return text;
}

enum PlayerAction
{
  Attack = 1,
  Defend = 2,
  ChangeWeapon = 3,
  ConsumePotion = 4,
};

class Enemy;

class Player
{
private:
  std::string m_name;
  int m_health;
  int m_max_health;
  int m_attack;
  int m_defense;
  std::vector<std::string> m_weapon_inventory;
  std::vector<std::string> m_potion_inventory;
  int m_selected_weapon;
  int m_player_miss_prob_max;
  bool m_is_attack_boosted;

public:
  // constructor
  Player()
  {
    m_health = 100;
    m_max_health = 100;
    m_attack = PLAYER_BASE_ATTACK;
    m_defense = 0;
    m_weapon_inventory.push_back("stick");
    m_selected_weapon = 0;
    m_player_miss_prob_max = 4;
    m_is_attack_boosted = false;
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
  int GetWeaponInventorySize()
  {
    return m_weapon_inventory.size();
  }
  int GetPotionInventorySize()
  {
    return m_potion_inventory.size();
  }
  std::string GetSelectedPotion(int index)
  {
    return m_potion_inventory[index];
  }
  int GetPlayerMisProbMax()
  {
    return m_player_miss_prob_max;
  }
  void AddWeaponToInventory(std::string weapon)
  {
    m_weapon_inventory.push_back(weapon);
  }
  void AddPotionToInventory(std::string potion)
  {
    m_potion_inventory.push_back(potion);
  }
  void RemovePotionFromInventory(int index)
  {
    m_potion_inventory.erase(m_potion_inventory.begin() + index);
  }
  bool GetBoostAttack()
  {
    return m_is_attack_boosted;
  }
  void SetBoostAttack()
  {
    m_is_attack_boosted = true;
  }
  void ResetBoostAttack()
  {
    m_is_attack_boosted = false;
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
      std::cout << i << ". " << ToUpperCase(weapon) << std::endl;
      i++;
    }
  }
  void ShowPotions()
  {
    int i = 1;
    for (std::string potion : m_potion_inventory)
    {
      std::cout << i << ". " << ToUpperCase(potion) << std::endl;
      i++;
    }
  }
  void Heal()
  {
    m_health = m_max_health;
  }
  void UpdateAttack()
  {
    m_attack = PLAYER_BASE_ATTACK + WEAPON_DAMAGE[m_selected_weapon];
  }
  void UpdatePlayerDefense()
  {
    m_defense += DEFENSE_INCREMENT;
    m_health += m_defense;
    m_max_health += m_defense;
    // increase accuracy
    m_player_miss_prob_max += 2;
  }
  void AttackEnemy(class Enemy &enemy, bool isCriticalHit);
};

class Enemy
{
private:
  std::string m_type;
  int m_health;
  int m_attack;
  std::string m_weapon_drop;
  std::string m_potion_drop;
  std::string m_move;
  bool m_isFinalBoss;
  int m_critHitMax; // lower the number, higher the chances of crit hit

public:
  // constructor to initalize enemy health and attack
  Enemy(const char *type, int health, int attack, std::string weaponDrop, std::string potionDrop, std::string move, bool isFinalBoss, int critHitMax)
  {
    m_type = type;
    m_health = health;
    m_attack = attack;
    m_weapon_drop = weaponDrop;
    m_potion_drop = potionDrop;
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
    return m_weapon_drop;
  }
  bool IsFinalBoss()
  {
    return m_isFinalBoss;
  }
  std::string GetEnemyPotionDrop()
  {
    return m_potion_drop;
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
  int damage = m_attack;
  if (m_is_attack_boosted)
    damage *= ATTACK_BOOST_MULTIPLIER;
  if (isCriticalHit)
    damage *= PLAYER_CRIT_MULTIPLIER;
  enemy.DecreaseEnemyHealth(damage);
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
  std::cout << "Weapon in hand: " << ToUpperCase(p.GetSelectedWeapon()) << std::endl
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
  if (weaponChoice < 1 || weaponChoice > p.GetWeaponInventorySize())
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

// use potion function
void UsePotion(Player &p)
{
  int potionChoice;
  ClearScreen();
  std::cout << "Choose a potion to use: " << std::endl;
  p.ShowPotions();
  std::cin >> potionChoice;

  // if player doesn't own the potion, return an error
  if (potionChoice < 1 || potionChoice > p.GetPotionInventorySize())
  {
    std::cout << "Oops! Seems like you don't own that potion yet. Too late!" << std::endl;
  }
  else
  {
    std::string selected_potion = p.GetSelectedPotion(potionChoice - 1);
    std::cout << "You used the " << selected_potion << "." << std::endl;

    if (selected_potion == "heal potion")
    {
      p.Heal();
      std::cout << "\nHealing";
    }
    else if (selected_potion == "attack boost")
    {
      p.SetBoostAttack();
      std::cout << "\nBoosting attack";
    }
    LoadingDots();
    p.RemovePotionFromInventory(potionChoice - 1);
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
            << "3. Change Weapon" << std::endl
            << "4. Use Potion" << std::endl;
  std::cin >> pAction;
  switch (pAction)
  {
  case Attack:
  {
    std::cout << "You attack it with your " << p.GetSelectedWeapon() << "." << std::endl;

    // generate a random number for player miss
    int playerMissProb = GetRandomNumber(1, p.GetPlayerMisProbMax());
    bool playerMisses = (playerMissProb == MISS_CHANCE);

    if (playerMisses)
    {
      std::cout << "Oh no! You missed the attack. Watch out!!!" << std::endl;
      break;
    }

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

  case ConsumePotion:
    UsePotion(p);
    break;

  default:
    std::cout << "Bad choice! Go again!" << std::endl;
    break;
  }
}

// enemy's attack turn
void EnemyTurn(Player &p, Enemy &e, bool &canBlock, bool &choseDefend)
{
  std::cout << "The " << e.GetEnemyType() << " tries to " << e.GetEnemyMove() << " you!" << std::endl;
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
    int weaponOrPotion;

    std::cout << "You defeated the " << e.GetEnemyType() << "! That was craaaazyyyyy!" << std::endl;
    LoadingDots();

    // if attack is boosted, reset it
    if (p.GetBoostAttack())
    {
      p.ResetBoostAttack();
      std::cout << "\n\nThe attack booster is wearing off";
      LoadingDots();
    }

    p.AddWeaponToInventory(e.GetEnemyDrop());
    p.AddPotionToInventory(e.GetEnemyPotionDrop());
    p.UpdatePlayerDefense();

    // if the defeated enemy is not the final boss, let the player change weapon and move to the next challenge
    if (!e.IsFinalBoss())
    {
      std::cout << "\nObtained:" << std::endl
                << ">> Weapon: " << ToUpperCase(e.GetEnemyDrop()) << std::endl
                << ">> Potion: " << ToUpperCase(e.GetEnemyPotionDrop()) << std::endl
                << ">> DEFENSE++" << std::endl
                << ">> ACCURACY++" << std::endl;
      HoldScreen("Press Enter to continue...");
      // std::cout << "\nWould you like to switch your weapon or use a potion?" << std::endl
      //           << "1. Change Weapon" << std::endl
      //           << "2. Use a Potion" << std::endl;
      // std::cin >> weaponOrPotion;

      // if (weaponOrPotion == 1)
      //   SwitchWeapon(p);
      // else if (weaponOrPotion == 2)
      //   UsePotion(p);

      // HoldScreen("Alright, hit Enter to move to the next challenge!");
    }
    else
    {
      ClearScreen();
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

// function for when player is in the cave
void PlayerInCave(Player &p)
{
  bool isExitSelected = false;
  int pAction;
  while (!isExitSelected)
  {
    ClearScreen();
    std::cout << "Choose an action:" << std::endl
              << "1. Change Weapon" << std::endl
              << "2. Use Potion" << std::endl
              << "3. Exit Cave" << std::endl;
    std::cin >> pAction;

    switch (pAction)
    {
    case 1:
      // switch weapon case
      SwitchWeapon(p);
      break;

    case 2:
      // use potion case
      UsePotion(p);
      break;

    case 3:
      // exit cave case
      std::cout << "Exiting Cave";
      LoadingDots();
      isExitSelected = true;
      break;

    default:
      std::cout << "Bad Choice! Try again." << std::endl;
      break;
    }
  }
}

// function for when player is in the dungeon
void PlayerInDungeon(Player &p, bool &exploresDungeon)
{
  bool isExitSelected = false;
  int pAction;
  while (!isExitSelected)
  {
    ClearScreen();
    std::cout << "Choose an action:" << std::endl
              << "1. Change Weapon" << std::endl
              << "2. Use Potion" << std::endl
              << "3. Explore Deeper" << std::endl
              << "4. Exit Dungeon" << std::endl;
    std::cin >> pAction;

    switch (pAction)
    {
    case 1:
      // switch weapon case
      SwitchWeapon(p);
      break;

    case 2:
      // use potion case
      UsePotion(p);
      break;

    case 3:
      // explore further
      std::cout << "Exploring Dungeon";
      LoadingDots();
      isExitSelected = true;
      exploresDungeon = true;
      break;

    case 4:
      // exit dungeon case
      std::cout << "Exiting Dungeon";
      LoadingDots();
      isExitSelected = true;

    default:
      std::cout << "Bad Choice! Try again." << std::endl;
      break;
    }
  }
}

// post battle choice
void PostBattleChoice(Player &p, bool &exploresDungeon)
{
  int playerChoice;
  bool isInvalidChoice = true;
  while (isInvalidChoice)
  {
    std::cout << "The following choices stand in front of you. What do you choose?" << std::endl
              << "1. Enter Dungeon" << std::endl
              << "2. Battle the next enemy" << std::endl;
    std::cin >> playerChoice;

    if (playerChoice == 1)
    {
      std::cout << "Entering Dungeon";
      LoadingDots();
      PlayerInDungeon(p, exploresDungeon);
      isInvalidChoice = false;
    }
    else if (playerChoice == 2)
    {
      isInvalidChoice = false;
    }
    else
    {
      std::cout << "Bad choice! Try again." << std::endl;
      isInvalidChoice = true;
    }
  }

  ClearScreen();
}

int main()
{
  // create a player and an enemy
  Player p;
  Enemy bsParasite("Blood Sucking Parasite", 50, 15, "dagger", "heal potion", "sting", false, 12);
  Enemy beZombie("Brain Eating Zombie", 100, 30, "sword", "attack boost", "bite", false, 8);
  Enemy ftDragon("Flame Throwing Dragon", 150, 50, "dragon scales", "elixir of life", "burn", true, 5);

  // holds the playerChoice
  int playerChoice;

  // whether player explores the dungeon
  bool exploresDungeon = false;

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

  // player chooses to enter a cave, a dungeon or battle the next enemy
  PostBattleChoice(p, exploresDungeon);

  if (!exploresDungeon)
  { // encounters a brain eating zombie
    std::cout << "Oh, look out " << p.GetPlayerName() << "! It's a Brain Eating Zombie!\n\n";
    LoadingDots();
    HoldScreen("Press Enter to battle it!");
    ClearScreen();

    // battle brain eating zombie
    Battle(p, beZombie);

    PostBattleChoice(p, exploresDungeon);

    // encounters a flame throwing dragon
    std::cout << "It couldn't get worse, or could it? It's a freaking DRAGON! A Flame Throwing Dragon!!\n\n";
    LoadingDots();
  }

  if (exploresDungeon)
  {
    std::cout << "You hear something";
    LoadingDots();
    std::cout << "\n\n";
    std::cout << "IS THAT...A DRAGONNNNNN????\n\n";
  }

  HoldScreen("Press Enter to battle it!");
  ClearScreen();
  // battle the dragon
  Battle(p, ftDragon);

  // game outro
  GameOutro(p);

  HoldScreen("Press Enter key to exit...");
  return 0;
}
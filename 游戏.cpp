// #include <iostream>
// #include <conio.h>
// #include <windows.h>
// #include <algorithm>
// #include <ctime>
// #include <fstream>
// #include <string>
// #include <cstring>
// #include <thread>
// #include <vector>
// #include <mutex>
// #include <chrono>
// #include <map>
// #include <functional>
// #include <queue>
// #include <cmath>

// using namespace std;
// using namespace chrono;

// // ==================== 常量定义 ====================
// const int M_SIZE = 50;
// const int E_COUNT = 12;
// const int OBSTACLE_COUNT = 100;
// const int MAX_SAFES = 12;
// const int MAX_PLAYERS = 4;
// const int MAX_LOOTBOXES = 40;
// const int MAX_WAREHOUSE = 30;
// const int MAX_INVENTORY = 50;
// const int MAX_LISTINGS = 20;
// const int MAX_AI_PLAYERS = 8;
// const string PIPE_NAME = "\\\\.\\pipe\\DeltaForceGame";

// // ==================== 颜色定义 ====================
// #define COLOR_RESET "\033[0m"
// #define COLOR_RED "\033[31m"
// #define COLOR_GREEN "\033[32m"
// #define COLOR_YELLOW "\033[33m"
// #define COLOR_BLUE "\033[34m"
// #define COLOR_MAGENTA "\033[35m"
// #define COLOR_CYAN "\033[36m"
// #define COLOR_WHITE "\033[37m"
// #define COLOR_BOLD "\033[1m"
// #define COLOR_ORANGE "\033[38;5;214m"
// #define COLOR_GOLD "\033[38;5;220m"
// #define COLOR_SILVER "\033[38;5;250m"
// #define COLOR_PURPLE "\033[38;5;165m"

// // ==================== 地图类型枚举 ====================
// enum MapType {
//     MAP_ZERO_DAM,
//     MAP_SPACE_BASE
// };

// // ==================== 钥匙卡类型枚举 ====================
// enum KeyCardType {
//     KEY_NONE = 0,
//     KEY_STAFF = 1,
//     KEY_WEST_GATE = 2,
//     KEY_EAST_BRIDGE = 3,
//     KEY_BLACK_ROOM = 4,
//     KEY_A_LEVEL = 5,
//     KEY_SUPPLY_ROOM = 6,
//     KEY_COMMANDER = 7,
//     KEY_LABORATORY = 8
// };

// // ==================== 通行证类型枚举 ====================
// enum PassType {
//     PASS_NONE = 0,
//     PASS_BASIC = 1,
//     PASS_BATTLE = 2,
//     PASS_PREMIUM = 3,
//     PASS_ELITE = 4
// };

// // ==================== 撤离类型枚举 ====================
// enum ExtractionType {
//     EXTRACT_HELICOPTER = 0,
//     EXTRACT_SUBMARINE = 1,
//     EXTRACT_LEVER = 2,
//     EXTRACT_ROCKET = 3,
//     EXTRACT_VEHICLE = 4
// };

// // ==================== AI行为状态枚举 ====================
// enum AIBehaviorState {
//     AI_PATROL = 0,
//     AI_INVESTIGATE = 1,
//     AI_COMBAT = 2,
//     AI_FLEE = 3,
//     AI_HEAL = 4,
//     AI_RELOAD = 5,
//     AI_LOOT = 6
// };

// // ==================== 角色技能效果结构体 ====================
// struct SkillEffect {
//     int damage;
//     int duration;
//     int range;
//     string effect;
// };

// // ==================== 角色结构体 ====================
// struct Character {
//     string name;
//     int maxHp;
//     int baseEnergy;
//     int skill1Cost;
//     int skill2Cost;
//     string skill1Name;
//     string skill2Name;
//     string description;
//     char icon;
//     SkillEffect skill1Effect;
//     SkillEffect skill2Effect;
// };

// // ==================== 14个角色定义 ====================
// Character characters[14] = {
//     {"威龙", 150, 100, 15, 40, "动力推进", "虎蹲炮", "高机动突击，突脸破点，击杀刷新位移", '@',
//      {0, 0, 3, "冲刺"}, {70, 0, 3, "范围伤害"}},
//     {"红狼", 90, 120, 25, 30, "动力外骨骼", "三联装榴弹炮", "快速移动，爆发突击，烟雾掩护", 'W',
//      {0, 45, 0, "伤害提升"}, {30, 0, 5, "三连发"}},
//     {"骇爪", 100, 100, 120, 40, "讯号破解器", "数据飞刀", "信息位大师，扫描定位，静步加速", 'K',
//      {0, 150, 999, "全图扫描"}, {50, 0, 10, "穿透射击"}},
//     {"蜂医", 80, 110, 5, 20, "激素枪", "烟幕无人机", "团队生命线，烟雾治疗双领域", '+',
//      {40, 0, 5, "治疗"}, {0, 40, 3, "持续治疗"}},
//     {"蝶", 85, 105, 30, 50, "治疗烟雾", "复活无人机", "战场医师，群体治疗，战术救援", '&',
//      {0, 60, 3, "范围治疗"}, {80, 0, 5, "复活"}},
//     {"疾风", 85, 130, 15, 25, "战术翻滚", "钻墙电刺", "机动控场，缴械强控，锚点回撤", '>',
//      {0, 15, 4, "隐身"}, {45, 0, 4, "穿透攻击"}},
//     {"牧羊人", 110, 90, 30, 35, "声波陷阱", "防御屏障", "区域封锁，阵地防御，声波探测", 'Y',
//      {30, 35, 3, "强化"}, {0, 30, 4, "缴械"}},
//     {"蛊", 95, 100, 35, 50, "肾上腺素", "致盲烟雾", "团队强化，血上限压制，战术冲锋", 'G',
//      {30, 35, 0, "强化"}, {0, 50, 4, "致盲"}},
//     {"比特", 100, 115, 120, 35, "寻猎蜘蛛", "哨兵母巢", "巡猎追踪，腐蚀标记，战场分隔", 'H',
//      {70, 0, 3, "护盾"}, {45, 40, 3, "持续伤害"}},
//     {"深蓝", 120, 95, 20, 30, "持盾", "燃烧弹/地刺", "盾卫先锋，绳索牵引，区域火控", 'D',
//      {0, 80, 0, "标记"}, {45, 0, 3, "范围攻击"}},
//     {"露娜", 90, 140, 60, 30, "探测箭矢", "电击箭矢", "信息狙击，猎人标记，远程侦察", 'L',
//      {0, 50, 999, "隐身"}, {40, 0, 10, "精准射击"}},
//     {"无名", 100, 100, 40, 60, "隐身", "狂暴", "全能平衡，适应多种战术", '?',
//      {0, 50, 0, "隐身"}, {0, 60, 0, "狂暴"}},
//     {"乌鲁鲁", 130, 80, 50, 70, "战吼", "狂战", "狂战士，高伤低防，极限输出", 'U',
//      {0, 45, 0, "伤害提升"}, {0, 50, 0, "狂暴"}},
//     {"银翼", 95, 110, 35, 55, "精准射击", "多重射击", "精准打击，多段伤害，远程压制", 'A',
//      {0, 40, 0, "精准"}, {35, 0, 8, "三连发"}}
// };

// // ==================== Item结构体 ====================
// struct Item {
//     string name;
//     int price;
//     int rarity;
//     string type;
//     string description;
//     int capacity;  // 容量（背包/胸挂格子数）
// };

// // ==================== 钥匙卡结构体 ====================
// struct KeyCard {
//     KeyCardType type;
//     string name;
//     string description;
//     int uses;
//     bool isHacked;
//     int code[4];
    
//     string getColor() const {
//         switch(type) {
//             case KEY_STAFF: return COLOR_GREEN;
//             case KEY_WEST_GATE: return COLOR_BLUE;
//             case KEY_EAST_BRIDGE: return COLOR_CYAN;
//             case KEY_BLACK_ROOM: return COLOR_MAGENTA;
//             case KEY_A_LEVEL: return COLOR_ORANGE;
//             case KEY_SUPPLY_ROOM: return COLOR_YELLOW;
//             case KEY_COMMANDER: return COLOR_RED;
//             case KEY_LABORATORY: return COLOR_GOLD;
//             default: return COLOR_WHITE;
//         }
//     }
// };

// // ==================== 钥匙卡定义 ====================
// const KeyCard KEY_CARDS[] = {
//     {KEY_NONE, "无", "无权限", 0, false, {0,0,0,0}},
//     {KEY_STAFF, "员工通道卡", "可开启一号、二号员工通道，外围进核心的安全路线", 3, false, {0,0,0,0}},
//     {KEY_WEST_GATE, "西区大门卡", "可开启西区大门，西大出生点专用路线", 3, false, {0,0,0,0}},
//     {KEY_EAST_BRIDGE, "东区吊桥卡", "可开启东区吊桥，牢三出生点进核心路线", 3, false, {0,0,0,0}},
//     {KEY_BLACK_ROOM, "黑室密钥", "随机密码门，每局密码不同，需听摩斯密码破解", 1, false, {0,0,0,0}},
//     {KEY_A_LEVEL, "A级工卡", "开启西区码头顶层总裁办公室，哈德森护卫掉落", 2, false, {0,0,0,0}},
//     {KEY_SUPPLY_ROOM, "物资室权限", "开启浮力室/黑室物资室铁门", 2, false, {0,0,0,0}},
//     {KEY_COMMANDER, "指挥官密钥", "开启指挥中心高级保险箱", 1, false, {0,0,0,0}},
//     {KEY_LABORATORY, "轨道实验室密钥", "开启轨道实验室", 1, false, {0,0,0,0}}
// };

// const int KEY_CARDS_COUNT = sizeof(KEY_CARDS) / sizeof(KEY_CARDS[0]);

// // ==================== 钥匙卡门结构体 ====================
// struct KeyCardDoor {
//     int id;
//     int x, y;
//     KeyCardType requiredCard;
//     string doorName;
//     string description;
//     bool isLocked;
//     bool isOpen;
//     int targetX, targetY;
//     int safeCount;
//     int lootBoxCount;
// };

// // ==================== 钥匙卡门定义 ====================
// const KeyCardDoor KEY_DOORS[] = {
//     {1, 12, 40, KEY_STAFF, "员工通道", "通往核心区的一号通道", true, false, 20, 25, 2, 3},
//     {2, 8, 38, KEY_WEST_GATE, "西区大门", "西区主要入口", true, false, 15, 20, 1, 2},
//     {3, 42, 40, KEY_EAST_BRIDGE, "东区吊桥", "连接东区的吊桥", true, false, 35, 30, 2, 2},
//     {4, 23, 18, KEY_BLACK_ROOM, "黑室密码门", "需要输入4位密码", true, false, 23, 19, 3, 4},
//     {5, 25, 22, KEY_A_LEVEL, "总裁办公室", "西区码头顶层办公室", true, false, 25, 23, 4, 5},
//     {6, 17, 26, KEY_SUPPLY_ROOM, "浮力室物资室", "存放高级物资的房间", true, false, 17, 27, 2, 3},
//     {7, 23, 19, KEY_SUPPLY_ROOM, "黑室物资室", "存放高级物资的房间", true, false, 23, 20, 2, 3},
//     {8, 31, 17, KEY_COMMANDER, "指挥中心", "离心机室指挥中心", true, false, 31, 18, 3, 4},
//     {9, 45, 45, KEY_LABORATORY, "轨道实验室", "高级实验室区域", true, false, 46, 46, 4, 5}
// };

// const int KEY_DOORS_COUNT = sizeof(KEY_DOORS) / sizeof(KEY_DOORS[0]);

// // ==================== 通行证结构体 ====================
// struct BattlePass {
//     PassType type;
//     string name;
//     int level;
//     int exp;
//     vector<int> claimedLevels;
    
//     string getColor() const {
//         switch(type) {
//             case PASS_BASIC: return COLOR_SILVER;
//             case PASS_BATTLE: return COLOR_GREEN;
//             case PASS_PREMIUM: return COLOR_BLUE;
//             case PASS_ELITE: return COLOR_GOLD;
//             default: return COLOR_WHITE;
//         }
//     }
    
//     string getName() const {
//         switch(type) {
//             case PASS_BASIC: return "基础通行证";
//             case PASS_BATTLE: return "战斗通行证";
//             case PASS_PREMIUM: return "高级通行证";
//             case PASS_ELITE: return "精英通行证";
//             default: return "无通行证";
//         }
//     }
// };

// // ==================== 通行证区域结构体 ====================
// struct PassZone {
//     int id;
//     int x, y;
//     string name;
//     string description;
//     PassType requiredPass;
//     int requiredLevel;
//     bool isLocked;
//     int targetX, targetY;
//     int safeCount;
//     int lootBoxCount;
// };

// // ==================== 通行证区域定义 ====================
// const PassZone PASS_ZONES[] = {
//     {1, 10, 10, "基础通行证区", "基础通行证可进入的区域", PASS_BASIC, 1, true, 11, 11, 1, 2},
//     {2, 20, 20, "战斗通行证区", "战斗通行证可进入的区域", PASS_BATTLE, 5, true, 21, 21, 2, 3},
//     {3, 30, 30, "高级通行证区", "高级通行证可进入的区域", PASS_PREMIUM, 10, true, 31, 31, 3, 4},
//     {4, 40, 40, "精英通行证区", "精英通行证可进入的区域", PASS_ELITE, 15, true, 41, 41, 4, 5},
//     {5, 50, 50, "通行证秘密区域", "需要高级通行证和等级20", PASS_PREMIUM, 20, true, 51, 51, 5, 6}
// };

// const int PASS_ZONES_COUNT = sizeof(PASS_ZONES) / sizeof(PASS_ZONES[0]);

// // ==================== 撤离点结构体 ====================
// struct ExtractionPoint {
//     int x, y;
//     ExtractionType type;
//     string name;
//     string description;
//     bool isActive;
//     bool requiresInteraction;
//     int timer;
//     KeyCardType requiredCard;
//     PassType requiredPass;
//     int requiredPassLevel;
// };

// // ==================== 撤离点定义 ====================
// const ExtractionPoint EXTRACTION_POINTS[] = {
//     {45, 45, EXTRACT_HELICOPTER, "长弓溪谷", "直升机撤离点，停留5秒召唤直升机", true, false, 0, KEY_NONE, PASS_NONE, 0},
//     {5, 5, EXTRACT_SUBMARINE, "水下管道", "秘密水下撤离通道", true, false, 0, KEY_NONE, PASS_NONE, 0},
//     {22, 12, EXTRACT_LEVER, "行政楼拉闸撤离", "拉下电闸后到东楼停车场等待", false, true, 180, KEY_NONE, PASS_NONE, 0},
//     {45, 5, EXTRACT_ROCKET, "火箭发射区", "乘坐火箭撤离，需完成飞升者行动", false, true, 60, KEY_COMMANDER, PASS_ELITE, 10},
//     {30, 20, EXTRACT_HELICOPTER, "直升机平台", "拉下两个电闸后激活", false, true, 300, KEY_NONE, PASS_BATTLE, 5},
//     {20, 15, EXTRACT_VEHICLE, "载具撤离点", "乘坐装甲车撤离", true, false, 0, KEY_NONE, PASS_BASIC, 1}
// };

// const int EXTRACTION_POINTS_COUNT = sizeof(EXTRACTION_POINTS) / sizeof(EXTRACTION_POINTS[0]);

// // ==================== AI装备结构体 ====================
// struct AIEquipment {
//     struct Weapon {
//         string name;
//         int damage;
//         int ammo;
//         int maxAmmo;
//         int range;
//         string type;
//     } primaryWeapon, secondaryWeapon;
    
//     struct Armor {
//         string name;
//         int defense;
//         int durability;
//         string slot;
//     } vest, helmet;
    
//     struct Tactical {
//         string name;
//         int count;
//         string type;
//     } grenades[3];
    
//     int medkits;
//     int stimulants;
//     int bandages;
// };

// // ==================== AI玩家结构体 ====================
// struct AIPlayer {
//     int id;
//     string name;
//     int characterId;
//     int x, y;
//     int targetX, targetY;
//     int hp;
//     int maxHp;
//     int energy;
//     int shield;
//     bool isAlive;
    
//     AIBehaviorState state;
//     AIEquipment equipment;
    
//     int skill1Cooldown;
//     int skill2Cooldown;
    
//     int detectionRange;
//     int hearingRange;
//     int lastSeenX, lastSeenY;
//     int investigationTimer;
    
//     vector<pair<int, int>> patrolPath;
//     int currentPathIndex;
    
//     int accuracy;
//     int aggression;
//     int caution;
    
//     int money;
//     vector<Item> loot;
// };

// // ==================== Unit结构体 ====================
// struct Unit {
//     int x, y;
//     bool live;
//     int hp;
// };

// // ==================== 保险箱结构体 ====================
// struct Safe {
//     int x, y;
//     bool locked;
//     KeyCardType requiredCard;
//     PassType requiredPass;
//     int requiredPassLevel;
//     int code[4];
//     int currentInput[4];
//     int inputPos;
//     Item reward;
//     bool opened;
// };

// // ==================== 战利品箱结构体 ====================
// struct LootBox {
//     int x, y;
//     bool active;
//     Item items[5];
//     int itemCount;
//     string ownerName;
// };

// // ==================== 玩家数据结构体 ====================
// struct PlayerData {
//     int money;
//     Item warehouse[MAX_WAREHOUSE];      // 仓库（基地存储）
//     int warehouseCount;
//     Item inventory[MAX_INVENTORY];       // 背包（局内携带）
//     int inventoryCount;
//     Item equippedWeapon;                 // 装备的武器
//     Item equippedArmor;                  // 装备的护甲
//     Item equippedChestRig;               // 装备的胸挂
//     Item equippedBackpack;               // 装备的背包
//     int backpackSize;                    // 背包容量（基础+背包配件加成）
//     bool hasBackpack;
//     Item safeBox;
//     bool hasSafeBoxItem;
//     vector<KeyCard> keyCards;
//     BattlePass battlePass;
// };

// // ==================== 全局变量 ====================
// PlayerData playerData = {
//     10000,                                              // money
//     {}, 0,                                              // warehouse, warehouseCount
//     {}, 0,                                              // inventory, inventoryCount
//     {"", 0, 0, "", "", 0},                            // equippedWeapon (空)
//     {"", 0, 0, "", "", 0},                            // equippedArmor (空)
//     {"", 0, 0, "", "", 0},                            // equippedChestRig (空)
//     {"", 0, 0, "", "", 0},                            // equippedBackpack (空)
//     10,                                                 // backpackSize (基础容量10)
//     false,                                              // hasBackpack
//     {"", 0, 0, "", "", 0}, false,                     // safeBox, hasSafeBoxItem
//     {},                                                 // keyCards
//     {PASS_NONE, "无通行证", 1, 0, {}}                  // battlePass
// };
// int g_selectedCharacter = 0;
// MapType currentMap = MAP_ZERO_DAM;
// bool g_isMultiplayerMode = false;

// // ==================== 联机模式结构体 ====================
// struct RoomInfo {
//     int roomId;
//     string roomName;
//     string hostName;
//     int hostId;
//     int playerCount;
//     int maxPlayers;
//     int players[4];
//     bool inGame;
//     int mapType;
//     int difficulty;
//     bool hasPassword;
//     long long createTime;
// };

// struct PlayerInfo {
//     int playerId;
//     string playerName;
//     int characterId;
//     bool isReady;
//     bool isHost;
//     int x, y;
//     int hp;
//     int maxHp;
//     int energy;
//     bool isAlive;
// };

// // ==================== 局域网联机管理器 ====================
// class LANGameManager {
// public:
//     bool isHost;
//     bool isConnected;
//     vector<PlayerInfo> players;
//     RoomInfo currentRoom;
//     int myPlayerId;
//     string myPlayerName;
//     int myCharacterId;
//     bool lobbyActive;
    
//     HANDLE hPipe;
//     thread* receiveThread;
//     mutex dataMutex;
    
//     LANGameManager() {
//         isHost = false;
//         isConnected = false;
//         myPlayerId = 0;
//         myPlayerName = "战士" + to_string(rand() % 1000);
//         myCharacterId = 0;
//         hPipe = INVALID_HANDLE_VALUE;
//         receiveThread = nullptr;
//         lobbyActive = true;
//     }
    
//     ~LANGameManager() {
//         disconnect();
//     }
    
//     bool createRoom(string roomName, int maxPlayers, int mapType, int difficulty) {
//         hPipe = CreateNamedPipeA(
//             PIPE_NAME.c_str(),
//             PIPE_ACCESS_DUPLEX,
//             PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
//             PIPE_UNLIMITED_INSTANCES,
//             4096, 4096,
//             0,
//             NULL
//         );
        
//         if (hPipe == INVALID_HANDLE_VALUE) return false;
        
//         isHost = true;
//         isConnected = true;
//         myPlayerId = 0;
        
//         PlayerInfo host;
//         host.playerId = 0;
//         host.playerName = myPlayerName;
//         host.characterId = myCharacterId;
//         host.isReady = false;
//         host.isHost = true;
//         host.x = 25;
//         host.y = 45;
//         host.hp = 100;
//         host.maxHp = 100;
//         host.energy = 100;
//         host.isAlive = true;
//         players.push_back(host);
        
//         currentRoom.roomName = roomName;
//         currentRoom.hostName = myPlayerName;
//         currentRoom.playerCount = 1;
//         currentRoom.maxPlayers = maxPlayers;
//         currentRoom.mapType = mapType;
//         currentRoom.difficulty = difficulty;
        
//         receiveThread = new thread(&LANGameManager::hostReceiveLoop, this);
        
//         return true;
//     }
    
//     void hostReceiveLoop() {
//         while (isHost && isConnected) {
//             BOOL connected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
            
//             if (connected) {
//                 char buffer[1024];
//                 DWORD bytesRead;
//                 BOOL result = ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
                
//                 if (result && bytesRead > 0) {
//                     buffer[bytesRead] = '\0';
//                     processClientMessage(string(buffer));
//                 }
                
//                 DisconnectNamedPipe(hPipe);
//             }
//             Sleep(100);
//         }
//     }
    
//     void processClientMessage(const string& data) {
//         lock_guard<mutex> lock(dataMutex);
        
//         if (data.find("JOIN:") == 0) {
//             string playerName = data.substr(5);
//             int newId = players.size();
            
//             PlayerInfo newPlayer;
//             newPlayer.playerId = newId;
//             newPlayer.playerName = playerName;
//             newPlayer.characterId = 0;
//             newPlayer.isReady = false;
//             newPlayer.isHost = false;
//             newPlayer.x = 25 + rand() % 10;
//             newPlayer.y = 45 + rand() % 10;
//             newPlayer.hp = 100;
//             newPlayer.maxHp = 100;
//             newPlayer.energy = 100;
//             newPlayer.isAlive = true;
//             players.push_back(newPlayer);
            
//             currentRoom.playerCount = players.size();
//         }
//         else if (data.find("READY:") == 0) {
//             int playerId = atoi(data.substr(6).c_str());
//             for (auto& player : players) {
//                 if (player.playerId == playerId) {
//                     player.isReady = !player.isReady;
//                     break;
//                 }
//             }
//         }
//     }
    
//     bool joinRoom() {
//         hPipe = CreateFileA(
//             PIPE_NAME.c_str(),
//             GENERIC_READ | GENERIC_WRITE,
//             0,
//             NULL,
//             OPEN_EXISTING,
//             0,
//             NULL
//         );
        
//         if (hPipe == INVALID_HANDLE_VALUE) return false;
        
//         isHost = false;
//         isConnected = true;
//         myPlayerId = 1;
        
//         string joinMsg = "JOIN:" + myPlayerName;
//         DWORD bytesWritten;
//         WriteFile(hPipe, joinMsg.c_str(), joinMsg.length(), &bytesWritten, NULL);
        
//         PlayerInfo myInfo;
//         myInfo.playerId = 1;
//         myInfo.playerName = myPlayerName;
//         myInfo.characterId = myCharacterId;
//         myInfo.isReady = false;
//         myInfo.isHost = false;
//         myInfo.x = 25;
//         myInfo.y = 45;
//         myInfo.hp = 100;
//         myInfo.maxHp = 100;
//         myInfo.energy = 100;
//         myInfo.isAlive = true;
//         players.push_back(myInfo);
        
//         return true;
//     }
    
//     void toggleReady() {
//         lock_guard<mutex> lock(dataMutex);
//         for (auto& player : players) {
//             if (player.playerId == myPlayerId) {
//                 player.isReady = !player.isReady;
//                 break;
//             }
//         }
        
//         if (!isHost && isConnected && hPipe != INVALID_HANDLE_VALUE) {
//             string readyMsg = "READY:" + to_string(myPlayerId);
//             DWORD bytesWritten;
//             WriteFile(hPipe, readyMsg.c_str(), readyMsg.length(), &bytesWritten, NULL);
//         }
//     }
    
//     void disconnect() {
//         isConnected = false;
        
//         if (receiveThread && receiveThread->joinable()) {
//             receiveThread->join();
//             delete receiveThread;
//             receiveThread = nullptr;
//         }
        
//         if (hPipe != INVALID_HANDLE_VALUE) {
//             CloseHandle(hPipe);
//             hPipe = INVALID_HANDLE_VALUE;
//         }
        
//         players.clear();
//         isHost = false;
//     }
// };

// // ==================== 全局联机变量 ====================
// LANGameManager* g_lanGame = nullptr;

// // ==================== 函数声明 ====================
// int showCharacterSelection();
// void showWarehouse();
// void showEquipment();
// void showInGameInventory();
// void storeItemInSafe();
// void showInventory();
// void showChestRigMarket();
// void showBackpackMarket();
// void showMarket();
// void showTradingPost();
// void showInstructions();
// void showSecretService();
// void showPreBattlePreparation(MapType map);
// void saveGame();
// void loadGame();
// void showMainMenu();
// void showMultiplayerMenu();
// void showCreateRoomMenu();
// void showLobby();
// void showBattlePass();
// void showKeyCards();
// void showPassZones();
// void showDeltaForceTitle();

// // ==================== 商品数据定义（完整版）====================
// // 三角洲行动变卖物定义
// const Item VALUABLE_ITEMS[] = {
//     {"战场情报终端", 3200, 0, "material", "高价值情报终端", 0},
//     {"指挥模块", 2800, 0, "material", "战术指挥核心模块", 0},
//     {"加密数据链", 2500, 0, "material", "军用加密数据传输链", 0},
//     {"战略绘图仪", 2200, 0, "material", "战场战略绘图设备", 0},
//     {"量子加密器", 2000, 0, "material", "量子加密通信设备", 0},
//     {"黄金怀表", 1800, 0, "material", "军官定制黄金怀表", 0},
//     {"古董罗盘", 1600, 0, "material", "百年历史的航海罗盘", 0},
//     {"翡翠印章", 1500, 0, "material", "翡翠雕刻的军官印章", 0},
//     {"钻石胸针", 1400, 0, "material", "镶嵌钻石的军官胸针", 0},
//     {"红宝石项链", 1300, 0, "material", "红宝石军官项链", 0},
//     {"蓝宝石戒指", 1250, 0, "material", "蓝宝石军官戒指", 0},
//     {"象牙雕像", 1200, 0, "material", "手工雕刻象牙艺术品", 0},
//     {"青铜爵杯", 1150, 0, "material", "古代青铜爵杯", 0},
//     {"战术平板", 1100, 0, "material", "军用战术平板电脑", 0},
//     {"军用GPS", 1000, 0, "material", "高精度军用定位仪", 0},
//     {"加密电台", 950, 0, "material", "军用加密通信电台", 0},
//     {"热成像仪", 900, 0, "material", "单兵热成像设备", 0},
//     {"夜视仪", 850, 0, "material", "四代微光夜视仪", 0},
//     {"激光测距仪", 800, 0, "material", "军用激光测距设备", 0},
//     {"弹道计算机", 750, 0, "material", "火控弹道计算机", 0},
//     {"金条", 700, 0, "material", "500g纯金金条", 0},
//     {"银条", 600, 0, "material", "1000g纯银银条", 0},
//     {"钛合金", 550, 0, "material", "航天级钛合金材料", 0},
//     {"碳纤维", 500, 0, "material", "高模量碳纤维布", 0},
//     {"特种陶瓷", 480, 0, "material", "防弹特种陶瓷板", 0},
//     {"现金", 400, 0, "material", "成捆的现金", 0},
//     {"珠宝", 350, 0, "material", "混合珠宝首饰", 0},
//     {"手表", 300, 0, "material", "高档机械手表", 0},
//     {"金戒指", 280, 0, "material", "纯金戒指", 0},
//     {"银项链", 250, 0, "material", "纯银项链", 0},
//     {"古钱币", 200, 0, "material", "稀有古钱币", 0},
//     {"机密文件", 1100, 0, "material", "军事机密文件", 0},
//     {"服务器硬盘", 950, 0, "material", "加密服务器硬盘", 0},
//     {"电路板", 450, 0, "material", "军用电路板", 0},
//     {"合金零件", 380, 0, "material", "精密合金零件", 0},
//     {"电子元件", 320, 0, "material", "军用电子元件", 0},
//     {"行政楼密钥", 2000, 0, "material", "行政楼金库密钥", 0},
//     {"金库密码本", 2500, 0, "material", "金库保险密码本", 0},
//     {"水泥厂设计图", 1800, 0, "material", "水泥厂结构图", 0},
//     {"变电站核心", 2200, 0, "material", "变电站核心部件", 0},
//     {"双首领信物", 5000, 0, "material", "击败双首领的信物", 0},
//     {"物流凭证", 3500, 0, "material", "战略物资物流凭证", 0},
//     {"哈夫克战车零件", 3000, 0, "material", "哈夫克战车残骸", 0},
//     {"完好的A级工卡", 2800, 0, "material", "哈德森掉落的工卡", 0},
//     {"密涅瓦程序", 4500, 0, "material", "航天核心程序", 0},
//     {"火箭燃料样本", 3800, 0, "material", "航天火箭燃料", 0},
//     {"返回舱碎片", 2500, 0, "material", "返回舱残骸", 0},
//     {"防空导弹残骸", 3000, 0, "material", "防空导弹部件", 0},
//     {"航天服原型", 3500, 0, "armor", "航天服原型机", 0}
// };

// const int VALUABLE_ITEMS_COUNT = sizeof(VALUABLE_ITEMS) / sizeof(VALUABLE_ITEMS[0]);

// // 保险箱奖励定义
// const Item SAFE_REWARDS[] = {
//     {"M4A1", 2500, 0, "weapon", "全自动突击步枪", 0},
//     {"SCAR-H", 2300, 0, "weapon", "重型战斗步枪", 0},
//     {"AWM", 2200, 0, "weapon", "栓动狙击步枪", 0},
//     {"Vector", 2000, 0, "weapon", "冲锋枪", 0},
//     {"MP7", 1900, 0, "weapon", "个人防卫武器", 0},
//     {"六级防弹衣", 2000, 0, "armor", "最高防护等级防弹衣", 0},
//     {"六级头盔", 1800, 0, "armor", "最高防护等级头盔", 0},
//     {"五级防弹衣", 1600, 0, "armor", "高级防弹衣", 0},
//     {"五级头盔", 1500, 0, "armor", "高级头盔", 0},
//     {"军用急救箱", 1200, 0, "med", "大型军用急救包", 0},
//     {"战术医疗包", 1000, 0, "med", "战术医疗套件", 0},
//     {"肾上腺素", 900, 0, "med", "紧急战斗兴奋剂", 0},
//     {"止血带", 800, 0, "med", "战术止血带", 0},
//     {"热成像瞄具", 1100, 0, "equipment", "热成像瞄准镜", 0},
//     {"消音器", 950, 0, "equipment", "枪械消音器", 0},
//     {"垂直握把", 850, 0, "equipment", "垂直前握把", 0},
//     {"全息瞄具", 750, 0, "equipment", "全息衍射瞄准镜", 0},
//     {"特制弹药箱", 600, 0, "ammo", "特制穿甲弹药箱", 0},
//     {"狙击弹药箱", 550, 0, "ammo", "狙击步枪弹药箱", 0},
//     {"步枪弹药箱", 500, 0, "ammo", "突击步枪弹药箱", 0},
//     {"冲锋枪弹药箱", 450, 0, "ammo", "冲锋枪弹药箱", 0}
// };

// const int SAFE_REWARDS_COUNT = sizeof(SAFE_REWARDS) / sizeof(SAFE_REWARDS[0]);

// // 航天基地保险箱奖励
// const Item SPACE_BASE_REWARDS[] = {
//     {"G18", 2800, 0, "weapon", "全自动手枪", 0},
//     {"M249", 3200, 0, "weapon", "轻机枪", 0},
//     {"M14", 2600, 0, "weapon", "精准射手步枪", 0},
//     {"P90", 2400, 0, "weapon", "个人防卫武器", 0},
//     {"UZI", 2100, 0, "weapon", "微型冲锋枪", 0},
//     {"空间站核心模块", 4500, 0, "material", "空间站核心部件", 0},
//     {"火箭燃料样本", 3800, 0, "material", "航天火箭燃料", 0},
//     {"航天服原型", 3500, 0, "armor", "航天服原型机", 0},
//     {"导航计算机", 3200, 0, "equipment", "航天导航计算机", 0},
//     {"轨道数据终端", 4000, 0, "material", "轨道数据记录仪", 0},
//     {"发射井密钥", 3600, 0, "material", "火箭发射井密钥", 0},
//     {"宇宙辐射计", 2900, 0, "equipment", "宇宙辐射测量仪", 0},
//     {"推进器组件", 3100, 0, "material", "火箭推进器部件", 0},
//     {"生命维持系统", 3400, 0, "equipment", "航天生命维持系统", 0},
//     {"黑匣子记录仪", 2800, 0, "material", "火箭黑匣子", 0},
//     {"航天级钛合金", 2700, 0, "armor", "航天钛合金板", 0},
//     {"隔热瓦", 2300, 0, "armor", "航天隔热陶瓷瓦", 0},
//     {"陀螺仪", 2500, 0, "equipment", "高精度陀螺仪", 0},
//     {"燃料舱蓝图", 3000, 0, "material", "火箭燃料舱图纸", 0},
//     {"指挥官密钥", 4200, 0, "material", "基地指挥官密钥", 0}
// };

// const int SPACE_BASE_REWARDS_COUNT = sizeof(SPACE_BASE_REWARDS) / sizeof(SPACE_BASE_REWARDS[0]);

// // 武器商城
// const Item WEAPON_ITEMS[] = {
//     {"M4A1突击步枪", 1200, 1, "weapon", "全自动，稳定性高", 0},
//     {"SCAR-H战斗步枪", 1500, 2, "weapon", "7.62mm，威力大", 0},
//     {"AK-74M突击步枪", 1100, 1, "weapon", "5.45mm，性价比高", 0},
//     {"HK416突击步枪", 1600, 2, "weapon", "高精度，高射速", 0},
//     {"AWM狙击步枪", 2500, 3, "weapon", "栓动，一枪致命", 0},
//     {"M700狙击步枪", 1800, 2, "weapon", "轻便，精准", 0},
//     {"Vector冲锋枪", 1300, 2, "weapon", ".45ACP，极高射速", 0},
//     {"MP5冲锋枪", 900, 1, "weapon", "9mm，稳定易控", 0},
//     {"G18手枪", 500, 0, "weapon", "9mm，全自动手枪", 0},
//     {"沙漠之鹰", 800, 1, "weapon", ".50AE，威力大", 0},
//     {"M1014霰弹枪", 1000, 1, "weapon", "半自动，射速快", 0},
//     {"M249轻机枪", 2200, 3, "weapon", "5.56mm，100发弹链", 0}
// };

// const int WEAPON_ITEMS_COUNT = sizeof(WEAPON_ITEMS) / sizeof(WEAPON_ITEMS[0]);

// // 护甲商城
// const Item ARMOR_ITEMS[] = {
//     {"一级防弹衣", 300, 0, "armor", "护甲值30", 0},
//     {"二级防弹衣", 500, 0, "armor", "护甲值50", 0},
//     {"三级防弹衣", 800, 1, "armor", "护甲值70", 0},
//     {"四级防弹衣", 1200, 1, "armor", "护甲值90", 0},
//     {"五级防弹衣", 1800, 2, "armor", "护甲值110", 0},
//     {"六级防弹衣", 2500, 3, "armor", "护甲值150", 0},
//     {"一级头盔", 200, 0, "armor", "护甲值20", 0},
//     {"二级头盔", 350, 0, "armor", "护甲值30", 0},
//     {"三级头盔", 600, 1, "armor", "护甲值40", 0},
//     {"四级头盔", 900, 1, "armor", "护甲值50", 0},
//     {"五级头盔", 1400, 2, "armor", "护甲值65", 0},
//     {"六级头盔", 2000, 3, "armor", "护甲值80", 0}
// };

// const int ARMOR_ITEMS_COUNT = sizeof(ARMOR_ITEMS) / sizeof(ARMOR_ITEMS[0]);

// // 医疗物资
// const Item MEDICAL_ITEMS[] = {
//     {"绷带", 50, 0, "med", "恢复20生命值", 0},
//     {"止血带", 120, 0, "med", "恢复40生命值", 0},
//     {"医疗包", 200, 0, "med", "恢复60生命值", 0},
//     {"军用急救箱", 500, 1, "med", "恢复100生命值", 0},
//     {"肾上腺素", 300, 1, "med", "恢复50生命值", 0},
//     {"手术包", 400, 1, "med", "恢复80生命值", 0},
//     {"大型急救箱", 700, 2, "med", "恢复140生命值", 0},
//     {"团队医疗包", 800, 2, "med", "恢复100生命值", 0}
// };

// const int MEDICAL_ITEMS_COUNT = sizeof(MEDICAL_ITEMS) / sizeof(MEDICAL_ITEMS[0]);

// // 弹药商城
// const Item AMMO_ITEMS[] = {
//     {"步枪弹药箱", 150, 0, "ammo", "5.56mm，30发", 0},
//     {"狙击弹药箱", 200, 0, "ammo", "7.62mm，20发", 0},
//     {"冲锋枪弹药箱", 120, 0, "ammo", "9mm，50发", 0},
//     {"霰弹弹药箱", 100, 0, "ammo", "12号，25发", 0},
//     {"手枪弹药箱", 80, 0, "ammo", "9mm，50发", 0},
//     {"特制穿甲弹", 300, 1, "ammo", "增强穿甲，30发", 0}
// };

// const int AMMO_ITEMS_COUNT = sizeof(AMMO_ITEMS) / sizeof(AMMO_ITEMS[0]);

// // 战术装备
// const Item EQUIPMENT_ITEMS[] = {
//     {"破片手雷", 200, 0, "equipment", "范围伤害", 0},
//     {"闪光弹", 150, 0, "equipment", "致盲敌人", 0},
//     {"烟雾弹", 120, 0, "equipment", "制造烟雾", 0},
//     {"C4炸药", 400, 1, "equipment", "定时引爆", 0},
//     {"战术无人机", 800, 2, "equipment", "空中侦察", 0},
//     {"心跳传感器", 600, 2, "equipment", "探测敌人", 0},
//     {"夜视仪", 500, 1, "equipment", "夜战必备", 0}
// };

// const int EQUIPMENT_ITEMS_COUNT = sizeof(EQUIPMENT_ITEMS) / sizeof(EQUIPMENT_ITEMS[0]);

// // 胸挂装备
// const Item CHEST_RIG_ITEMS[] = {
//     {"通用战术胸挂", 800, 0, "chestrig", "绿色品质", 9},
//     {"简易携行弹挂", 1000, 1, "chestrig", "蓝色品质", 10},
//     {"DSA战术胸挂", 1200, 1, "chestrig", "蓝色品质", 12},
//     {"HD3战术胸挂", 1200, 1, "chestrig", "蓝色品质", 12},
//     {"G01战术弹挂", 1500, 1, "chestrig", "蓝色品质", 13},
//     {"强袭战术背心", 1800, 2, "chestrig", "紫色品质", 14},
//     {"突击者战术背心", 2000, 2, "chestrig", "紫色品质", 16},
//     {"DRC先进侦察胸挂", 2200, 2, "chestrig", "紫色品质", 17},
//     {"GIR野战胸挂", 2500, 2, "chestrig", "紫色品质", 20},
//     {"DAR突击手胸挂", 3000, 3, "chestrig", "金色品质", 24}
// };

// const int CHEST_RIG_ITEMS_COUNT = sizeof(CHEST_RIG_ITEMS) / sizeof(CHEST_RIG_ITEMS[0]);

// // 背包装备
// const Item BACKPACK_ITEMS[] = {
//     {"GA野战背包", 2000, 1, "backpack", "蓝色品质", 20},
//     {"雨林猎手背包", 2100, 1, "backpack", "蓝色品质", 21},
//     {"D2战术登山包", 2800, 2, "backpack", "紫色品质", 24},
//     {"野战徒步背包", 2800, 2, "backpack", "紫色品质", 24},
//     {"GT1户外登山包", 3000, 2, "backpack", "紫色品质", 25},
//     {"D3战术登山包", 3500, 3, "backpack", "金色品质", 28},
//     {"HLS-2重型背包", 3500, 3, "backpack", "金色品质", 28},
//     {"GT5野战背包", 4000, 3, "backpack", "金色品质", 30},
//     {"D7战术背包", 5000, 4, "backpack", "红色品质", 35},
//     {"重型登山包", 6000, 4, "backpack", "红色品质", 40},
//     {"GTO重型战术包", 7000, 4, "backpack", "红色品质", 45}
// };

// const int BACKPACK_ITEMS_COUNT = sizeof(BACKPACK_ITEMS) / sizeof(BACKPACK_ITEMS[0]);

// // 枪械配件
// const Item GUNSMITH_ITEMS[] = {
//     {"红点瞄具", 300, 0, "equipment", "1倍，快速瞄准", 0},
//     {"全息瞄具", 400, 1, "equipment", "1倍，精确瞄准", 0},
//     {"2倍镜", 350, 1, "equipment", "2倍放大", 0},
//     {"4倍镜", 500, 1, "equipment", "4倍放大", 0},
//     {"消音器", 400, 1, "equipment", "降低枪声", 0},
//     {"垂直握把", 350, 1, "equipment", "减少垂直后座", 0},
//     {"加长弹匣", 300, 1, "equipment", "增加弹容量", 0},
//     {"战术镭射", 250, 0, "equipment", "提高腰射精度", 0}
// };

// const int GUNSMITH_ITEMS_COUNT = sizeof(GUNSMITH_ITEMS) / sizeof(GUNSMITH_ITEMS[0]);

// // 物资箱
// const Item SUPPLY_CRATE_ITEMS[] = {
//     {"小型物资箱", 800, 1, "crate", "随机获得2-3件装备", 0},
//     {"中型物资箱", 1500, 2, "crate", "随机获得3-4件装备", 0},
//     {"大型物资箱", 2500, 3, "crate", "随机获得4-5件装备", 0},
//     {"武器物资箱", 2000, 2, "crate", "随机获得1-2件武器", 0},
//     {"护甲物资箱", 1800, 2, "crate", "随机获得1-2件护甲", 0},
//     {"医疗物资箱", 1200, 1, "crate", "随机获得2-3件医疗品", 0}
// };

// const int SUPPLY_CRATE_ITEMS_COUNT = sizeof(SUPPLY_CRATE_ITEMS) / sizeof(SUPPLY_CRATE_ITEMS[0]);

// // 航天基地出生点
// struct SpawnPoint {
//     int x, y;
//     string name;
//     string description;
// };

// const SpawnPoint SPACE_BASE_SPAWNS[] = {
//     {5, 45, "宿舍区", "西北方遮阳伞旁"},
//     {8, 40, "西区大门", "西区大门门口"},
//     {20, 42, "中控楼", "中控楼一楼后门外"},
//     {15, 38, "二号员工通道", "二号员工通道岸边"},
//     {35, 43, "工业区北门", "工业区北方大门内"},
//     {38, 40, "工业区组装室", "工业区组装室一楼"},
//     {42, 42, "罐装区", "罐装区东南角空地"},
//     {45, 5, "发射区", "南部发射区"}
// };

// const int SPAWN_POINTS_COUNT = sizeof(SPACE_BASE_SPAWNS) / sizeof(SPACE_BASE_SPAWNS[0]);

// // ==================== 显示三角洲标题 ====================
// void showDeltaForceTitle() {
//     printf("\n");
//     printf("    %s╔════════════════════════════════════════════════════════════════════════════╗%s\n", COLOR_RED, COLOR_RESET);
//     printf("    %s║                                                                            ║%s\n", COLOR_RED, COLOR_RESET);
//     printf("    %s║                       %s三角洲行动 - DELTA FORCE%s                          %s║%s\n", 
//            COLOR_RED, COLOR_GOLD, COLOR_RED, COLOR_RED, COLOR_RESET);
//     printf("    %s║                         %s零号大坝 / 航天基地%s                             %s║%s\n", 
//            COLOR_RED, COLOR_SILVER, COLOR_RED, COLOR_RED, COLOR_RESET);
//     printf("    %s║                                                                            ║%s\n", COLOR_RED, COLOR_RESET);
//     printf("    %s╚════════════════════════════════════════════════════════════════════════════╝%s\n\n", COLOR_RED, COLOR_RESET);
// }

// // ==================== 显示通行证界面 ====================
// void showBattlePass() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 战斗通行证 ═══════════════════════%s\n\n", COLOR_GOLD, COLOR_RESET);
        
//         printf("    %s当前通行证类型: %s%s%s\n", 
//                COLOR_WHITE, 
//                playerData.battlePass.getColor().c_str(), 
//                playerData.battlePass.getName().c_str(), 
//                COLOR_RESET);
//         printf("    %s当前等级: %s%d%s\n", 
//                COLOR_WHITE, COLOR_GREEN, playerData.battlePass.level, COLOR_RESET);
//         printf("    %s当前经验: %s%d/%d%s\n\n", 
//                COLOR_WHITE, COLOR_YELLOW, playerData.battlePass.exp, 
//                playerData.battlePass.level * 100, COLOR_RESET);
        
//         printf("    %s═══════════════════════ 通行证奖励 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
        
//         const char* rewardNames[] = {"经验卡", "徽章", "武器皮肤", "角色皮肤", "表情", "动作", "挂饰", "边框"};
        
//         for (int i = 1; i <= 10; i++) {
//             string status = "未解锁";
//             string statusColor = COLOR_RED;
            
//             if (i <= playerData.battlePass.level) {
//                 bool claimed = false;
//                 for (int lvl : playerData.battlePass.claimedLevels) {
//                     if (lvl == i) {
//                         claimed = true;
//                         break;
//                     }
//                 }
                
//                 if (claimed) {
//                     status = "已领取";
//                     statusColor = COLOR_SILVER;
//                 } else {
//                     status = "可领取";
//                     statusColor = COLOR_GREEN;
//                 }
//             }
            
//             string passColor = (i <= playerData.battlePass.level) ? COLOR_GREEN : COLOR_WHITE;
            
//             printf("    %s[等级 %2d]%s 通行证%s %s%s%s\n", 
//                    passColor.c_str(), i, COLOR_RESET,
//                    rewardNames[(i-1) % 8],
//                    statusColor.c_str(), status.c_str(), COLOR_RESET);
//         }
        
//         printf("\n    %s──────────────────────────────────────────────────────────%s\n", COLOR_SILVER, COLOR_RESET);
//         printf("\n    %s[1]%s 购买基础通行证 ($500)\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[2]%s 购买战斗通行证 ($1500)\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[3]%s 购买高级通行证 ($3000)\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[4]%s 购买精英通行证 ($5000)\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[L]%s 领取当前等级奖励\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    %s[0]%s 返回\n\n", COLOR_RED, COLOR_RESET);
//         printf("    请选择: ");
        
//         int choice = _getch();
        
//         if (choice == '0') {
//             return;
//         }
//         else if (choice == '1') {
//             if (playerData.battlePass.type == PASS_NONE && playerData.money >= 500) {
//                 playerData.money -= 500;
//                 playerData.battlePass.type = PASS_BASIC;
//                 playerData.battlePass.name = "基础通行证";
//                 playerData.battlePass.level = 1;
//                 printf("\n    %s? 购买基础通行证成功！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             } else if (playerData.battlePass.type != PASS_NONE) {
//                 printf("\n    %s? 已经拥有通行证！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             } else {
//                 printf("\n    %s? 资金不足！需要 $500%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == '2') {
//             if (playerData.battlePass.type == PASS_NONE && playerData.money >= 1500) {
//                 playerData.money -= 1500;
//                 playerData.battlePass.type = PASS_BATTLE;
//                 playerData.battlePass.name = "战斗通行证";
//                 playerData.battlePass.level = 1;
//                 printf("\n    %s? 购买战斗通行证成功！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             } else if (playerData.battlePass.type != PASS_NONE) {
//                 printf("\n    %s? 已经拥有通行证！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             } else {
//                 printf("\n    %s? 资金不足！需要 $1500%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == '3') {
//             if (playerData.battlePass.type == PASS_NONE && playerData.money >= 3000) {
//                 playerData.money -= 3000;
//                 playerData.battlePass.type = PASS_PREMIUM;
//                 playerData.battlePass.name = "高级通行证";
//                 playerData.battlePass.level = 1;
//                 printf("\n    %s? 购买高级通行证成功！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             } else if (playerData.battlePass.type != PASS_NONE) {
//                 printf("\n    %s? 已经拥有通行证！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             } else {
//                 printf("\n    %s? 资金不足！需要 $3000%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == '4') {
//             if (playerData.battlePass.type == PASS_NONE && playerData.money >= 5000) {
//                 playerData.money -= 5000;
//                 playerData.battlePass.type = PASS_ELITE;
//                 playerData.battlePass.name = "精英通行证";
//                 playerData.battlePass.level = 1;
//                 printf("\n    %s? 购买精英通行证成功！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             } else if (playerData.battlePass.type != PASS_NONE) {
//                 printf("\n    %s? 已经拥有通行证！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             } else {
//                 printf("\n    %s? 资金不足！需要 $5000%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == 'l' || choice == 'L') {
//             if (playerData.battlePass.type == PASS_NONE) {
//                 printf("\n    %s? 没有通行证！请先购买%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//                 continue;
//             }
            
//             bool canClaim = false;
//             int claimLevel = -1;
            
//             for (int i = 1; i <= playerData.battlePass.level; i++) {
//                 bool claimed = false;
//                 for (int lvl : playerData.battlePass.claimedLevels) {
//                     if (lvl == i) {
//                         claimed = true;
//                         break;
//                     }
//                 }
//                 if (!claimed) {
//                     canClaim = true;
//                     claimLevel = i;
//                     break;
//                 }
//             }
            
//             if (canClaim) {
//                 playerData.battlePass.claimedLevels.push_back(claimLevel);
//                 Item reward = {"通行证奖励", 100 * claimLevel, 1, "material", "通行证奖励"};
                
//                 if (playerData.inventoryCount < MAX_INVENTORY) {
//                     playerData.inventory[playerData.inventoryCount++] = reward;
//                     printf("\n    %s? 领取等级 %d 奖励成功！%s\n", COLOR_GREEN, claimLevel, COLOR_RESET);
//                 } else {
//                     printf("\n    %s? 背包已满！%s\n", COLOR_RED, COLOR_RESET);
//                 }
//                 Sleep(1500);
//             } else {
//                 printf("\n    %s? 没有可领取的奖励！%s\n", COLOR_YELLOW, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//     }
// }

// // ==================== 显示钥匙卡界面 ====================
// void showKeyCards() {
//     system("cls");
//     showDeltaForceTitle();
//     printf("    %s═══════════════════════ 钥匙卡管理 ═══════════════════════%s\n\n", COLOR_MAGENTA, COLOR_RESET);
    
//     if (playerData.keyCards.empty()) {
//         printf("    %s你还没有任何钥匙卡。%s\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    钥匙卡可以从首领或高级物资箱中获得。\n\n");
//     } else {
//         printf("    %s你拥有的钥匙卡：%s\n\n", COLOR_CYAN, COLOR_RESET);
//         for (size_t i = 0; i < playerData.keyCards.size(); i++) {
//             const KeyCard& card = playerData.keyCards[i];
//             printf("    %s[%d]%s %s%s%s\n", 
//                    COLOR_YELLOW, i + 1, COLOR_RESET,
//                    card.getColor().c_str(), card.name.c_str(), COLOR_RESET);
//             printf("        %s%s%s\n", COLOR_WHITE, card.description.c_str(), COLOR_RESET);
//             printf("        剩余使用次数: %s%d%s\n", COLOR_GREEN, card.uses, COLOR_RESET);
//             if (card.type == KEY_BLACK_ROOM) {
//                 printf("        密码: %s%d%d%d%d%s\n", COLOR_CYAN, 
//                        card.code[0], card.code[1], card.code[2], card.code[3], COLOR_RESET);
//             }
//             printf("\n");
//         }
//     }
    
//     printf("\n    %s═══════════════════════ 钥匙卡门列表 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
    
//     for (int i = 0; i < KEY_DOORS_COUNT; i++) {
//         const KeyCardDoor& door = KEY_DOORS[i];
//         string status = door.isLocked ? "?? 锁定" : "?? 已开";
//         string statusColor = door.isLocked ? COLOR_RED : COLOR_GREEN;
        
//         printf("    %s[%d]%s %s - %s\n", 
//                COLOR_YELLOW, i+1, COLOR_RESET, door.doorName.c_str(), door.description.c_str());
//         printf("        位置: (%d,%d) %s%s%s\n", 
//                door.x, door.y, statusColor.c_str(), status.c_str(), COLOR_RESET);
//         printf("        需要钥匙卡: %s\n", KEY_CARDS[door.requiredCard].name.c_str());
//         printf("        内部: %d个保险箱, %d个战利品箱\n\n", door.safeCount, door.lootBoxCount);
//     }
    
//     printf("\n    按任意键返回...");
//     _getch();
// }

// // ==================== 显示通行证区域界面 ====================
// void showPassZones() {
//     system("cls");
//     showDeltaForceTitle();
//     printf("    %s═══════════════════════ 通行证区域 ═══════════════════════%s\n\n", COLOR_GOLD, COLOR_RESET);
    
//     printf("    %s当前通行证: %s%s%s 等级: %s%d%s\n\n", 
//            COLOR_WHITE,
//            playerData.battlePass.getColor().c_str(),
//            playerData.battlePass.getName().c_str(),
//            COLOR_RESET,
//            COLOR_GREEN, playerData.battlePass.level, COLOR_RESET);
    
//     for (int i = 0; i < PASS_ZONES_COUNT; i++) {
//         const PassZone& zone = PASS_ZONES[i];
        
//         bool canEnter = false;
//         string status = "无法进入";
//         string statusColor = COLOR_RED;
        
//         if (playerData.battlePass.type >= zone.requiredPass && 
//             playerData.battlePass.level >= zone.requiredLevel) {
//             canEnter = true;
//             status = "可进入";
//             statusColor = COLOR_GREEN;
//         } else if (playerData.battlePass.type >= zone.requiredPass) {
//             status = "等级不足";
//             statusColor = COLOR_YELLOW;
//         }
        
//         string passName = "";
//         switch(zone.requiredPass) {
//             case PASS_BASIC: passName = "基础通行证"; break;
//             case PASS_BATTLE: passName = "战斗通行证"; break;
//             case PASS_PREMIUM: passName = "高级通行证"; break;
//             case PASS_ELITE: passName = "精英通行证"; break;
//             default: passName = "未知";
//         }
        
//         printf("    %s[%d]%s %s\n", 
//                COLOR_YELLOW, i+1, COLOR_RESET, zone.name.c_str());
//         printf("        %s\n", zone.description.c_str());
//         printf("        位置: (%d,%d) 需要: %s 等级 %d\n", 
//                zone.x, zone.y, passName.c_str(), zone.requiredLevel);
//         printf("        状态: %s%s%s\n", 
//                statusColor.c_str(), status.c_str(), COLOR_RESET);
//         printf("        内部: %d个保险箱, %d个战利品箱\n\n", 
//                zone.safeCount, zone.lootBoxCount);
//     }
    
//     printf("\n    按任意键返回...");
//     _getch();
// }

// // ==================== 干员选择 ====================
// int showCharacterSelection() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 干员选择 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
        
//         for (int i = 0; i < 14; i++) {
//             printf("    %s[%d]%s %c %s - %s\n", 
//                    COLOR_YELLOW, i + 1, COLOR_RESET, 
//                    characters[i].icon, characters[i].name.c_str(), 
//                    characters[i].description.c_str());
//             printf("        %sHP:%d EN:%d 技能:%s/%s%s\n\n", 
//                    COLOR_GREEN, characters[i].maxHp, characters[i].baseEnergy,
//                    characters[i].skill1Name.c_str(), characters[i].skill2Name.c_str(),
//                    COLOR_RESET);
//         }
        
//         printf("    %s[0]%s 返回\n\n", COLOR_RED, COLOR_RESET);
//         printf("    请选择干员 (1-14): ");
        
//         char input[10];
//         fgets(input, 10, stdin);
//         int choice = atoi(input);
        
//         int c;
//         while ((c = getchar()) != '\n' && c != EOF);
        
//         if (choice == 0) return -1;
//         if (choice >= 1 && choice <= 14) {
//             g_selectedCharacter = choice - 1;
//             printf("\n    %s? 已选择干员: %s%s\n", COLOR_GREEN, 
//                    characters[choice - 1].name.c_str(), COLOR_RESET);
//             Sleep(800);
//             return choice - 1;
//         }
        
//         printf("\n    %s? 无效选择，请重新选择！%s\n", COLOR_RED, COLOR_RESET);
//         Sleep(1000);
//     }
// }

// // ==================== 联机菜单 ====================
// void showMultiplayerMenu() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════ 联机模式 - 局域网对战 ═══════════════════%s\n\n", COLOR_MAGENTA, COLOR_RESET);
        
//         printf("    %s┌───────────────── 请选择操作 ─────────────────┐%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│                                              │%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│    %s[1]%s 创建房间（成为主机）                %s│%s\n", 
//                COLOR_CYAN, COLOR_GREEN, COLOR_CYAN, COLOR_CYAN, COLOR_RESET);
//         printf("    %s│        - 成为服务器，等待其他玩家加入       %s│%s\n", COLOR_CYAN, COLOR_CYAN, COLOR_RESET);
//         printf("    %s│                                              │%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│    %s[2]%s 加入房间（需先启动主机）          %s│%s\n", 
//                COLOR_CYAN, COLOR_GREEN, COLOR_CYAN, COLOR_CYAN, COLOR_RESET);
//         printf("    %s│        - 作为客户端连接至主机               %s│%s\n", COLOR_CYAN, COLOR_CYAN, COLOR_RESET);
//         printf("    %s│                                              │%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│    %s[0]%s 返回主菜单                        %s│%s\n", 
//                COLOR_CYAN, COLOR_RED, COLOR_CYAN, COLOR_CYAN, COLOR_RESET);
//         printf("    %s│                                              │%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s└─────────────────────────────────────────────┘%s\n\n", COLOR_CYAN, COLOR_RESET);
//         printf("    请选择 [1/2/0]: ");
        
//         int choice = _getch() - '0';
        
//         if (choice == 1) {
//             showCreateRoomMenu();
//         } else if (choice == 2) {
//             if (g_lanGame == nullptr) {
//                 g_lanGame = new LANGameManager();
//             }
//             g_lanGame->myCharacterId = g_selectedCharacter;
            
//             printf("\n    %s正在连接主机...%s\n", COLOR_YELLOW, COLOR_RESET);
//             if (g_lanGame->joinRoom()) {
//                 printf("    %s? 连接成功！正在进入大厅...%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//                 showLobby();
//             } else {
//                 printf("    %s? 连接失败！请先启动主机房间。%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(2000);
//             }
//         } else if (choice == 0) {
//             return;
//         }
//     }
// }

// // ==================== 创建房间菜单 ====================
// void showCreateRoomMenu() {
//     system("cls");
//     showDeltaForceTitle();
//     printf("    %s═══════════════════════ 创建游戏房间 ═══════════════════════%s\n\n", COLOR_GOLD, COLOR_RESET);
    
//     string roomName = "三角洲联机房间";
//     int maxPlayers = 4;
//     int mapType = 0;
//     int difficulty = 1;
//     char input[16];
    
//     printf("    %s┌───────────────── 房间设置 ─────────────────┐%s\n", COLOR_CYAN, COLOR_RESET);
//     printf("    %s│                                            │%s\n", COLOR_CYAN, COLOR_RESET);
    
//     printf("    %s│  房间名称 [%s]: %s", COLOR_CYAN, roomName.c_str(), COLOR_RESET);
//     char nameBuffer[32];
//     fgets(nameBuffer, 32, stdin);
//     nameBuffer[strcspn(nameBuffer, "\n")] = 0;
//     if (strlen(nameBuffer) > 0) roomName = nameBuffer;
//     printf("\r    %s│  房间名称: %-32s %s│%s\n", COLOR_CYAN, roomName.c_str(), COLOR_CYAN, COLOR_RESET);
    
//     printf("    %s│                                            │%s\n", COLOR_CYAN, COLOR_RESET);
//     printf("    %s│  最大玩家数 (2-4) [4]: %s", COLOR_CYAN, COLOR_RESET);
//     fgets(input, 16, stdin);
//     if (strlen(input) > 1) maxPlayers = atoi(input);
//     if (maxPlayers < 2) maxPlayers = 2;
//     if (maxPlayers > 4) maxPlayers = 4;
//     printf("\r    %s│  最大玩家数: %-32d %s│%s\n", COLOR_CYAN, maxPlayers, COLOR_CYAN, COLOR_RESET);
    
//     printf("    %s│                                            │%s\n", COLOR_CYAN, COLOR_RESET);
//     printf("    %s│  地图选项:                                 │%s\n", COLOR_CYAN, COLOR_RESET);
//     printf("    %s│     %s[0]%s 零号大坝                          %s│%s\n", 
//            COLOR_CYAN, COLOR_GREEN, COLOR_CYAN, COLOR_CYAN, COLOR_RESET);
//     printf("    %s│     %s[1]%s 航天基地                          %s│%s\n", 
//            COLOR_CYAN, COLOR_GREEN, COLOR_CYAN, COLOR_CYAN, COLOR_RESET);
//     printf("    %s│  请选择 [0]: %s", COLOR_CYAN, COLOR_RESET);
//     fgets(input, 16, stdin);
//     if (strlen(input) > 1) mapType = atoi(input);
//     printf("\r    %s│  地    图: %-32s %s│%s\n", 
//            COLOR_CYAN, mapType == 0 ? "零号大坝" : "航天基地", COLOR_CYAN, COLOR_RESET);
    
//     printf("    %s│                                            │%s\n", COLOR_CYAN, COLOR_RESET);
//     printf("    %s│  难度选项:                                 │%s\n", COLOR_CYAN, COLOR_RESET);
//     printf("    %s│     %s[0]%s 简单                              %s│%s\n", 
//            COLOR_CYAN, COLOR_GREEN, COLOR_CYAN, COLOR_CYAN, COLOR_RESET);
//     printf("    %s│     %s[1]%s 普通                              %s│%s\n", 
//            COLOR_CYAN, COLOR_GREEN, COLOR_CYAN, COLOR_CYAN, COLOR_RESET);
//     printf("    %s│     %s[2]%s 困难                              %s│%s\n", 
//            COLOR_CYAN, COLOR_GREEN, COLOR_CYAN, COLOR_CYAN, COLOR_RESET);
//     printf("    %s│  请选择 [1]: %s", COLOR_CYAN, COLOR_RESET);
//     fgets(input, 16, stdin);
//     if (strlen(input) > 1) difficulty = atoi(input);
//     printf("\r    %s│  难    度: %-32s %s│%s\n", 
//            COLOR_CYAN, 
//            difficulty == 0 ? "简单" : difficulty == 1 ? "普通" : "困难", 
//            COLOR_CYAN, COLOR_RESET);
    
//     printf("    %s│                                            │%s\n", COLOR_CYAN, COLOR_RESET);
//     printf("    %s└─────────────────────────────────────────────┘%s\n\n", COLOR_CYAN, COLOR_RESET);
    
//     if (g_lanGame == nullptr) {
//         g_lanGame = new LANGameManager();
//     }
    
//     g_lanGame->myPlayerName = "主机";
//     g_lanGame->myCharacterId = g_selectedCharacter;
    
//     printf("    %s╔════════════════════════════════════════╗%s\n", COLOR_YELLOW, COLOR_RESET);
//     printf("    %s║     正在创建房间，请稍候...           ║%s\n", COLOR_YELLOW, COLOR_RESET);
//     printf("    %s╚════════════════════════════════════════╝%s\n", COLOR_YELLOW, COLOR_RESET);
    
//     if (g_lanGame->createRoom(roomName, maxPlayers, mapType, difficulty)) {
//         printf("\n    %s? 房间创建成功！%s\n\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s┌─────────────── 服务器信息 ───────────────┐%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│                                          │%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│  房间名: %-30s %s│%s\n", COLOR_CYAN, roomName.c_str(), COLOR_CYAN, COLOR_RESET);
//         printf("    %s│  主机:   %-30s %s│%s\n", COLOR_CYAN, "本机", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│  最大玩家: %d/%-26d %s│%s\n", COLOR_CYAN, maxPlayers, maxPlayers, COLOR_CYAN, COLOR_RESET);
//         printf("    %s│                                          │%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s└──────────────────────────────────────────┘%s\n\n", COLOR_CYAN, COLOR_RESET);
//         printf("    按任意键进入大厅...");
//         _getch();
        
//         showLobby();
//     } else {
//         printf("\n    %s? 房间创建失败！%s\n", COLOR_RED, COLOR_RESET);
//         printf("      可能原因：\n");
//         printf("        1. 已有房间在运行\n");
//         printf("        2. 系统资源不足\n");
//         Sleep(3000);
//     }
// }

// // ==================== 游戏大厅 ====================
// void showLobby() {
//     if (!g_lanGame) return;
    
//     g_lanGame->lobbyActive = true;
    
//     while (g_lanGame && g_lanGame->isConnected && g_lanGame->lobbyActive) {
//         system("cls");
        
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 游戏大厅 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
        
//         if (!g_lanGame->players.empty()) {
//             printf("    %s┌─────────────── 房间信息 ───────────────┐%s\n", COLOR_GOLD, COLOR_RESET);
//             printf("    %s│  房间名称: %-30s %s│%s\n", 
//                    COLOR_GOLD, g_lanGame->currentRoom.roomName.c_str(), COLOR_GOLD, COLOR_RESET);
//             printf("    %s│  主    机: %-30s %s│%s\n", 
//                    COLOR_GOLD, g_lanGame->currentRoom.hostName.c_str(), COLOR_GOLD, COLOR_RESET);
//             printf("    %s│  地    图: %-30s %s│%s\n", 
//                    COLOR_GOLD, 
//                    g_lanGame->currentRoom.mapType == 0 ? "零号大坝" : "航天基地", 
//                    COLOR_GOLD, COLOR_RESET);
//             printf("    %s│  难    度: %-30s %s│%s\n", 
//                    COLOR_GOLD, 
//                    g_lanGame->currentRoom.difficulty == 0 ? "简单" : 
//                    g_lanGame->currentRoom.difficulty == 1 ? "普通" : "困难", 
//                    COLOR_GOLD, COLOR_RESET);
//             printf("    %s│  玩家数量: %d/%-28d %s│%s\n", 
//                    COLOR_GOLD, (int)g_lanGame->players.size(), 
//                    g_lanGame->currentRoom.maxPlayers, COLOR_GOLD, COLOR_RESET);
//             printf("    %s└────────────────────────────────────────┘%s\n\n", COLOR_GOLD, COLOR_RESET);
//         }
        
//         printf("    %s┌─────────────── 在线玩家 (%d/%d) ───────────────┐%s\n", 
//                COLOR_CYAN, (int)g_lanGame->players.size(), 
//                g_lanGame->currentRoom.maxPlayers, COLOR_RESET);
        
//         int playerCount = 0;
//         for (const auto& player : g_lanGame->players) {
//             playerCount++;
//             const char* status = player.isReady ? "? 已准备" : "○ 未准备";
//             const char* statusColor = player.isReady ? COLOR_GREEN : COLOR_YELLOW;
//             const char* isYou = (player.playerId == g_lanGame->myPlayerId) ? " [你]" : "";
//             const char* isHost = player.isHost ? " [主机]" : "";
            
//             string charName = "未知";
//             if (player.characterId >= 0 && player.characterId < 14) {
//                 charName = characters[player.characterId].name;
//             }
            
//             string displayName = player.playerName + string(isYou) + string(isHost);
//             printf("    %s│  %-2d. %-22s %-10s %s%-8s%s %s│%s\n", 
//                    COLOR_CYAN, playerCount, displayName.c_str(), charName.c_str(),
//                    statusColor, status, COLOR_CYAN, COLOR_CYAN, COLOR_RESET);
//         }
        
//         int emptySlots = g_lanGame->currentRoom.maxPlayers - g_lanGame->players.size();
//         for (int i = 0; i < emptySlots; i++) {
//             printf("    %s│  %-2d. %-22s %-10s %s%-8s%s %s│%s\n", 
//                    COLOR_CYAN, playerCount + i + 1, "等待玩家...", "---",
//                    COLOR_YELLOW, "○ 未准备", COLOR_CYAN, COLOR_CYAN, COLOR_RESET);
//         }
//         printf("    %s└────────────────────────────────────────────────┘%s\n\n", COLOR_CYAN, COLOR_RESET);
        
//         printf("    %s┌────────────────── 操作说明 ──────────────────┐%s\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s│                                              │%s\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s│    %s[P]%s 准备/取消准备                        %s│%s\n", 
//                COLOR_GREEN, COLOR_YELLOW, COLOR_GREEN, COLOR_GREEN, COLOR_RESET);
//         if (g_lanGame->isHost) {
//             printf("    %s│    %s[S]%s 开始游戏（需全员准备）              %s│%s\n", 
//                    COLOR_GREEN, COLOR_YELLOW, COLOR_GREEN, COLOR_GREEN, COLOR_RESET);
//         }
//         printf("    %s│    %s[Q]%s 退出大厅                            %s│%s\n", 
//                COLOR_GREEN, COLOR_RED, COLOR_GREEN, COLOR_GREEN, COLOR_RESET);
//         printf("    %s│                                              │%s\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s└──────────────────────────────────────────────┘%s\n\n", COLOR_GREEN, COLOR_RESET);
        
//         printf("    %s════════════════════════════════════════════════════════════════%s\n", COLOR_SILVER, COLOR_RESET);
//         printf("    你的状态: %s%s%s\n", 
//                g_lanGame->players[g_lanGame->myPlayerId].isReady ? COLOR_GREEN : COLOR_YELLOW,
//                g_lanGame->players[g_lanGame->myPlayerId].isReady ? "? 已准备" : "○ 未准备",
//                COLOR_RESET);
//         printf("    %s════════════════════════════════════════════════════════════════%s\n", COLOR_SILVER, COLOR_RESET);
//         printf("    请选择操作: ");
        
//         if (_kbhit()) {
//             int key = _getch();
            
//             if (key == 'p' || key == 'P') {
//                 g_lanGame->toggleReady();
//             }
//             else if (key == 's' || key == 'S') {
//                 if (g_lanGame->isHost) {
//                     bool allReady = true;
//                     for (const auto& player : g_lanGame->players) {
//                         if (!player.isReady) {
//                             allReady = false;
//                             break;
//                         }
//                     }
                    
//                     if (allReady && g_lanGame->players.size() >= 1) {
//                         printf("\n\n    %s╔════════════════════════════════════════╗%s\n", COLOR_GREEN, COLOR_RESET);
//                         printf("    %s║    所有玩家已准备，开始游戏...        ║%s\n", COLOR_GREEN, COLOR_RESET);
//                         printf("    %s╚════════════════════════════════════════╝%s\n", COLOR_GREEN, COLOR_RESET);
//                         Sleep(2000);
                        
//                         system("cls");
//                         printf("\n\n");
//                         printf("    %s╔════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
//                         printf("    %s║        ?? 联机任务正在启动...        ║%s\n", COLOR_CYAN, COLOR_RESET);
//                         printf("    %s╚════════════════════════════════════════╝%s\n\n", COLOR_CYAN, COLOR_RESET);
//                         printf("    地图: %s\n", g_lanGame->currentRoom.mapType == 0 ? "零号大坝" : "航天基地");
//                         printf("    难度: %s\n", g_lanGame->currentRoom.difficulty == 0 ? "简单" : 
//                                                g_lanGame->currentRoom.difficulty == 1 ? "普通" : "困难");
//                         printf("    队友: %d 人\n\n", (int)g_lanGame->players.size() - 1);
//                         printf("    按任意键开始行动...");
//                         _getch();
                        
//                         g_lanGame->lobbyActive = false;
//                         return;
//                     } else {
//                         printf("\n\n    %s? 等待所有玩家准备...%s\n", COLOR_RED, COLOR_RESET);
//                         Sleep(1500);
//                     }
//                 }
//             }
//             else if (key == 'q' || key == 'Q') {
//                 printf("\n\n    %s╔════════════════════════════════════════╗%s\n", COLOR_RED, COLOR_RESET);
//                 printf("    %s║        正在退出大厅...                ║%s\n", COLOR_RED, COLOR_RESET);
//                 printf("    %s╚════════════════════════════════════════╝%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//                 g_lanGame->disconnect();
//                 g_lanGame->lobbyActive = false;
//                 return;
//             }
//         }
        
//         Sleep(200);
//     }
// }

// // ==================== 武器商城 ====================
// void showWeaponMarket() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 武器商城 ═══════════════════════%s\n\n", COLOR_RED, COLOR_RESET);
//         printf("    %s你的资金: %s$%d%s | 背包空间: %d/%d\n\n", 
//                COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET, 
//                playerData.inventoryCount, MAX_INVENTORY);
        
//         for (int i = 0; i < WEAPON_ITEMS_COUNT; i++) {
//             printf("    %s[%d]%s %-20s - %s$%-5d%s | %s\n", 
//                    COLOR_YELLOW, i + 1, COLOR_RESET, 
//                    WEAPON_ITEMS[i].name.c_str(),
//                    COLOR_GREEN, WEAPON_ITEMS[i].price, COLOR_RESET,
//                    WEAPON_ITEMS[i].description.c_str());
//         }
        
//         printf("\n    %s[0]%s 返回上级菜单\n\n", COLOR_RED, COLOR_RESET);
//         printf("    请输入物品编号购买: ");
        
//         int choice;
//         scanf("%d", &choice);
//         getchar();
        
//         if (choice == 0) break;
//         if (choice >= 1 && choice <= WEAPON_ITEMS_COUNT) {
//             const Item& selectedItem = WEAPON_ITEMS[choice - 1];
            
//             if (!playerData.hasBackpack) {
//                 printf("\n    %s? 你没有背包！请先在仓库购买背包。%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             }
//             else if (playerData.money >= selectedItem.price) {
//                 if (playerData.inventoryCount < MAX_INVENTORY) {
//                     playerData.money -= selectedItem.price;
//                     playerData.inventory[playerData.inventoryCount++] = selectedItem;
//                     printf("\n    %s? 购买成功！获得 %s%s\n", COLOR_GREEN, selectedItem.name.c_str(), COLOR_RESET);
//                     printf("    剩余资金: %s$%d%s\n", COLOR_YELLOW, playerData.money, COLOR_RESET);
//                     Sleep(1500);
//                 } else {
//                     printf("\n    %s? 背包已满！%s\n", COLOR_RED, COLOR_RESET);
//                     Sleep(1500);
//                 }
//             } else {
//                 printf("\n    %s? 资金不足！需要 $%d%s\n", COLOR_RED, selectedItem.price, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//     }
// }

// // ==================== 护甲商城 ====================
// void showArmorMarket() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 护甲商城 ═══════════════════════%s\n\n", COLOR_BLUE, COLOR_RESET);
//         printf("    %s你的资金: %s$%d%s | 背包空间: %d/%d\n\n", 
//                COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET, 
//                playerData.inventoryCount, MAX_INVENTORY);
        
//         for (int i = 0; i < ARMOR_ITEMS_COUNT; i++) {
//             printf("    %s[%d]%s %-20s - %s$%-5d%s | %s\n", 
//                    COLOR_YELLOW, i + 1, COLOR_RESET, 
//                    ARMOR_ITEMS[i].name.c_str(),
//                    COLOR_GREEN, ARMOR_ITEMS[i].price, COLOR_RESET,
//                    ARMOR_ITEMS[i].description.c_str());
//         }
        
//         printf("\n    %s[0]%s 返回上级菜单\n\n", COLOR_RED, COLOR_RESET);
//         printf("    请输入物品编号购买: ");
        
//         int choice;
//         scanf("%d", &choice);
//         getchar();
        
//         if (choice == 0) break;
//         if (choice >= 1 && choice <= ARMOR_ITEMS_COUNT) {
//             const Item& selectedItem = ARMOR_ITEMS[choice - 1];
            
//             if (!playerData.hasBackpack) {
//                 printf("\n    %s? 你没有背包！请先在仓库购买背包。%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             }
//             else if (playerData.money >= selectedItem.price) {
//                 if (playerData.inventoryCount < MAX_INVENTORY) {
//                     playerData.money -= selectedItem.price;
//                     playerData.inventory[playerData.inventoryCount++] = selectedItem;
//                     printf("\n    %s? 购买成功！获得 %s%s\n", COLOR_GREEN, selectedItem.name.c_str(), COLOR_RESET);
//                     printf("    剩余资金: %s$%d%s\n", COLOR_YELLOW, playerData.money, COLOR_RESET);
//                     Sleep(1500);
//                 } else {
//                     printf("\n    %s? 背包已满！%s\n", COLOR_RED, COLOR_RESET);
//                     Sleep(1500);
//                 }
//             } else {
//                 printf("\n    %s? 资金不足！需要 $%d%s\n", COLOR_RED, selectedItem.price, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//     }
// }

// // ==================== 医疗商城 ====================
// void showMedicalMarket() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 医疗商城 ═══════════════════════%s\n\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s你的资金: %s$%d%s | 背包空间: %d/%d\n\n", 
//                COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET, 
//                playerData.inventoryCount, MAX_INVENTORY);
        
//         for (int i = 0; i < MEDICAL_ITEMS_COUNT; i++) {
//             printf("    %s[%d]%s %-20s - %s$%-5d%s | %s\n", 
//                    COLOR_YELLOW, i + 1, COLOR_RESET, 
//                    MEDICAL_ITEMS[i].name.c_str(),
//                    COLOR_GREEN, MEDICAL_ITEMS[i].price, COLOR_RESET,
//                    MEDICAL_ITEMS[i].description.c_str());
//         }
        
//         printf("\n    %s[0]%s 返回上级菜单\n\n", COLOR_RED, COLOR_RESET);
//         printf("    请输入物品编号购买: ");
        
//         int choice;
//         scanf("%d", &choice);
//         getchar();
        
//         if (choice == 0) break;
//         if (choice >= 1 && choice <= MEDICAL_ITEMS_COUNT) {
//             const Item& selectedItem = MEDICAL_ITEMS[choice - 1];
            
//             if (!playerData.hasBackpack) {
//                 printf("\n    %s? 你没有背包！请先在仓库购买背包。%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             }
//             else if (playerData.money >= selectedItem.price) {
//                 if (playerData.inventoryCount < MAX_INVENTORY) {
//                     playerData.money -= selectedItem.price;
//                     playerData.inventory[playerData.inventoryCount++] = selectedItem;
//                     printf("\n    %s? 购买成功！获得 %s%s\n", COLOR_GREEN, selectedItem.name.c_str(), COLOR_RESET);
//                     printf("    剩余资金: %s$%d%s\n", COLOR_YELLOW, playerData.money, COLOR_RESET);
//                     Sleep(1500);
//                 } else {
//                     printf("\n    %s? 背包已满！%s\n", COLOR_RED, COLOR_RESET);
//                     Sleep(1500);
//                 }
//             } else {
//                 printf("\n    %s? 资金不足！需要 $%d%s\n", COLOR_RED, selectedItem.price, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//     }
// }

// // ==================== 弹药商城 ====================
// void showAmmoMarket() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 弹药商城 ═══════════════════════%s\n\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    %s你的资金: %s$%d%s | 背包空间: %d/%d\n\n", 
//                COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET, 
//                playerData.inventoryCount, MAX_INVENTORY);
        
//         for (int i = 0; i < AMMO_ITEMS_COUNT; i++) {
//             printf("    %s[%d]%s %-20s - %s$%-5d%s | %s\n", 
//                    COLOR_YELLOW, i + 1, COLOR_RESET, 
//                    AMMO_ITEMS[i].name.c_str(),
//                    COLOR_GREEN, AMMO_ITEMS[i].price, COLOR_RESET,
//                    AMMO_ITEMS[i].description.c_str());
//         }
        
//         printf("\n    %s[0]%s 返回上级菜单\n\n", COLOR_RED, COLOR_RESET);
//         printf("    请输入物品编号购买: ");
        
//         int choice;
//         scanf("%d", &choice);
//         getchar();
        
//         if (choice == 0) break;
//         if (choice >= 1 && choice <= AMMO_ITEMS_COUNT) {
//             const Item& selectedItem = AMMO_ITEMS[choice - 1];
            
//             if (!playerData.hasBackpack) {
//                 printf("\n    %s? 你没有背包！请先在仓库购买背包。%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             }
//             else if (playerData.money >= selectedItem.price) {
//                 if (playerData.inventoryCount < MAX_INVENTORY) {
//                     playerData.money -= selectedItem.price;
//                     playerData.inventory[playerData.inventoryCount++] = selectedItem;
//                     printf("\n    %s? 购买成功！获得 %s%s\n", COLOR_GREEN, selectedItem.name.c_str(), COLOR_RESET);
//                     printf("    剩余资金: %s$%d%s\n", COLOR_YELLOW, playerData.money, COLOR_RESET);
//                     Sleep(1500);
//                 } else {
//                     printf("\n    %s? 背包已满！%s\n", COLOR_RED, COLOR_RESET);
//                     Sleep(1500);
//                 }
//             } else {
//                 printf("\n    %s? 资金不足！需要 $%d%s\n", COLOR_RED, selectedItem.price, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//     }
// }

// // ==================== 战术装备商城 ====================
// void showEquipmentMarket() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 战术装备商城 ═══════════════════════%s\n\n", COLOR_MAGENTA, COLOR_RESET);
//         printf("    %s你的资金: %s$%d%s | 背包空间: %d/%d\n\n", 
//                COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET, 
//                playerData.inventoryCount, MAX_INVENTORY);
        
//         for (int i = 0; i < EQUIPMENT_ITEMS_COUNT; i++) {
//             printf("    %s[%d]%s %-20s - %s$%-5d%s | %s\n", 
//                    COLOR_YELLOW, i + 1, COLOR_RESET, 
//                    EQUIPMENT_ITEMS[i].name.c_str(),
//                    COLOR_GREEN, EQUIPMENT_ITEMS[i].price, COLOR_RESET,
//                    EQUIPMENT_ITEMS[i].description.c_str());
//         }
        
//         printf("\n    %s[0]%s 返回上级菜单\n\n", COLOR_RED, COLOR_RESET);
//         printf("    请输入物品编号购买: ");
        
//         int choice;
//         scanf("%d", &choice);
//         getchar();
        
//         if (choice == 0) break;
//         if (choice >= 1 && choice <= EQUIPMENT_ITEMS_COUNT) {
//             const Item& selectedItem = EQUIPMENT_ITEMS[choice - 1];
            
//             if (!playerData.hasBackpack) {
//                 printf("\n    %s? 你没有背包！请先在仓库购买背包。%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             }
//             else if (playerData.money >= selectedItem.price) {
//                 if (playerData.inventoryCount < MAX_INVENTORY) {
//                     playerData.money -= selectedItem.price;
//                     playerData.inventory[playerData.inventoryCount++] = selectedItem;
//                     printf("\n    %s? 购买成功！获得 %s%s\n", COLOR_GREEN, selectedItem.name.c_str(), COLOR_RESET);
//                     printf("    剩余资金: %s$%d%s\n", COLOR_YELLOW, playerData.money, COLOR_RESET);
//                     Sleep(1500);
//                 } else {
//                     printf("\n    %s? 背包已满！%s\n", COLOR_RED, COLOR_RESET);
//                     Sleep(1500);
//                 }
//             } else {
//                 printf("\n    %s? 资金不足！需要 $%d%s\n", COLOR_RED, selectedItem.price, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//     }
// }

// // ==================== 背包配件商城 ====================
// void showChestRigMarket() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 胸挂装备商城 ═══════════════════════%s\n\n", COLOR_ORANGE, COLOR_RESET);
//         printf("    %s你的资金: %s$%d%s | 当前胸挂: %s%s%s (容量:%d)\n\n", 
//                COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET,
//                COLOR_GREEN, playerData.equippedChestRig.name.empty() ? "未装备" : playerData.equippedChestRig.name.c_str(), COLOR_RESET,
//                playerData.equippedChestRig.capacity);
        
//         for (int i = 0; i < CHEST_RIG_ITEMS_COUNT; i++) {
//             printf("    %s[%d]%s %-22s - %s$%-5d%s | 容量:%d格 | %s\n", 
//                    COLOR_YELLOW, i + 1, COLOR_RESET, 
//                    CHEST_RIG_ITEMS[i].name.c_str(),
//                    COLOR_GREEN, CHEST_RIG_ITEMS[i].price, COLOR_RESET,
//                    CHEST_RIG_ITEMS[i].capacity,
//                    CHEST_RIG_ITEMS[i].description.c_str());
//         }
        
//         printf("\n    %s[0]%s 返回上级菜单\n\n", COLOR_RED, COLOR_RESET);
//         printf("    请输入物品编号购买: ");
        
//         int choice;
//         scanf("%d", &choice);
//         getchar();
        
//         if (choice == 0) break;
//         if (choice >= 1 && choice <= CHEST_RIG_ITEMS_COUNT) {
//             const Item& selectedItem = CHEST_RIG_ITEMS[choice - 1];
            
//             if (playerData.money >= selectedItem.price) {
//                 if (playerData.warehouseCount < MAX_WAREHOUSE) {
//                     playerData.money -= selectedItem.price;
//                     playerData.warehouse[playerData.warehouseCount++] = selectedItem;
//                     printf("\n    %s✓ 购买成功！已放入仓库：%s (容量+%d格)%s\n", COLOR_GREEN, selectedItem.name.c_str(), selectedItem.capacity, COLOR_RESET);
//                     printf("    剩余资金: %s$%d%s\n", COLOR_YELLOW, playerData.money, COLOR_RESET);
//                     Sleep(1500);
//                 } else {
//                     printf("\n    %s✗ 仓库已满！%s\n", COLOR_RED, COLOR_RESET);
//                     Sleep(1500);
//                 }
//             } else {
//                 printf("\n    %s✗ 资金不足！需要 $%d%s\n", COLOR_RED, selectedItem.price, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//     }
// }

// void showBackpackMarket() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 背包装备商城 ═══════════════════════%s\n\n", COLOR_ORANGE, COLOR_RESET);
//         printf("    %s你的资金: %s$%d%s | 当前背包: %s%s%s (容量:%d)\n\n", 
//                COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET,
//                COLOR_GREEN, playerData.equippedBackpack.name.empty() ? "未装备" : playerData.equippedBackpack.name.c_str(), COLOR_RESET,
//                playerData.equippedBackpack.capacity);
        
//         for (int i = 0; i < BACKPACK_ITEMS_COUNT; i++) {
//             printf("    %s[%d]%s %-22s - %s$%-5d%s | 容量:%d格 | %s\n", 
//                    COLOR_YELLOW, i + 1, COLOR_RESET, 
//                    BACKPACK_ITEMS[i].name.c_str(),
//                    COLOR_GREEN, BACKPACK_ITEMS[i].price, COLOR_RESET,
//                    BACKPACK_ITEMS[i].capacity,
//                    BACKPACK_ITEMS[i].description.c_str());
//         }
        
//         printf("\n    %s[0]%s 返回上级菜单\n\n", COLOR_RED, COLOR_RESET);
//         printf("    请输入物品编号购买: ");
        
//         int choice;
//         scanf("%d", &choice);
//         getchar();
        
//         if (choice == 0) break;
//         if (choice >= 1 && choice <= BACKPACK_ITEMS_COUNT) {
//             const Item& selectedItem = BACKPACK_ITEMS[choice - 1];
            
//             if (playerData.money >= selectedItem.price) {
//                 if (playerData.warehouseCount < MAX_WAREHOUSE) {
//                     playerData.money -= selectedItem.price;
//                     playerData.warehouse[playerData.warehouseCount++] = selectedItem;
//                     printf("\n    %s✓ 购买成功！已放入仓库：%s (容量+%d格)%s\n", COLOR_GREEN, selectedItem.name.c_str(), selectedItem.capacity, COLOR_RESET);
//                     printf("    剩余资金: %s$%d%s\n", COLOR_YELLOW, playerData.money, COLOR_RESET);
//                     Sleep(1500);
//                 } else {
//                     printf("\n    %s✗ 仓库已满！%s\n", COLOR_RED, COLOR_RESET);
//                     Sleep(1500);
//                 }
//             } else {
//                 printf("\n    %s✗ 资金不足！需要 $%d%s\n", COLOR_RED, selectedItem.price, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//     }
// }

// // ==================== 枪械配件商城 ====================
// void showGunsmithMarket() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 枪械配件商城 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s你的资金: %s$%d%s | 背包空间: %d/%d\n\n", 
//                COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET, 
//                playerData.inventoryCount, MAX_INVENTORY);
        
//         for (int i = 0; i < GUNSMITH_ITEMS_COUNT; i++) {
//             printf("    %s[%d]%s %-20s - %s$%-5d%s | %s\n", 
//                    COLOR_YELLOW, i + 1, COLOR_RESET, 
//                    GUNSMITH_ITEMS[i].name.c_str(),
//                    COLOR_GREEN, GUNSMITH_ITEMS[i].price, COLOR_RESET,
//                    GUNSMITH_ITEMS[i].description.c_str());
//         }
        
//         printf("\n    %s[0]%s 返回上级菜单\n\n", COLOR_RED, COLOR_RESET);
//         printf("    请输入物品编号购买: ");
        
//         int choice;
//         scanf("%d", &choice);
//         getchar();
        
//         if (choice == 0) break;
//         if (choice >= 1 && choice <= GUNSMITH_ITEMS_COUNT) {
//             const Item& selectedItem = GUNSMITH_ITEMS[choice - 1];
            
//             if (!playerData.hasBackpack) {
//                 printf("\n    %s? 你没有背包！请先在仓库购买背包。%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             }
//             else if (playerData.money >= selectedItem.price) {
//                 if (playerData.inventoryCount < MAX_INVENTORY) {
//                     playerData.money -= selectedItem.price;
//                     playerData.inventory[playerData.inventoryCount++] = selectedItem;
//                     printf("\n    %s? 购买成功！获得 %s%s\n", COLOR_GREEN, selectedItem.name.c_str(), COLOR_RESET);
//                     printf("    剩余资金: %s$%d%s\n", COLOR_YELLOW, playerData.money, COLOR_RESET);
//                     Sleep(1500);
//                 } else {
//                     printf("\n    %s? 背包已满！%s\n", COLOR_RED, COLOR_RESET);
//                     Sleep(1500);
//                 }
//             } else {
//                 printf("\n    %s? 资金不足！需要 $%d%s\n", COLOR_RED, selectedItem.price, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//     }
// }

// // ==================== 物资箱商城 ====================
// void showSupplyCrateMarket() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 物资箱商城 ═══════════════════════%s\n\n", COLOR_GOLD, COLOR_RESET);
//         printf("    %s你的资金: %s$%d%s | 背包空间: %d/%d\n\n", 
//                COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET, 
//                playerData.inventoryCount, MAX_INVENTORY);
        
//         for (int i = 0; i < SUPPLY_CRATE_ITEMS_COUNT; i++) {
//             printf("    %s[%d]%s %-20s - %s$%-5d%s | %s\n", 
//                    COLOR_YELLOW, i + 1, COLOR_RESET, 
//                    SUPPLY_CRATE_ITEMS[i].name.c_str(),
//                    COLOR_GREEN, SUPPLY_CRATE_ITEMS[i].price, COLOR_RESET,
//                    SUPPLY_CRATE_ITEMS[i].description.c_str());
//         }
        
//         printf("\n    %s[0]%s 返回上级菜单\n\n", COLOR_RED, COLOR_RESET);
//         printf("    请输入物品编号购买: ");
        
//         int choice;
//         scanf("%d", &choice);
//         getchar();
        
//         if (choice == 0) break;
//         if (choice >= 1 && choice <= SUPPLY_CRATE_ITEMS_COUNT) {
//             const Item& selectedItem = SUPPLY_CRATE_ITEMS[choice - 1];
            
//             if (!playerData.hasBackpack) {
//                 printf("\n    %s? 你没有背包！请先在仓库购买背包。%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             }
//             else if (playerData.money >= selectedItem.price) {
//                 if (playerData.inventoryCount < MAX_INVENTORY) {
//                     playerData.money -= selectedItem.price;
//                     playerData.inventory[playerData.inventoryCount++] = selectedItem;
//                     printf("\n    %s? 购买成功！获得 %s%s\n", COLOR_GREEN, selectedItem.name.c_str(), COLOR_RESET);
//                     printf("    剩余资金: %s$%d%s\n", COLOR_YELLOW, playerData.money, COLOR_RESET);
//                     Sleep(1500);
//                 } else {
//                     printf("\n    %s? 背包已满！%s\n", COLOR_RED, COLOR_RESET);
//                     Sleep(1500);
//                 }
//             } else {
//                 printf("\n    %s? 资金不足！需要 $%d%s\n", COLOR_RED, selectedItem.price, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//     }
// }

// // ==================== 主市场菜单 ====================
// void showMarket() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 官方商店 ═══════════════════════%s\n\n", COLOR_GOLD, COLOR_RESET);
        
//         printf("    %s【你的资金】: %s$%d%s | 【背包空间】: %d/%d\n\n", 
//                COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET, 
//                playerData.inventoryCount, MAX_INVENTORY);
//         printf("    %s════════════════════════════════════════════════════════%s\n", COLOR_SILVER, COLOR_RESET);
//         printf("    %s【商品分类】%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s════════════════════════════════════════════════════════%s\n\n", COLOR_SILVER, COLOR_RESET);
//         printf("    %s[1]%s 🔫 武器类     - %d款枪械\n", COLOR_GREEN, COLOR_RESET, WEAPON_ITEMS_COUNT);
//         printf("    %s[2]%s 🛡️ 护甲类     - %d款护具\n", COLOR_GREEN, COLOR_RESET, ARMOR_ITEMS_COUNT);
//         printf("    %s[3]%s 🏥 医疗物资   - %d款药品\n", COLOR_GREEN, COLOR_RESET, MEDICAL_ITEMS_COUNT);
//         printf("    %s[4]%s 🎯 弹药类     - %d款弹药\n", COLOR_GREEN, COLOR_RESET, AMMO_ITEMS_COUNT);
//         printf("    %s[5]%s 🎲 战术装备   - %d款装备\n", COLOR_GREEN, COLOR_RESET, EQUIPMENT_ITEMS_COUNT);
//         printf("    %s[6]%s 👕 胸挂装备   - %d款胸挂\n", COLOR_GREEN, COLOR_RESET, CHEST_RIG_ITEMS_COUNT);
//         printf("    %s[7]%s 🔧 枪械配件   - %d款配件\n", COLOR_GREEN, COLOR_RESET, GUNSMITH_ITEMS_COUNT);
//         printf("    %s[8]%s 📦 物资箱     - %d款箱子\n", COLOR_GREEN, COLOR_RESET, SUPPLY_CRATE_ITEMS_COUNT);
//         printf("    %s[9]%s 🎒 背包装备   - %d款背包\n", COLOR_GREEN, COLOR_RESET, BACKPACK_ITEMS_COUNT);
//         printf("    %s[0]%s ❌ 返回主菜单\n\n", COLOR_RED, COLOR_RESET);
//         printf("    %s════════════════════════════════════════════════════════%s\n", COLOR_SILVER, COLOR_RESET);
//         printf("    请选择分类: ");
        
//         int category = _getch() - '0';
        
//         if (category == 0) {
//             return;
//         }
//         else if (category == 1) {
//             showWeaponMarket();
//         }
//         else if (category == 2) {
//             showArmorMarket();
//         }
//         else if (category == 3) {
//             showMedicalMarket();
//         }
//         else if (category == 4) {
//             showAmmoMarket();
//         }
//         else if (category == 5) {
//             showEquipmentMarket();
//         }
//         else if (category == 6) {
//             showChestRigMarket();
//         }
//         else if (category == 7) {
//             showGunsmithMarket();
//         }
//         else if (category == 8) {
//             showSupplyCrateMarket();
//         }
//         else if (category == 9) {
//             showBackpackMarket();
//         }
//         else {
//             printf("\n    %s? 无效选择！%s\n", COLOR_RED, COLOR_RESET);
//             Sleep(1000);
//         }
//     }
// }

// void showTradingPost() {
//     system("cls");
//     showDeltaForceTitle();
//     printf("    %s═══════════════════════ 玩家交易行 ═══════════════════════%s\n\n", COLOR_GOLD, COLOR_RESET);
//     printf("    交易行功能正在开发中...\n\n    按任意键返回...");
//     _getch();
// }

// // ==================== 仓库管理系统 ====================
// void showInventory() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 仓库管理 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s资金: %s$%d%s\n", COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET);
//         printf("    %s背包状态: %s%s%s\n", COLOR_WHITE, 
//                playerData.hasBackpack ? COLOR_GREEN : COLOR_RED,
//                playerData.hasBackpack ? "已拥有" : "未购买", COLOR_RESET);
//         printf("    %s安全箱: %s%s%s\n", COLOR_WHITE,
//                playerData.hasSafeBoxItem ? COLOR_GREEN : COLOR_YELLOW,
//                playerData.hasSafeBoxItem ? 
//                (playerData.safeBox.name + " ($" + to_string(playerData.safeBox.price) + ")").c_str() : "空",
//                COLOR_RESET);
//         printf("    %s钥匙卡: %s%d 张%s\n", COLOR_WHITE, COLOR_GREEN, (int)playerData.keyCards.size(), COLOR_RESET);
//         printf("    %s通行证: %s%s%s 等级 %d%s\n\n", 
//                COLOR_WHITE,
//                playerData.battlePass.getColor().c_str(),
//                playerData.battlePass.getName().c_str(),
//                COLOR_RESET,
//                playerData.battlePass.level, COLOR_RESET);
        
//         printf("    %s──────────────────────────────────────%s\n", COLOR_SILVER, COLOR_RESET);
//         printf("    %s[1]%s 购买背包 ($500) - 可携带80件物品\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[2]%s 查看背包物品\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[3]%s 安全箱管理\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[4]%s 查看钥匙卡\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[5]%s 通行证管理\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[6]%s 通行证区域\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[0]%s 返回主菜单\n", COLOR_RED, COLOR_RESET);
//         printf("    %s══════════════════════════════════════%s\n", COLOR_SILVER, COLOR_RESET);
//         printf("    请选择: ");
        
//         int choice = _getch() - '0';
        
//         if (choice == 0) return;
//         else if (choice == 1) {
//             if (playerData.hasBackpack) {
//                 printf("\n\n    %s你已经拥有背包了！%s\n", COLOR_YELLOW, COLOR_RESET);
//                 Sleep(1500);
//             } else if (playerData.money >= 500) {
//                 playerData.money -= 500;
//                 playerData.hasBackpack = true;
//                 printf("\n\n    %s? 购买成功！你现在拥有背包了！%s\n", COLOR_GREEN, COLOR_RESET);
//                 printf("    可以携带最多80件物品。\n");
//                 Sleep(2000);
//             } else {
//                 printf("\n\n    %s? 资金不足！需要 $500%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == 2) {
//             if (!playerData.hasBackpack) {
//                 printf("\n\n    %s你没有背包！请先购买背包。%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//                 continue;
//             }
            
//             while (true) {
//                 system("cls");
//                 showDeltaForceTitle();
//                 printf("    %s═══════════════════════ 背包物品 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
//                 printf("    %s资金: %s$%d%s | 物品数量: %d/%d\n\n", 
//                        COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET, 
//                        playerData.inventoryCount, MAX_INVENTORY);
                
//                 if (playerData.inventoryCount == 0) {
//                     printf("    %s背包是空的...%s\n", COLOR_YELLOW, COLOR_RESET);
//                     printf("\n    按任意键返回...");
//                     _getch();
//                     break;
//                 } else {
//                     printf("    %s【物品列表】%s\n", COLOR_CYAN, COLOR_RESET);
//                     for (int i = 0; i < playerData.inventoryCount; i++) {
//                         Item& item = playerData.inventory[i];
//                         int recyclePrice = (int)(item.price * 0.8);
//                         printf("    %s[%d]%s %s - 原价:%s$%d%s 回收:%s$%d%s", 
//                                COLOR_YELLOW, i + 1, COLOR_RESET, 
//                                item.name.c_str(),
//                                COLOR_GREEN, item.price, COLOR_RESET,
//                                COLOR_ORANGE, recyclePrice, COLOR_RESET);
//                         if (item.type == "material") printf(" %s[可存入安全箱]%s", COLOR_GREEN, COLOR_RESET);
//                         printf("\n");
//                     }
//                 }
                
//                 printf("\n    %s──────────────────────────────────────%s", COLOR_SILVER, COLOR_RESET);
//                 printf("\n    %sR+编号%s: 回收物品", COLOR_YELLOW, COLOR_RESET);
//                 printf("\n    %sS+编号%s: 存入安全箱（仅变卖物，死亡不会掉落）", COLOR_GREEN, COLOR_RESET);
//                 printf("\n    %s0%s: 返回\n", COLOR_RED, COLOR_RESET);
//                 printf("    %s══════════════════════════════════════%s\n", COLOR_SILVER, COLOR_RESET);
//                 printf("    请输入: ");
                
//                 char inputBuffer[256];
//                 fgets(inputBuffer, 256, stdin);
                
//                 if (inputBuffer[0] == '0') break;
//                 else if (inputBuffer[0] == 'R' || inputBuffer[0] == 'r') {
//                     int num = atoi(inputBuffer + 1);
//                     if (num > 0 && num <= playerData.inventoryCount) {
//                         int index = num - 1;
//                         int recyclePrice = (int)(playerData.inventory[index].price * 0.8);
//                         playerData.money += recyclePrice;
//                         for (int i = index; i < playerData.inventoryCount - 1; i++) {
//                             playerData.inventory[i] = playerData.inventory[i + 1];
//                         }
//                         playerData.inventoryCount--;
//                         printf("\n    %s? 回收成功！获得 $%d%s\n", COLOR_GREEN, recyclePrice, COLOR_RESET);
//                         Sleep(1500);
//                     }
//                 }
//                 else if (inputBuffer[0] == 'S' || inputBuffer[0] == 's') {
//                     int num = atoi(inputBuffer + 1);
//                     if (num > 0 && num <= playerData.inventoryCount) {
//                         int index = num - 1;
//                         Item& item = playerData.inventory[index];
                        
//                         if (item.type != "material") {
//                             printf("\n    %s? 只能存放变卖物（material）到安全箱！%s\n", COLOR_RED, COLOR_RESET);
//                             Sleep(1500);
//                         } else if (playerData.hasSafeBoxItem) {
//                             printf("\n    %s? 安全箱已满！请先取出物品。%s\n", COLOR_RED, COLOR_RESET);
//                             Sleep(1500);
//                         } else {
//                             playerData.safeBox = item;
//                             playerData.hasSafeBoxItem = true;
//                             for (int i = index; i < playerData.inventoryCount - 1; i++) {
//                                 playerData.inventory[i] = playerData.inventory[i + 1];
//                             }
//                             playerData.inventoryCount--;
//                             printf("\n    %s? 成功存入安全箱：%s（死亡不会掉落）%s\n", 
//                                    COLOR_GREEN, item.name.c_str(), COLOR_RESET);
//                             Sleep(1500);
//                         }
//                     }
//                 }
//             }
//         }
//         else if (choice == 3) {
//             system("cls");
//             showDeltaForceTitle();
//             printf("    %s═══════════════════════ 安全箱管理 ═══════════════════════%s\n\n", COLOR_GOLD, COLOR_RESET);
            
//             if (playerData.hasSafeBoxItem) {
//                 printf("    %s存放物品: %s%s%s\n", COLOR_WHITE, COLOR_GREEN, playerData.safeBox.name.c_str(), COLOR_RESET);
//                 printf("    %s物品价值: %s$%d%s\n", COLOR_WHITE, COLOR_YELLOW, playerData.safeBox.price, COLOR_RESET);
//                 printf("    %s回收价格: %s$%d (80%%)%s\n", COLOR_WHITE, COLOR_ORANGE, 
//                        (int)(playerData.safeBox.price * 0.8), COLOR_RESET);
//                 printf("    %s【安全箱内的物品死亡不会掉落】%s\n\n", COLOR_GREEN, COLOR_RESET);
                
//                 printf("    %s[1]%s 取出到背包\n", COLOR_YELLOW, COLOR_RESET);
//                 printf("    %s[2]%s 直接回收\n", COLOR_ORANGE, COLOR_RESET);
//                 printf("    %s[0]%s 返回\n\n", COLOR_RED, COLOR_RESET);
//                 printf("    请选择: ");
                
//                 int safeChoice = _getch() - '0';
                
//                 if (safeChoice == 1) {
//                     if (!playerData.hasBackpack) {
//                         printf("\n\n    %s你没有背包！无法取出物品。%s\n", COLOR_RED, COLOR_RESET);
//                         Sleep(1500);
//                     } else if (playerData.inventoryCount >= MAX_INVENTORY) {
//                         printf("\n\n    %s背包已满！%s\n", COLOR_RED, COLOR_RESET);
//                         Sleep(1500);
//                     } else {
//                         playerData.inventory[playerData.inventoryCount++] = playerData.safeBox;
//                         printf("\n\n    %s? 已取出到背包：%s（现在死亡会掉落）%s\n", 
//                                COLOR_GREEN, playerData.safeBox.name.c_str(), COLOR_RESET);
//                         playerData.hasSafeBoxItem = false;
//                         Sleep(1500);
//                     }
//                 } else if (safeChoice == 2) {
//                     int recyclePrice = (int)(playerData.safeBox.price * 0.8);
//                     playerData.money += recyclePrice;
//                     printf("\n\n    %s? 回收成功！获得 $%d%s\n", COLOR_GREEN, recyclePrice, COLOR_RESET);
//                     playerData.hasSafeBoxItem = false;
//                     Sleep(2000);
//                 }
//             } else {
//                 printf("    %s安全箱是空的。%s\n", COLOR_YELLOW, COLOR_RESET);
//                 printf("    %s只能存放变卖物（material类型）。%s\n", COLOR_WHITE, COLOR_RESET);
//                 printf("    %s【安全箱内的物品死亡不会掉落】%s\n\n", COLOR_GREEN, COLOR_RESET);
//                 printf("    按任意键返回...");
//                 _getch();
//             }
//         }
//         else if (choice == 4) {
//             showKeyCards();
//         }
//         else if (choice == 5) {
//             showBattlePass();
//         }
//         else if (choice == 6) {
//             showPassZones();
//         }
//     }
// }

// // ==================== 仓库管理 ====================
// void showWarehouse() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 仓库管理 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s资金: %s$%d%s\n", COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET);
//         printf("    %s仓库容量: %s%d/%d%s\n\n", COLOR_WHITE, COLOR_GREEN, 
//                playerData.warehouseCount, MAX_WAREHOUSE, COLOR_RESET);
        
//         if (playerData.warehouseCount == 0) {
//             printf("    %s仓库是空的...%s\n\n", COLOR_YELLOW, COLOR_RESET);
//         } else {
//             printf("    %s仓库物品列表：%s\n", COLOR_BOLD, COLOR_RESET);
//             printf("    %s──────────────────────────────────────%s\n", COLOR_SILVER, COLOR_RESET);
//             for (int i = 0; i < playerData.warehouseCount; i++) {
//                 Item& item = playerData.warehouse[i];
//                 printf("    [%d] %-20s $%-6d %s\n", 
//                        i + 1, item.name.c_str(), item.price, item.description.c_str());
//             }
//             printf("    %s──────────────────────────────────────%s\n\n", COLOR_SILVER, COLOR_RESET);
//         }
        
//         printf("    %s[W]%s 从背包转移到仓库\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[B]%s 从仓库转移到背包\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[R]%s 从仓库回收物品\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    %s[0]%s 返回主菜单\n", COLOR_RED, COLOR_RESET);
//         printf("\n    请选择操作: ");
        
//         char choice = _getch();
        
//         if (choice == '0') return;
//         else if (choice == 'W' || choice == 'w') {
//             if (playerData.inventoryCount == 0) {
//                 printf("\n\n    %s背包是空的！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//                 continue;
//             }
//             if (playerData.warehouseCount >= MAX_WAREHOUSE) {
//                 printf("\n\n    %s仓库已满！（最多30件）%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//                 continue;
//             }
            
//             system("cls");
//             showDeltaForceTitle();
//             printf("    %s═══════════════════════ 转移到仓库 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
//             printf("    %s背包物品：%s\n", COLOR_BOLD, COLOR_RESET);
//             for (int i = 0; i < playerData.inventoryCount; i++) {
//                 printf("    [%d] %s\n", i + 1, playerData.inventory[i].name.c_str());
//             }
//             printf("\n    输入编号（0取消）: ");
            
//             int num;
//             scanf("%d", &num);
//             getchar();
            
//             if (num > 0 && num <= playerData.inventoryCount) {
//                 int index = num - 1;
//                 playerData.warehouse[playerData.warehouseCount++] = playerData.inventory[index];
                
//                 for (int i = index; i < playerData.inventoryCount - 1; i++) {
//                     playerData.inventory[i] = playerData.inventory[i + 1];
//                 }
//                 playerData.inventoryCount--;
                
//                 printf("\n    %s? 成功转移到仓库！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == 'B' || choice == 'b') {
//             if (playerData.warehouseCount == 0) {
//                 printf("\n\n    %s仓库是空的！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//                 continue;
//             }
//             if (playerData.inventoryCount >= MAX_INVENTORY) {
//                 printf("\n\n    %s背包已满！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//                 continue;
//             }
            
//             system("cls");
//             showDeltaForceTitle();
//             printf("    %s═══════════════════════ 转移到背包 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
//             printf("    %s仓库物品：%s\n", COLOR_BOLD, COLOR_RESET);
//             for (int i = 0; i < playerData.warehouseCount; i++) {
//                 printf("    [%d] %s\n", i + 1, playerData.warehouse[i].name.c_str());
//             }
//             printf("\n    输入编号（0取消）: ");
            
//             int num;
//             scanf("%d", &num);
//             getchar();
            
//             if (num > 0 && num <= playerData.warehouseCount) {
//                 int index = num - 1;
//                 playerData.inventory[playerData.inventoryCount++] = playerData.warehouse[index];
                
//                 for (int i = index; i < playerData.warehouseCount - 1; i++) {
//                     playerData.warehouse[i] = playerData.warehouse[i + 1];
//                 }
//                 playerData.warehouseCount--;
                
//                 printf("\n    %s? 成功转移到背包！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == 'R' || choice == 'r') {
//             if (playerData.warehouseCount == 0) {
//                 printf("\n\n    %s仓库是空的！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//                 continue;
//             }
            
//             system("cls");
//             showDeltaForceTitle();
//             printf("    %s═══════════════════════ 回收物品 ═══════════════════════%s\n\n", COLOR_YELLOW, COLOR_RESET);
//             for (int i = 0; i < playerData.warehouseCount; i++) {
//                 Item& item = playerData.warehouse[i];
//                 printf("    [%d] %-20s 回收价: $%d\n", 
//                        i + 1, item.name.c_str(), (int)(item.price * 0.8));
//             }
//             printf("\n    输入编号（0取消）: ");
            
//             int num;
//             scanf("%d", &num);
//             getchar();
            
//             if (num > 0 && num <= playerData.warehouseCount) {
//                 int index = num - 1;
//                 int recyclePrice = (int)(playerData.warehouse[index].price * 0.8);
//                 playerData.money += recyclePrice;
                
//                 for (int i = index; i < playerData.warehouseCount - 1; i++) {
//                     playerData.warehouse[i] = playerData.warehouse[i + 1];
//                 }
//                 playerData.warehouseCount--;
                
//                 printf("\n    %s? 回收成功！获得 $%d%s\n", COLOR_GREEN, recyclePrice, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//     }
// }

// // ==================== 装备界面 ====================
// void showEquipment() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 装备界面 ═══════════════════════%s\n\n", COLOR_GOLD, COLOR_RESET);
        
//         printf("    %s当前装备：%s\n", COLOR_BOLD, COLOR_RESET);
//         printf("    %s──────────────────────────────────────%s\n", COLOR_SILVER, COLOR_RESET);
        
//         // 显示武器
//         if (playerData.equippedWeapon.name.empty()) {
//             printf("    %s[武器]%s 未装备\n", COLOR_RED, COLOR_RESET);
//         } else {
//             printf("    %s[武器]%s %s ($%d)\n", COLOR_GREEN, COLOR_RESET,
//                    playerData.equippedWeapon.name.c_str(), 
//                    playerData.equippedWeapon.price);
//             printf("           %s\n", playerData.equippedWeapon.description.c_str());
//         }
        
//         // 显示护甲
//         if (playerData.equippedArmor.name.empty()) {
//             printf("    %s[护甲]%s 未装备\n", COLOR_RED, COLOR_RESET);
//         } else {
//             printf("    %s[护甲]%s %s ($%d)\n", COLOR_GREEN, COLOR_RESET,
//                    playerData.equippedArmor.name.c_str(), 
//                    playerData.equippedArmor.price);
//             printf("           %s\n", playerData.equippedArmor.description.c_str());
//         }
        
//         // 显示胸挂
//         if (playerData.equippedChestRig.name.empty()) {
//             printf("    %s[胸挂]%s 未装备\n", COLOR_RED, COLOR_RESET);
//         } else {
//             printf("    %s[胸挂]%s %s (容量: %d) ($%d)\n", COLOR_GREEN, COLOR_RESET,
//                    playerData.equippedChestRig.name.c_str(),
//                    playerData.equippedChestRig.capacity,
//                    playerData.equippedChestRig.price);
//             printf("           %s\n", playerData.equippedChestRig.description.c_str());
//         }
        
//         // 显示背包
//         if (playerData.equippedBackpack.name.empty()) {
//             printf("    %s[背包]%s 未装备\n", COLOR_RED, COLOR_RESET);
//         } else {
//             printf("    %s[背包]%s %s (容量: %d) ($%d)\n", COLOR_GREEN, COLOR_RESET,
//                    playerData.equippedBackpack.name.c_str(),
//                    playerData.equippedBackpack.capacity,
//                    playerData.equippedBackpack.price);
//             printf("           %s\n", playerData.equippedBackpack.description.c_str());
//         }
        
//         printf("    %s──────────────────────────────────────%s\n\n", COLOR_SILVER, COLOR_RESET);
        
//         printf("    %s[1]%s 装备武器（从仓库）\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[2]%s 装备护甲（从仓库）\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[3]%s 装备胸挂（从仓库）\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[4]%s 装备背包（从仓库）\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[5]%s 卸下武器\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    %s[6]%s 卸下护甲\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    %s[7]%s 卸下胸挂\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    %s[8]%s 卸下背包\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    %s[0]%s 返回\n", COLOR_RED, COLOR_RESET);
//         printf("\n    请选择: ");
        
//         char choice = _getch();
        
//         if (choice == '0') return;
//         else if (choice == '1') {
//             // 装备武器
//             system("cls");
//             showDeltaForceTitle();
//             printf("    %s═══════════════════════ 选择武器 ═══════════════════════%s\n\n", COLOR_GOLD, COLOR_RESET);
            
//             vector<int> weaponIndices;
//             for (int i = 0; i < playerData.warehouseCount; i++) {
//                 if (playerData.warehouse[i].type == "weapon") {
//                     weaponIndices.push_back(i);
//                     printf("    [%d] %s\n", (int)weaponIndices.size(), 
//                            playerData.warehouse[i].name.c_str());
//                 }
//             }
            
//             if (weaponIndices.empty()) {
//                 printf("    %s仓库中没有武器！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//                 continue;
//             }
            
//             printf("\n    输入编号（0取消）: ");
//             int num;
//             scanf("%d", &num);
//             getchar();
            
//             if (num > 0 && num <= (int)weaponIndices.size()) {
//                 int index = weaponIndices[num - 1];
                
//                 // 如果已有装备武器，放回仓库
//                 if (!playerData.equippedWeapon.name.empty()) {
//                     if (playerData.warehouseCount < MAX_WAREHOUSE) {
//                         playerData.warehouse[playerData.warehouseCount++] = playerData.equippedWeapon;
//                     }
//                 }
                
//                 playerData.equippedWeapon = playerData.warehouse[index];
                
//                 // 从仓库移除
//                 for (int i = index; i < playerData.warehouseCount - 1; i++) {
//                     playerData.warehouse[i] = playerData.warehouse[i + 1];
//                 }
//                 playerData.warehouseCount--;
                
//                 printf("\n    %s✓ 装备成功！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == '2') {
//             // 装备护甲
//             system("cls");
//             showDeltaForceTitle();
//             printf("    %s═══════════════════════ 选择护甲 ═══════════════════════%s\n\n", COLOR_GOLD, COLOR_RESET);
            
//             vector<int> armorIndices;
//             for (int i = 0; i < playerData.warehouseCount; i++) {
//                 if (playerData.warehouse[i].type == "armor") {
//                     armorIndices.push_back(i);
//                     printf("    [%d] %s\n", (int)armorIndices.size(), 
//                            playerData.warehouse[i].name.c_str());
//                 }
//             }
            
//             if (armorIndices.empty()) {
//                 printf("    %s仓库中没有护甲！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//                 continue;
//             }
            
//             printf("\n    输入编号（0取消）: ");
//             int num;
//             scanf("%d", &num);
//             getchar();
            
//             if (num > 0 && num <= (int)armorIndices.size()) {
//                 int index = armorIndices[num - 1];
                
//                 // 如果已有装备护甲，放回仓库
//                 if (!playerData.equippedArmor.name.empty()) {
//                     if (playerData.warehouseCount < MAX_WAREHOUSE) {
//                         playerData.warehouse[playerData.warehouseCount++] = playerData.equippedArmor;
//                     }
//                 }
                
//                 playerData.equippedArmor = playerData.warehouse[index];
                
//                 // 从仓库移除
//                 for (int i = index; i < playerData.warehouseCount - 1; i++) {
//                     playerData.warehouse[i] = playerData.warehouse[i + 1];
//                 }
//                 playerData.warehouseCount--;
                
//                 printf("\n    %s✓ 装备成功！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == '3') {
//             // 装备胸挂
//             system("cls");
//             showDeltaForceTitle();
//             printf("    %s═══════════════════════ 选择胸挂 ═══════════════════════%s\n\n", COLOR_GOLD, COLOR_RESET);
            
//             vector<int> chestrigIndices;
//             for (int i = 0; i < playerData.warehouseCount; i++) {
//                 if (playerData.warehouse[i].type == "chestrig") {
//                     chestrigIndices.push_back(i);
//                     printf("    [%d] %s (容量: %d)\n", (int)chestrigIndices.size(), 
//                            playerData.warehouse[i].name.c_str(),
//                            playerData.warehouse[i].capacity);
//                 }
//             }
            
//             if (chestrigIndices.empty()) {
//                 printf("    %s仓库中没有胸挂！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//                 continue;
//             }
            
//             printf("\n    输入编号（0取消）: ");
//             int num;
//             scanf("%d", &num);
//             getchar();
            
//             if (num > 0 && num <= (int)chestrigIndices.size()) {
//                 int index = chestrigIndices[num - 1];
                
//                 // 如果已有装备胸挂，放回仓库
//                 if (!playerData.equippedChestRig.name.empty()) {
//                     if (playerData.warehouseCount < MAX_WAREHOUSE) {
//                         playerData.warehouse[playerData.warehouseCount++] = playerData.equippedChestRig;
//                     }
//                 }
                
//                 playerData.equippedChestRig = playerData.warehouse[index];
                
//                 // 从仓库移除
//                 for (int i = index; i < playerData.warehouseCount - 1; i++) {
//                     playerData.warehouse[i] = playerData.warehouse[i + 1];
//                 }
//                 playerData.warehouseCount--;
                
//                 printf("\n    %s✓ 装备成功！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == '4') {
//             // 装备背包
//             system("cls");
//             showDeltaForceTitle();
//             printf("    %s═══════════════════════ 选择背包 ═══════════════════════%s\n\n", COLOR_GOLD, COLOR_RESET);
            
//             vector<int> backpackIndices;
//             for (int i = 0; i < playerData.warehouseCount; i++) {
//                 if (playerData.warehouse[i].type == "backpack") {
//                     backpackIndices.push_back(i);
//                     printf("    [%d] %s (容量: %d)\n", (int)backpackIndices.size(), 
//                            playerData.warehouse[i].name.c_str(),
//                            playerData.warehouse[i].capacity);
//                 }
//             }
            
//             if (backpackIndices.empty()) {
//                 printf("    %s仓库中没有背包！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//                 continue;
//             }
            
//             printf("\n    输入编号（0取消）: ");
//             int num;
//             scanf("%d", &num);
//             getchar();
            
//             if (num > 0 && num <= (int)backpackIndices.size()) {
//                 int index = backpackIndices[num - 1];
                
//                 // 如果已有装备背包，放回仓库
//                 if (!playerData.equippedBackpack.name.empty()) {
//                     if (playerData.warehouseCount < MAX_WAREHOUSE) {
//                         playerData.warehouse[playerData.warehouseCount++] = playerData.equippedBackpack;
//                     }
//                 }
                
//                 playerData.equippedBackpack = playerData.warehouse[index];
                
//                 // 从仓库移除
//                 for (int i = index; i < playerData.warehouseCount - 1; i++) {
//                     playerData.warehouse[i] = playerData.warehouse[i + 1];
//                 }
//                 playerData.warehouseCount--;
                
//                 printf("\n    %s✓ 装备成功！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == '5') {
//             // 卸下武器
//             if (playerData.equippedWeapon.name.empty()) {
//                 printf("\n\n    %s未装备武器！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             } else if (playerData.warehouseCount >= MAX_WAREHOUSE) {
//                 printf("\n\n    %s仓库已满！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             } else {
//                 playerData.warehouse[playerData.warehouseCount++] = playerData.equippedWeapon;
//                 playerData.equippedWeapon = {"", 0, 0, "", "", 0};
//                 printf("\n\n    %s✓ 卸下武器成功！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == '6') {
//             // 卸下护甲
//             if (playerData.equippedArmor.name.empty()) {
//                 printf("\n\n    %s未装备护甲！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             } else if (playerData.warehouseCount >= MAX_WAREHOUSE) {
//                 printf("\n\n    %s仓库已满！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             } else {
//                 playerData.warehouse[playerData.warehouseCount++] = playerData.equippedArmor;
//                 playerData.equippedArmor = {"", 0, 0, "", "", 0};
//                 printf("\n\n    %s✓ 卸下护甲成功！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == '7') {
//             // 卸下胸挂
//             if (playerData.equippedChestRig.name.empty()) {
//                 printf("\n\n    %s未装备胸挂！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             } else if (playerData.warehouseCount >= MAX_WAREHOUSE) {
//                 printf("\n\n    %s仓库已满！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             } else {
//                 playerData.warehouse[playerData.warehouseCount++] = playerData.equippedChestRig;
//                 playerData.equippedChestRig = {"", 0, 0, "", "", 0};
//                 printf("\n\n    %s✓ 卸下胸挂成功！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//         else if (choice == '8') {
//             // 卸下背包
//             if (playerData.equippedBackpack.name.empty()) {
//                 printf("\n\n    %s未装备背包！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             } else if (playerData.warehouseCount >= MAX_WAREHOUSE) {
//                 printf("\n\n    %s仓库已满！%s\n", COLOR_RED, COLOR_RESET);
//                 Sleep(1500);
//             } else {
//                 playerData.warehouse[playerData.warehouseCount++] = playerData.equippedBackpack;
//                 playerData.equippedBackpack = {"", 0, 0, "", "", 0};
//                 printf("\n\n    %s✓ 卸下背包成功！%s\n", COLOR_GREEN, COLOR_RESET);
//                 Sleep(1500);
//             }
//         }
//     }
// }

// // ==================== 局内背包查看 ====================
// void showInGameInventory() {
//     system("cls");
//     showDeltaForceTitle();
//     printf("    %s═══════════════════════ 局内背包 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
    
//     printf("    %s当前装备：%s\n", COLOR_BOLD, COLOR_RESET);
//     printf("    %s──────────────────────────────────────%s\n", COLOR_SILVER, COLOR_RESET);
//     printf("    %s[武器]%s %s\n", COLOR_GREEN, COLOR_RESET,
//            playerData.equippedWeapon.name.empty() ? "未装备" : playerData.equippedWeapon.name.c_str());
//     printf("    %s[护甲]%s %s\n", COLOR_GREEN, COLOR_RESET,
//            playerData.equippedArmor.name.empty() ? "未装备" : playerData.equippedArmor.name.c_str());
//     printf("    %s──────────────────────────────────────%s\n\n", COLOR_SILVER, COLOR_RESET);
    
//     printf("    %s背包物品 (%d/%d)：%s\n", COLOR_BOLD, playerData.inventoryCount, MAX_INVENTORY, COLOR_RESET);
//     if (playerData.inventoryCount == 0) {
//         printf("    %s背包是空的...%s\n", COLOR_YELLOW, COLOR_RESET);
//     } else {
//         for (int i = 0; i < playerData.inventoryCount; i++) {
//             printf("    [%d] %-20s $%-6d\n", 
//                    i + 1, playerData.inventory[i].name.c_str(), 
//                    playerData.inventory[i].price);
//         }
//     }
    
//     printf("\n    按任意键继续...");
//     _getch();
// }

// // ==================== 局内存储到安全箱 ====================
// void storeItemInSafe() {
//     if (playerData.inventoryCount == 0) {
//         printf("\n    %s背包是空的！%s\n", COLOR_RED, COLOR_RESET);
//         Sleep(1500);
//         return;
//     }
    
//     if (playerData.hasSafeBoxItem) {
//         printf("\n    %s安全箱已满！只能存放1件物品。%s\n", COLOR_RED, COLOR_RESET);
//         Sleep(1500);
//         return;
//     }
    
//     system("cls");
//     showDeltaForceTitle();
//     printf("    %s═══════════════════════ 存入安全箱 ═══════════════════════%s\n\n", COLOR_GOLD, COLOR_RESET);
//     printf("    %s只能存放材料类物品（material）到安全箱%s\n\n", COLOR_YELLOW, COLOR_RESET);
    
//     vector<int> materialIndices;
//     for (int i = 0; i < playerData.inventoryCount; i++) {
//         if (playerData.inventory[i].type == "material") {
//             materialIndices.push_back(i);
//             printf("    [%d] %-20s $%-6d\n", 
//                    (int)materialIndices.size(), 
//                    playerData.inventory[i].name.c_str(),
//                    playerData.inventory[i].price);
//         }
//     }
    
//     if (materialIndices.empty()) {
//         printf("\n    %s背包中没有可存放的材料！%s\n", COLOR_RED, COLOR_RESET);
//         Sleep(1500);
//         return;
//     }
    
//     printf("\n    输入编号（0取消）: ");
//     int num;
//     scanf("%d", &num);
//     getchar();
    
//     if (num > 0 && num <= (int)materialIndices.size()) {
//         int index = materialIndices[num - 1];
        
//         playerData.safeBox = playerData.inventory[index];
//         playerData.hasSafeBoxItem = true;
        
//         // 从背包移除
//         for (int i = index; i < playerData.inventoryCount - 1; i++) {
//             playerData.inventory[i] = playerData.inventory[i + 1];
//         }
//         playerData.inventoryCount--;
        
//         printf("\n    %s? 成功存入安全箱：%s（死亡不会掉落）%s\n", 
//                COLOR_GREEN, playerData.safeBox.name.c_str(), COLOR_RESET);
//         Sleep(2000);
//     }
// }

// // ==================== 存档系统 ====================
// void saveGame() {
//     ofstream file("savegame.dat");
//     if (!file.is_open()) {
//         printf("\n\n    %s保存失败！无法创建存档文件。%s\n\n", COLOR_RED, COLOR_RESET);
//         Sleep(2000);
//         return;
//     }
    
//     file << "[DeltaForce_Save]\n";
//     file << "Version=1.0\n";
//     file << "SelectedCharacter=" << g_selectedCharacter << "\n";
//     file << "Money=" << playerData.money << "\n";
//     file << "InventoryCount=" << playerData.inventoryCount << "\n";
//     file << "HasBackpack=" << playerData.hasBackpack << "\n";
//     file << "HasSafeBoxItem=" << playerData.hasSafeBoxItem << "\n";
//     file << "KeyCardCount=" << playerData.keyCards.size() << "\n";
//     file << "PassType=" << playerData.battlePass.type << "\n";
//     file << "PassLevel=" << playerData.battlePass.level << "\n";
//     file << "PassExp=" << playerData.battlePass.exp << "\n";
    
//     file << "[PassClaimed]\n";
//     for (int lvl : playerData.battlePass.claimedLevels) {
//         file << "Claimed=" << lvl << "\n";
//     }
    
//     file << "[Inventory]\n";
//     for (int i = 0; i < playerData.inventoryCount; i++) {
//         file << "Item=" 
//              << playerData.inventory[i].name << "|" 
//              << playerData.inventory[i].price << "|" 
//              << playerData.inventory[i].rarity << "|" 
//              << playerData.inventory[i].type << "\n";
//     }
    
//     if (playerData.hasSafeBoxItem) {
//         file << "[SafeBox]\n";
//         file << "Item=" 
//              << playerData.safeBox.name << "|" 
//              << playerData.safeBox.price << "|" 
//              << playerData.safeBox.rarity << "|" 
//              << playerData.safeBox.type << "\n";
//     }
    
//     file << "[KeyCards]\n";
//     for (size_t i = 0; i < playerData.keyCards.size(); i++) {
//         const KeyCard& card = playerData.keyCards[i];
//         file << "Card=" << card.type << "|" << card.uses << "|"
//              << card.code[0] << "|" << card.code[1] << "|" 
//              << card.code[2] << "|" << card.code[3] << "\n";
//     }
    
//     file.close();
    
//     printf("\n\n");
//     printf("    %s╔════════════════════════════════════════╗%s\n", COLOR_GREEN, COLOR_RESET);
//     printf("    %s║          游戏进度已保存              ║%s\n", COLOR_GREEN, COLOR_RESET);
//     printf("    %s╚════════════════════════════════════════╝%s\n\n", COLOR_GREEN, COLOR_RESET);
//     printf("    %s干员: %s%s%s\n", COLOR_WHITE, COLOR_CYAN, characters[g_selectedCharacter].name.c_str(), COLOR_RESET);
//     printf("    %s资金: %s$%d%s\n", COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET);
//     printf("    %s物品: %s%d件%s\n", COLOR_WHITE, COLOR_GREEN, playerData.inventoryCount, COLOR_RESET);
//     printf("    %s背包: %s%s%s\n", COLOR_WHITE, 
//            playerData.hasBackpack ? COLOR_GREEN : COLOR_RED,
//            playerData.hasBackpack ? "已拥有" : "未购买", COLOR_RESET);
//     printf("    %s安全箱: %s%s%s\n", COLOR_WHITE, 
//            playerData.hasSafeBoxItem ? COLOR_GREEN : COLOR_YELLOW,
//            playerData.hasSafeBoxItem ? "已存放物品" : "空", COLOR_RESET);
//     printf("    %s钥匙卡: %s%d张%s\n", COLOR_WHITE, COLOR_GREEN, (int)playerData.keyCards.size(), COLOR_RESET);
//     printf("    %s通行证: %s%s%s 等级 %d%s\n", 
//            COLOR_WHITE,
//            playerData.battlePass.getColor().c_str(),
//            playerData.battlePass.getName().c_str(),
//            COLOR_RESET,
//            playerData.battlePass.level, COLOR_RESET);
//     Sleep(2000);
// }

// void loadGame() {
//     ifstream file("savegame.dat");
//     if (!file.is_open()) {
//         printf("\n\n    %s未找到存档文件！%s\n\n", COLOR_RED, COLOR_RESET);
//         Sleep(2000);
//         return;
//     }
    
//     string line;
//     string section;
    
//     playerData.money = 1000;
//     playerData.inventoryCount = 0;
//     playerData.hasBackpack = false;
//     playerData.hasSafeBoxItem = false;
//     playerData.keyCards.clear();
//     playerData.battlePass = {PASS_NONE, "无通行证", 1, 0, {}};
//     g_selectedCharacter = 0;
    
//     while (getline(file, line)) {
//         if (line.empty()) continue;
        
//         if (line == "[DeltaForce_Save]") {
//             section = "save";
//             continue;
//         } else if (line == "[PassClaimed]") {
//             section = "passclaimed";
//             continue;
//         } else if (line == "[Inventory]") {
//             section = "inventory";
//             continue;
//         } else if (line == "[SafeBox]") {
//             section = "safebox";
//             continue;
//         } else if (line == "[KeyCards]") {
//             section = "keycards";
//             continue;
//         }
        
//         size_t eq_pos = line.find('=');
//         if (eq_pos == string::npos) continue;
        
//         string key = line.substr(0, eq_pos);
//         string value = line.substr(eq_pos + 1);
        
//         if (section == "save") {
//             if (key == "SelectedCharacter") {
//                 g_selectedCharacter = atoi(value.c_str());
//             }
//             else if (key == "Money") {
//                 playerData.money = atoi(value.c_str());
//             }
//             else if (key == "InventoryCount") {
//                 playerData.inventoryCount = atoi(value.c_str());
//             }
//             else if (key == "HasBackpack") {
//                 playerData.hasBackpack = (value == "1" || value == "true");
//             }
//             else if (key == "HasSafeBoxItem") {
//                 playerData.hasSafeBoxItem = (value == "1" || value == "true");
//             }
//             else if (key == "PassType") {
//                 playerData.battlePass.type = (PassType)atoi(value.c_str());
//             }
//             else if (key == "PassLevel") {
//                 playerData.battlePass.level = atoi(value.c_str());
//             }
//             else if (key == "PassExp") {
//                 playerData.battlePass.exp = atoi(value.c_str());
//             }
//         }
//         else if (section == "passclaimed" && key == "Claimed") {
//             playerData.battlePass.claimedLevels.push_back(atoi(value.c_str()));
//         }
//         else if (section == "inventory" && key == "Item") {
//             size_t pos1 = value.find('|');
//             size_t pos2 = value.find('|', pos1 + 1);
//             size_t pos3 = value.find('|', pos2 + 1);
            
//             if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
//                 string name = value.substr(0, pos1);
//                 int price = atoi(value.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
//                 int rarity = atoi(value.substr(pos2 + 1, pos3 - pos2 - 1).c_str());
//                 string type = value.substr(pos3 + 1);
                
//                 if (playerData.inventoryCount < MAX_INVENTORY) {
//                     playerData.inventory[playerData.inventoryCount].name = name;
//                     playerData.inventory[playerData.inventoryCount].price = price;
//                     playerData.inventory[playerData.inventoryCount].rarity = rarity;
//                     playerData.inventory[playerData.inventoryCount].type = type;
//                     playerData.inventoryCount++;
//                 }
//             }
//         }
//         else if (section == "safebox" && key == "Item") {
//             size_t pos1 = value.find('|');
//             size_t pos2 = value.find('|', pos1 + 1);
//             size_t pos3 = value.find('|', pos2 + 1);
            
//             if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
//                 playerData.safeBox.name = value.substr(0, pos1);
//                 playerData.safeBox.price = atoi(value.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
//                 playerData.safeBox.rarity = atoi(value.substr(pos2 + 1, pos3 - pos2 - 1).c_str());
//                 playerData.safeBox.type = value.substr(pos3 + 1);
//                 playerData.hasSafeBoxItem = true;
//             }
//         }
//         else if (section == "keycards" && key == "Card") {
//             size_t pos1 = value.find('|');
//             size_t pos2 = value.find('|', pos1 + 1);
//             size_t pos3 = value.find('|', pos2 + 1);
//             size_t pos4 = value.find('|', pos3 + 1);
//             size_t pos5 = value.find('|', pos4 + 1);
            
//             if (pos1 != string::npos) {
//                 KeyCard card;
//                 card.type = (KeyCardType)atoi(value.substr(0, pos1).c_str());
//                 card.uses = atoi(value.substr(pos1 + 1, pos2 - pos1 - 1).c_str());
                
//                 for (int i = 0; i < KEY_CARDS_COUNT; i++) {
//                     if (KEY_CARDS[i].type == card.type) {
//                         card.name = KEY_CARDS[i].name;
//                         card.description = KEY_CARDS[i].description;
//                         card.isHacked = KEY_CARDS[i].isHacked;
//                         break;
//                     }
//                 }
                
//                 if (pos3 != string::npos) {
//                     card.code[0] = atoi(value.substr(pos2 + 1, pos3 - pos2 - 1).c_str());
//                     if (pos4 != string::npos) {
//                         card.code[1] = atoi(value.substr(pos3 + 1, pos4 - pos3 - 1).c_str());
//                         if (pos5 != string::npos) {
//                             card.code[2] = atoi(value.substr(pos4 + 1, pos5 - pos4 - 1).c_str());
//                             card.code[3] = atoi(value.substr(pos5 + 1).c_str());
//                         }
//                     }
//                 }
                
//                 playerData.keyCards.push_back(card);
//             }
//         }
//     }
    
//     file.close();
    
//     // 更新通行证名称
//     playerData.battlePass.name = playerData.battlePass.getName();
    
//     if (g_selectedCharacter < 0 || g_selectedCharacter >= 14) {
//         g_selectedCharacter = 0;
//     }
    
//     printf("\n\n");
//     printf("    %s╔════════════════════════════════════════╗%s\n", COLOR_GREEN, COLOR_RESET);
//     printf("    %s║          游戏进度已加载              ║%s\n", COLOR_GREEN, COLOR_RESET);
//     printf("    %s╚════════════════════════════════════════╝%s\n\n", COLOR_GREEN, COLOR_RESET);
//     printf("    %s干员: %s%s%s\n", COLOR_WHITE, COLOR_CYAN, characters[g_selectedCharacter].name.c_str(), COLOR_RESET);
//     printf("    %s资金: %s$%d%s\n", COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET);
//     printf("    %s物品: %s%d件%s\n", COLOR_WHITE, COLOR_GREEN, playerData.inventoryCount, COLOR_RESET);
//     printf("    %s背包: %s%s%s\n", COLOR_WHITE, 
//            playerData.hasBackpack ? COLOR_GREEN : COLOR_RED,
//            playerData.hasBackpack ? "已拥有" : "未购买", COLOR_RESET);
//     printf("    %s安全箱: %s%s%s\n", COLOR_WHITE, 
//            playerData.hasSafeBoxItem ? COLOR_GREEN : COLOR_YELLOW,
//            playerData.hasSafeBoxItem ? "已存放物品" : "空", COLOR_RESET);
//     printf("    %s钥匙卡: %s%d张%s\n", COLOR_WHITE, COLOR_GREEN, (int)playerData.keyCards.size(), COLOR_RESET);
//     printf("    %s通行证: %s%s%s 等级 %d%s\n", 
//            COLOR_WHITE,
//            playerData.battlePass.getColor().c_str(),
//            playerData.battlePass.getName().c_str(),
//            COLOR_RESET,
//            playerData.battlePass.level, COLOR_RESET);
//     Sleep(2000);
// }

// // ==================== 操作说明 ====================
// void showInstructions() {
//     system("cls");
//     showDeltaForceTitle();
//     printf("    %s═══════════════════════ 操作说明 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
    
//     printf("    %s【任务目标】%s\n", COLOR_GOLD, COLOR_RESET);
//     printf("      到达撤离点并生存！\n\n");
    
//     printf("    %s【操作说明】%s\n", COLOR_GOLD, COLOR_RESET);
//     printf("      %sWASD键%s   - 移动角色\n", COLOR_YELLOW, COLOR_RESET);
//     printf("      %s方向键%s   - 转换朝向\n", COLOR_YELLOW, COLOR_RESET);
//     printf("      %s空格键%s   - 射击（需购买武器和弹药）\n", COLOR_YELLOW, COLOR_RESET);
//     printf("      %sQ/E键%s    - 使用技能\n", COLOR_YELLOW, COLOR_RESET);
//     printf("      %sF键%s      - 打开保险箱/拾取盒子/使用钥匙卡/进入通行证区域\n", COLOR_YELLOW, COLOR_RESET);
//     printf("      %sR键%s      - 换弹\n", COLOR_YELLOW, COLOR_RESET);
//     printf("      %sH键%s      - 使用药品\n", COLOR_YELLOW, COLOR_RESET);
//     printf("      %sI/B键%s    - 查看背包\n", COLOR_YELLOW, COLOR_RESET);
//     printf("      %sM键%s      - 查看地图\n\n", COLOR_YELLOW, COLOR_RESET);
    
//     printf("    %s【重要提示】%s\n", COLOR_GOLD, COLOR_RESET);
//     printf("      %s??%s 开局没有任何武器，请先在市场购买！\n", COLOR_RED, COLOR_RESET);
//     printf("      %s??%s 放入安全箱的物品死亡不会掉落！\n", COLOR_GREEN, COLOR_RESET);
//     printf("      %s??%s 钥匙卡可开启特殊区域和高级保险箱\n", COLOR_YELLOW, COLOR_RESET);
//     printf("      %s??%s 通行证可获得专属奖励和特殊区域\n", COLOR_GOLD, COLOR_RESET);
//     printf("      %s??%s 多种撤离方式，每种都有独特动画\n\n", COLOR_CYAN, COLOR_RESET);
    
//     printf("    按任意键返回主菜单...");
//     _getch();
// }

// // ==================== 特勤处 ====================
// void showSecretService() {
//     while (true) {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 特勤处 - 行动中心 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
        
//         printf("    %s欢迎来到特勤处！%s\n\n", COLOR_GOLD, COLOR_RESET);
        
//         printf("    %s【可用服务】%s\n", COLOR_GREEN, COLOR_RESET);
//         printf("    %s[1]%s 情报收集\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    %s[2]%s 通行证商店\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    %s[3]%s 钥匙卡图鉴\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    %s[0]%s 返回大厅\n\n", COLOR_RED, COLOR_RESET);
//         printf("    请选择: ");
        
//         int choice = _getch() - '0';
        
//         if (choice == 1) {
//             system("cls");
//             showDeltaForceTitle();
//             printf("    %s═══════════════════════ 情报收集中心 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
            
//             printf("    %s【零号大坝】%s\n", COLOR_GREEN, COLOR_RESET);
//             printf("    - 首领赛伊德血量800，建议使用狙击枪\n");
//             printf("    - 员工通道卡可开启核心区捷径\n");
//             printf("    - 行政楼拉闸撤离需坚守3分钟\n\n");
            
//             printf("    %s【航天基地】%s\n", COLOR_CYAN, COLOR_RESET);
//             printf("    - 双首领德穆兰(800血)+哈德森(600血)\n");
//             printf("    - 黑室密钥密码每局随机生成\n");
//             printf("    - 火箭撤离需完成飞升者行动\n\n");
            
//             printf("    %s【通行证信息】%s\n", COLOR_GOLD, COLOR_RESET);
//             printf("    - 基础通行证 ($500)：基础奖励\n");
//             printf("    - 战斗通行证 ($1500)：战斗奖励\n");
//             printf("    - 高级通行证 ($3000)：高级奖励\n");
//             printf("    - 精英通行证 ($5000)：精英奖励\n\n");
            
//             printf("    %s【钥匙卡信息】%s\n", COLOR_MAGENTA, COLOR_RESET);
//             for (int i = 1; i < KEY_CARDS_COUNT; i++) {
//                 printf("    - %s：%s\n", KEY_CARDS[i].name.c_str(), KEY_CARDS[i].description.c_str());
//             }
//             printf("\n");
            
//             printf("    按任意键返回...");
//             _getch();
//         }
//         else if (choice == 2) {
//             showBattlePass();
//         }
//         else if (choice == 3) {
//             showKeyCards();
//         }
//         else if (choice == 0) {
//             return;
//         }
//     }
// }

// // ==================== 行前备战 ====================
// void showPreBattlePreparation(MapType map) {
//     system("cls");
//     Character& ch = characters[g_selectedCharacter];
    
//     showDeltaForceTitle();
//     printf("    %s═══════════════════════ 行前备战 ═══════════════════════%s\n\n", COLOR_YELLOW, COLOR_RESET);
    
//     printf("    %s┌─────────────── 当前干员 ───────────────┐%s\n", COLOR_CYAN, COLOR_RESET);
//     printf("    %s│  代号: %s%s%s\n", COLOR_CYAN, COLOR_GREEN, ch.name.c_str(), COLOR_RESET);
//     printf("    %s│  生命: %s%d%s | 能量: %s%d%s\n", 
//            COLOR_CYAN, COLOR_RED, ch.maxHp, COLOR_RESET, COLOR_BLUE, ch.baseEnergy, COLOR_RESET);
//     printf("    %s│  技能1: %s%s%s (消耗:%s%d%s)\n", 
//            COLOR_CYAN, COLOR_YELLOW, ch.skill1Name.c_str(), COLOR_RESET, COLOR_RED, ch.skill1Cost, COLOR_RESET);
//     printf("    %s│  技能2: %s%s%s (消耗:%s%d%s)\n", 
//            COLOR_CYAN, COLOR_YELLOW, ch.skill2Name.c_str(), COLOR_RESET, COLOR_RED, ch.skill2Cost, COLOR_RESET);
//     printf("    %s└─────────────────────────────────────────┘%s\n\n", COLOR_CYAN, COLOR_RESET);
    
//     printf("    %s┌─────────────── 资源状态 ───────────────┐%s\n", COLOR_CYAN, COLOR_RESET);
//     printf("    %s│  资金: %s$%d%s\n", COLOR_CYAN, COLOR_YELLOW, playerData.money, COLOR_RESET);
//     printf("    %s│  背包: %s%s%s\n", COLOR_CYAN, 
//            playerData.hasBackpack ? COLOR_GREEN : COLOR_RED,
//            playerData.hasBackpack ? "已拥有" : "未购买", COLOR_RESET);
//     printf("    %s│  物品数量: %s%d/%d%s\n", 
//            COLOR_CYAN, COLOR_GREEN, playerData.inventoryCount, MAX_INVENTORY, COLOR_RESET);
//     printf("    %s│  安全箱: %s%s%s\n", COLOR_CYAN, 
//            playerData.hasSafeBoxItem ? COLOR_GREEN : COLOR_YELLOW,
//            playerData.hasSafeBoxItem ? "已存放物品" : "空", COLOR_RESET);
//     printf("    %s│  钥匙卡: %s%d张%s\n", COLOR_CYAN, COLOR_GREEN, (int)playerData.keyCards.size(), COLOR_RESET);
//     printf("    %s│  通行证: %s%s%s 等级 %d%s\n", 
//            COLOR_CYAN,
//            playerData.battlePass.getColor().c_str(),
//            playerData.battlePass.getName().c_str(),
//            COLOR_RESET,
//            playerData.battlePass.level, COLOR_RESET);
//     printf("    %s└─────────────────────────────────────────┘%s\n\n", COLOR_CYAN, COLOR_RESET);
    
//     printf("    %s┌─────────────── 任务简报 ───────────────┐%s\n", COLOR_CYAN, COLOR_RESET);
//     if (map == MAP_ZERO_DAM) {
//         printf("    %s│  地图: 零号大坝%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│  地点: 阿萨拉半岛 · 乌姆河%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│  目标: 搜集高价值物资%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│  首领: 赛伊德 (800血)%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│  撤离: 长弓溪谷/水下管道/行政楼拉闸%s\n", COLOR_CYAN, COLOR_RESET);
//     } else {
//         printf("    %s│  地图: 航天基地%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│  地点: 发射中心 · 轨道区%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│  目标: 夺取航天核心模块%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│  首领: 德穆兰(800血)+哈德森(600血)%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s│  撤离: 直升机平台/火箭发射/载具撤离%s\n", COLOR_CYAN, COLOR_RESET);
//     }
//     printf("    %s└─────────────────────────────────────────┘%s\n\n", COLOR_CYAN, COLOR_RESET);
    
//     printf("\n  按任意键返回主菜单...");
//     _getch();
// }

// // ==================== 游戏引擎类 - 简化版 ====================
// class GameEngine {
// public:
//     Unit p;
//     int playerDir;
//     Unit extractionPoint;
//     int extractionTimer;
//     bool missionComplete;
//     int bulletDirX[4];
//     int bulletDirY[4];
    
//     int energy;
//     int armorValue;
//     int currentWeapon;
//     int primaryAmmo;
//     int secondaryAmmo;
    
//     int selectedChar;
//     int difficulty;
//     Character* currentChar;
    
//     bool viewingMap;
//     MapType mapType;
    
//     HANDLE hConsole;
    
//     vector<KeyCard> playerKeyCards;
//     ExtractionPoint currentExtraction;
    
//     GameEngine(int charIndex, int diff, MapType map) {
//         selectedChar = charIndex;
//         difficulty = diff;
//         currentChar = &characters[selectedChar];
//         mapType = map;
        
//         hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//         srand((unsigned)time(0));
        
//         p = {25, 45, true, currentChar->maxHp};
//         energy = currentChar->baseEnergy;
        
//         armorValue = 0;
        
//         bool hasWeapon = false;
//         for (int i = 0; i < playerData.inventoryCount; i++) {
//             if (playerData.inventory[i].type == "weapon") {
//                 hasWeapon = true;
//                 break;
//             }
//         }
        
//         currentWeapon = hasWeapon ? 0 : -1;
//         primaryAmmo = hasWeapon ? 30 : 0;
//         secondaryAmmo = hasWeapon ? 15 : 0;
        
//         bulletDirX[0] = 0; bulletDirX[1] = 1; bulletDirX[2] = 0; bulletDirX[3] = -1;
//         bulletDirY[0] = -1; bulletDirY[1] = 0; bulletDirY[2] = 1; bulletDirY[3] = 0;
        
//         extractionTimer = 0;
//         missionComplete = false;
        
//         viewingMap = false;
        
//         playerKeyCards = playerData.keyCards;
        
//         if (mapType == MAP_ZERO_DAM) {
//             currentExtraction = EXTRACTION_POINTS[0];
//         } else {
//             currentExtraction = EXTRACTION_POINTS[3];
//         }
//         extractionPoint = {currentExtraction.x, currentExtraction.y, true, 0};
//     }
    
//     void draw() {
//         CONSOLE_CURSOR_INFO cursorInfo;
//         GetConsoleCursorInfo(hConsole, &cursorInfo);
//         cursorInfo.bVisible = FALSE;
//         SetConsoleCursorInfo(hConsole, &cursorInfo);
        
//         if (viewingMap) {
//             displayMap();
//             return;
//         }
        
//         system("cls");
//         showDeltaForceTitle();
        
//         printf("    %s==========================================%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("     %s角色:%s %s | %s难度:%s %s | %s地图:%s %s\n", 
//                 COLOR_GREEN, COLOR_RESET, currentChar->name.c_str(),
//                 COLOR_YELLOW, COLOR_RESET,
//                 difficulty == 0 ? "简单" : difficulty == 1 ? "普通" : "困难",
//                 COLOR_MAGENTA, COLOR_RESET,
//                 mapType == MAP_ZERO_DAM ? "零号大坝" : "航天基地");
//         printf("     %s生命:%s %d/%d | %s能量:%s %d/%d\n", 
//                 COLOR_RED, COLOR_RESET, p.hp, currentChar->maxHp,
//                 COLOR_BLUE, COLOR_RESET, energy, currentChar->baseEnergy);
        
//         if (currentWeapon == -1) {
//             printf("     %s??  没有武器！请去市场购买！%s\n", COLOR_RED, COLOR_RESET);
//         } else {
//             printf("     %s弹药:%s 主武器 %d/30 | 副武器 %d/15\n", 
//                     COLOR_YELLOW, COLOR_RESET, primaryAmmo, secondaryAmmo);
//         }
//         printf("    %s==========================================%s\n", COLOR_CYAN, COLOR_RESET);
        
//         printf("\n    当前位置: (%d, %d)\n", p.x, p.y);
//         printf("    撤离点: (%d, %d)\n\n", extractionPoint.x, extractionPoint.y);
        
//         printf("    %s──────────────────────────────────────────%s\n", COLOR_SILVER, COLOR_RESET);
//         printf("     %sW/A/S/D:%s移动  %s空格:%s射击  %sF:%s交互\n", 
//                 COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET);
//         printf("     %sI:%s背包  %sM:%s地图  %sQ/E:%s技能  %sH:%s治疗\n", 
//                 COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET, COLOR_YELLOW, COLOR_RESET);
//     }
    
//     void displayMap() {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 当前地图 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
//         printf("\n    当前位置: (%d, %d)\n", p.x, p.y);
//         printf("    撤离点: (%d, %d)\n\n", extractionPoint.x, extractionPoint.y);
//         printf("\n    %s图例: @你  ▲撤离点%s\n", COLOR_WHITE, COLOR_RESET);
//         printf("\n按任意键返回...");
//         _getch();
//         viewingMap = false;
//     }
    
//     void shoot() {
//         if (currentWeapon == -1) {
//             printf("\n    %s? 你没有武器！%s\n", COLOR_RED, COLOR_RESET);
//             Sleep(500);
//             return;
//         }
        
//         if (currentWeapon == 0 && primaryAmmo <= 0) {
//             printf("\n    %s? 主武器弹药不足！按R换弹%s\n", COLOR_RED, COLOR_RESET);
//             Sleep(500);
//             return;
//         }
//         if (currentWeapon == 1 && secondaryAmmo <= 0) {
//             printf("\n    %s? 副武器弹药不足！按R换弹%s\n", COLOR_RED, COLOR_RESET);
//             Sleep(500);
//             return;
//         }
        
//         if (currentWeapon == 0) primaryAmmo--;
//         if (currentWeapon == 1) secondaryAmmo--;
        
//         printf("\n    %s?? 开枪射击！%s\n", COLOR_YELLOW, COLOR_RESET);
//         Sleep(200);
//     }
    
//     void useKeyCard() {
//         for (int i = 0; i < KEY_DOORS_COUNT; i++) {
//             if (abs(p.x - KEY_DOORS[i].x) <= 2 && abs(p.y - KEY_DOORS[i].y) <= 2) {
//                 if (!KEY_DOORS[i].isLocked) {
//                     printf("\n    %s?? 门已经开了%s\n", COLOR_YELLOW, COLOR_RESET);
//                     return;
//                 }
                
//                 for (size_t j = 0; j < playerKeyCards.size(); j++) {
//                     if (playerKeyCards[j].type == KEY_DOORS[i].requiredCard) {
//                         printf("\n    %s?? 使用 %s 开门！%s\n", COLOR_GREEN, playerKeyCards[j].name.c_str(), COLOR_RESET);
//                         playerKeyCards[j].uses--;
                        
//                         p.x = KEY_DOORS[i].targetX;
//                         p.y = KEY_DOORS[i].targetY;
                        
//                         printf("    %s已进入内部区域！%s\n", COLOR_GREEN, COLOR_RESET);
                        
//                         if (playerKeyCards[j].uses <= 0) {
//                             playerKeyCards.erase(playerKeyCards.begin() + j);
//                         }
//                         Sleep(1500);
//                         return;
//                     }
//                 }
//                 printf("\n    %s? 需要 %s 才能开门%s\n", COLOR_RED, KEY_CARDS[KEY_DOORS[i].requiredCard].name.c_str(), COLOR_RESET);
//                 Sleep(1000);
//                 return;
//             }
//         }
//     }
    
//     void usePassZone() {
//         for (int i = 0; i < PASS_ZONES_COUNT; i++) {
//             if (abs(p.x - PASS_ZONES[i].x) <= 2 && abs(p.y - PASS_ZONES[i].y) <= 2) {
//                 if (!PASS_ZONES[i].isLocked) {
//                     printf("\n    %s?? 区域已开放%s\n", COLOR_YELLOW, COLOR_RESET);
//                     return;
//                 }
                
//                 if (playerData.battlePass.type >= PASS_ZONES[i].requiredPass && 
//                     playerData.battlePass.level >= PASS_ZONES[i].requiredLevel) {
//                     printf("\n    %s?? 使用通行证进入 %s%s\n", COLOR_GREEN, PASS_ZONES[i].name.c_str(), COLOR_RESET);
                    
//                     p.x = PASS_ZONES[i].targetX;
//                     p.y = PASS_ZONES[i].targetY;
                    
//                     printf("    %s已进入通行证区域！%s\n", COLOR_GREEN, COLOR_RESET);
                    
//                     Sleep(1500);
//                     return;
//                 } else if (playerData.battlePass.type < PASS_ZONES[i].requiredPass) {
//                     printf("\n    %s? 需要 %s 才能进入%s\n", 
//                            COLOR_RED, 
//                            PASS_ZONES[i].requiredPass == PASS_BASIC ? "基础通行证" :
//                            PASS_ZONES[i].requiredPass == PASS_BATTLE ? "战斗通行证" :
//                            PASS_ZONES[i].requiredPass == PASS_PREMIUM ? "高级通行证" : "精英通行证", 
//                            COLOR_RESET);
//                 } else {
//                     printf("\n    %s? 需要通行证等级 %d 才能进入（当前等级 %d）%s\n", 
//                            COLOR_RED, 
//                            PASS_ZONES[i].requiredLevel, 
//                            playerData.battlePass.level, 
//                            COLOR_RESET);
//                 }
//                 Sleep(1000);
//                 return;
//             }
//         }
//     }
    
//     void update() {
//         if (_kbhit()) {
//             int k = _getch();
            
//             if (k == 224 || k == 0) {
//                 k = _getch();
//                 if (k == 72) playerDir = 0;
//                 else if (k == 77) playerDir = 1;
//                 else if (k == 80) playerDir = 2;
//                 else if (k == 75) playerDir = 3;
//                 else if (k == 59) currentWeapon = 0;
//                 else if (k == 60) currentWeapon = 1;
//                 else if (k == 61) currentWeapon = 2;
//                 return;
//             }
            
//             if (k == 'w' || k == 'W') {
//                 p.y--;
//             }
//             else if (k == 's' || k == 'S') {
//                 p.y++;
//             }
//             else if (k == 'a' || k == 'A') {
//                 p.x--;
//             }
//             else if (k == 'd' || k == 'D') {
//                 p.x++;
//             }
//             else if (k == ' ') {
//                 shoot();
//             }
//             else if (k == 'f' || k == 'F') {
//                 useKeyCard();
//                 usePassZone();
//             }
//             else if (k == 'm' || k == 'M') {
//                 viewingMap = !viewingMap;
//             }
//             else if (k == 'r' || k == 'R') {
//                 if (currentWeapon == 0) primaryAmmo = 30;
//                 else if (currentWeapon == 1) secondaryAmmo = 15;
//                 printf("\n    %s?? 换弹中...%s\n", COLOR_YELLOW, COLOR_RESET);
//                 Sleep(500);
//             }
//             else if (k == 'i' || k == 'I' || k == 'b' || k == 'B') {
//                 showInventoryInGame();
//             }
//             else if (k == 'e' || k == 'E') {
//                 showInGameInventory();
//             }
//             else if (k == 'x' || k == 'X') {
//                 storeItemInSafe();
//             }
//         }
        
//         p.x = max(1, min(M_SIZE - 2, p.x));
//         p.y = max(1, min(M_SIZE - 2, p.y));
        
//         if (p.x == extractionPoint.x && p.y == extractionPoint.y) {
//             if (extractionTimer == 0) extractionTimer = 33;
//             extractionTimer--;
//             if (extractionTimer <= 0) {
//                 missionComplete = true;
//                 playExtractionAnimation();
//             }
//         } else {
//             extractionTimer = 0;
//         }
//     }
    
//     void showInventoryInGame() {
//         system("cls");
//         showDeltaForceTitle();
//         printf("    %s═══════════════════════ 背包物品 ═══════════════════════%s\n\n", COLOR_CYAN, COLOR_RESET);
        
//         printf("    %s资金: %s$%d%s\n", COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET);
//         printf("    %s背包空间: %s%d/%d%s\n\n", COLOR_WHITE, COLOR_GREEN, playerData.inventoryCount, MAX_INVENTORY, COLOR_RESET);
        
//         if (playerData.inventoryCount == 0) {
//             printf("    %s背包是空的...%s\n", COLOR_YELLOW, COLOR_RESET);
//         } else {
//             for (int i = 0; i < playerData.inventoryCount; i++) {
//                 printf("    %s[%d]%s %s - %s$%d%s\n", 
//                        COLOR_YELLOW, i+1, COLOR_RESET, 
//                        playerData.inventory[i].name.c_str(),
//                        COLOR_GREEN, playerData.inventory[i].price, COLOR_RESET);
//             }
//         }
        
//         printf("\n    %s钥匙卡: %s%d张%s\n", COLOR_WHITE, COLOR_GREEN, (int)playerKeyCards.size(), COLOR_RESET);
//         for (size_t i = 0; i < playerKeyCards.size(); i++) {
//             printf("        %s%s%s (剩余%d次)\n", 
//                    playerKeyCards[i].getColor().c_str(), 
//                    playerKeyCards[i].name.c_str(), 
//                    COLOR_RESET, 
//                    playerKeyCards[i].uses);
//         }
        
//         printf("\n    %s通行证: %s%s%s 等级 %d%s\n", 
//                COLOR_WHITE, 
//                playerData.battlePass.getColor().c_str(),
//                playerData.battlePass.getName().c_str(),
//                COLOR_RESET,
//                playerData.battlePass.level, COLOR_RESET);
        
//         printf("\n    %s按任意键返回...%s", COLOR_WHITE, COLOR_RESET);
//         _getch();
//     }
    
//     void playExtractionAnimation() {
//         system("cls");
//         printf("\n\n");
//         printf("    %s╔════════════════════════════════════════════════════════════════╗%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s║                    ?? 撤离行动 - 直升机抵达                   ║%s\n", COLOR_CYAN, COLOR_RESET);
//         printf("    %s╚════════════════════════════════════════════════════════════════╝%s\n\n", COLOR_CYAN, COLOR_RESET);
        
//         printf("    %s[22:00]%s 你掏出信号棒，用力拉下引信。\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    ‘嗤——’ 耀眼的红色火光冲天而起，照亮了夜空。\n\n");
//         Sleep(1500);
        
//         printf("    %s[22:02]%s 远处传来微弱的“突突”声，那是旋翼切割空气的声音。\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    声音越来越大，越来越近……\n\n");
//         Sleep(1500);
        
//         printf("    %s[22:04]%s 一架 MH-60“黑鹰”直升机出现在视野中，卷起巨大的风沙。\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    舱门边的机枪手警惕地环视四周，向你打出“快速登机”的手势。\n");
//         printf("    你弯下腰，顶着强风向直升机跑去。\n\n");
//         Sleep(1500);
        
//         printf("    %s[22:05]%s 你抓住舱门边缘，被机务员一把拉进机舱。\n", COLOR_YELLOW, COLOR_RESET);
//         printf("    “走！快走！” 驾驶员吼道。\n");
//         printf("    直升机猛然拉升，看着脚下越来越小的撤离点，你长舒了一口气。\n\n");
//         Sleep(1500);
        
//         playerData.battlePass.exp += 50;
//         printf("    %s? 撤离成功！通行证经验+50%s\n", COLOR_GREEN, COLOR_RESET);
//     }
    
//     void start() {
//         system("cls");
//         printf("\n\n");
//         printf("    %s========================================%s\n", COLOR_RED, COLOR_RESET);
//         printf("    %s||          行动开始                 ||%s\n", COLOR_RED, COLOR_RESET);
//         printf("    %s========================================%s\n", COLOR_RED, COLOR_RESET);
//         printf("\n    %s干员: %s%s%s\n", COLOR_WHITE, COLOR_GREEN, currentChar->name.c_str(), COLOR_RESET);
//         printf("    %s地点: %s%s%s\n", COLOR_WHITE, COLOR_CYAN, 
//                mapType == MAP_ZERO_DAM ? "零号大坝" : "航天基地", COLOR_RESET);
//         printf("    %s难度: %s%s%s\n", COLOR_WHITE, COLOR_YELLOW, 
//                difficulty == 0 ? "简单" : difficulty == 1 ? "普通" : "困难", COLOR_RESET);
//         printf("\n    %s按任意键进入战场...%s", COLOR_GREEN, COLOR_RESET);
//         _getch();
        
//         while (p.hp > 0 && !missionComplete) {
//             draw();
//             update();
//             Sleep(100);
//         }
        
//         system("cls");
//         printf("\n\n");
//         if (p.hp <= 0) {
//             printf("    %s========================================%s\n", COLOR_RED, COLOR_RESET);
//             printf("    %s||          任务失败！               ||%s\n", COLOR_RED, COLOR_RESET);
//             printf("    %s========================================%s\n", COLOR_RED, COLOR_RESET);
//             printf("\n    你阵亡了...\n");
//         } else {
//             printf("    %s========================================%s\n", COLOR_GREEN, COLOR_RESET);
//             printf("    %s||        撤离成功！                 ||%s\n", COLOR_GREEN, COLOR_RESET);
//             printf("    %s========================================%s\n", COLOR_GREEN, COLOR_RESET);
//             printf("\n    %s干员: %s%s%s\n", COLOR_WHITE, COLOR_GREEN, currentChar->name.c_str(), COLOR_RESET);
//             printf("    %s存活生命值: %s%d%s\n", COLOR_WHITE, COLOR_RED, p.hp, COLOR_RESET);
//         }
        
//         printf("\n    %s按任意键返回大厅...%s", COLOR_WHITE, COLOR_RESET);
//         _getch();
//     }
// };

// // ==================== 主菜单 ====================
// void showMainMenu() {
//     system("cls");
//     Character& ch = characters[g_selectedCharacter];
    
//     bool saveExists = false;
//     ifstream file("savegame.dat");
//     if (file.is_open()) {
//         saveExists = true;
//         file.close();
//     }
    
//     showDeltaForceTitle();
    
//     const char* backpackStatus = playerData.hasBackpack ? "已拥有" : "未购买";
//     printf("    %s资金: %s$%d%s | 背包: %s%s%s | 物品: %s%d/%d%s\n", 
//            COLOR_WHITE, COLOR_YELLOW, playerData.money, COLOR_RESET,
//            playerData.hasBackpack ? COLOR_GREEN : COLOR_RED,
//            backpackStatus, COLOR_RESET,
//            COLOR_GREEN, playerData.inventoryCount, MAX_INVENTORY, COLOR_RESET);
//     printf("    %s安全箱: %s%s%s | 钥匙卡: %s%d张%s | 通行证: %s%s%s Lv.%d%s\n\n", 
//            COLOR_WHITE,
//            playerData.hasSafeBoxItem ? COLOR_GREEN : COLOR_YELLOW,
//            playerData.hasSafeBoxItem ? ("已存放: " + playerData.safeBox.name).c_str() : "空",
//            COLOR_RESET,
//            COLOR_GREEN, (int)playerData.keyCards.size(), COLOR_RESET,
//            playerData.battlePass.getColor().c_str(),
//            playerData.battlePass.getName().c_str(),
//            COLOR_RESET,
//            playerData.battlePass.level, COLOR_RESET);
    
//     printf("    %s──────────────────────────────────────────────────────────────%s\n\n", COLOR_SILVER, COLOR_RESET);
//     printf("                  %s╭────╗%s\n", COLOR_RED, COLOR_RESET);
//     printf("                  %s│  %c  │%s\n", COLOR_RED, ch.icon, COLOR_RESET);
//     printf("                  %s╰────╝%s\n", COLOR_RED, COLOR_RESET);
//     printf("\n");
//     printf("        %s[1]%s 零号大坝     %s[5]%s 市场商店     %s[W]%s 仓库管理\n", COLOR_GREEN, COLOR_RESET, COLOR_GREEN, COLOR_RESET, COLOR_GREEN, COLOR_RESET);
//     printf("        %s[2]%s 航天基地     %s[6]%s 交易行       %s[E]%s 装备界面\n", COLOR_GREEN, COLOR_RESET, COLOR_GREEN, COLOR_RESET, COLOR_GREEN, COLOR_RESET);
//     printf("        %s[3]%s 行前备战     %s[7]%s 背包管理     %s[9]%s 保存进度\n", COLOR_GREEN, COLOR_RESET, COLOR_GREEN, COLOR_RESET, COLOR_GREEN, COLOR_RESET);
//     printf("        %s[4]%s 操作说明     %s[T]%s 特勤处       %s[8]%s 退出游戏\n", COLOR_GREEN, COLOR_RESET, COLOR_GREEN, COLOR_RESET, COLOR_RED, COLOR_RESET);
//     printf("        %s[M]%s 联机模式     %s[0]%s 加载进度%s\n\n", 
//            COLOR_GREEN, COLOR_RESET, COLOR_GREEN, COLOR_RESET,
//            saveExists ? " (有存档)" : " (无存档)");
//     printf("    %s当前干员: %s%s%s | %sHP:%d EN:%d%s\n", 
//            COLOR_WHITE, COLOR_CYAN, ch.name.c_str(), COLOR_RESET,
//            COLOR_RED, ch.maxHp, ch.baseEnergy, COLOR_RESET);
//     printf("    %s──────────────────────────────────────────────────────────────%s\n", COLOR_SILVER, COLOR_RESET);
//     printf("    %s请选择: %s", COLOR_YELLOW, COLOR_RESET);
// }

// // ==================== 主函数 ====================
// int main() {
//     SetConsoleOutputCP(65001);
//     SetConsoleCP(65001);
//     system("chcp 65001 > nul");
    
//     // 手动定义 ENABLE_VIRTUAL_TERMINAL_PROCESSING 以防未定义
//     #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
//     #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
//     #endif
    
//     HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
//     DWORD dwMode = 0;
//     GetConsoleMode(hOut, &dwMode);
//     dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
//     SetConsoleMode(hOut, dwMode);
    
//     SetConsoleTitle("三角洲行动 - 钥匙卡+通行证完全分离版");
//     system("mode con cols=110 lines=40");
    
//     srand((unsigned int)time(NULL));
    
//     g_selectedCharacter = 0;
//     g_lanGame = nullptr;
//     currentMap = MAP_ZERO_DAM;
    
//     while (true) {
//         showMainMenu();
        
//         int k = _getch();
//         system("cls");
        
//         if (k == '1' || k == '2') {
//             int charIndex = showCharacterSelection();
//             if (charIndex == -1) continue;
            
//             int diff = 1;
//             int mapType = (k == '1') ? 0 : 1;
//             currentMap = (k == '1') ? MAP_ZERO_DAM : MAP_SPACE_BASE;
            
//             system("cls");
//             printf("\n    %s选择难度:%s\n", COLOR_YELLOW, COLOR_RESET);
//             printf("    %s[1]%s 简单\n", COLOR_GREEN, COLOR_RESET);
//             printf("    %s[2]%s 普通\n", COLOR_GREEN, COLOR_RESET);
//             printf("    %s[3]%s 困难\n\n", COLOR_GREEN, COLOR_RESET);
//             printf("    %s请选择: %s", COLOR_YELLOW, COLOR_RESET);
//             int diffChoice = _getch() - '0';
//             if (diffChoice == 1) diff = 0;
//             else if (diffChoice == 3) diff = 2;
//             else diff = 1;
            
//             system("cls");
//             printf("\n    %s开始%s行动...%s\n", 
//                    COLOR_GREEN,
//                    mapType == 0 ? "零号大坝" : "航天基地",
//                    COLOR_RESET);
//             printf("    %s干员: %s%s%s\n", COLOR_WHITE, COLOR_CYAN, characters[charIndex].name.c_str(), COLOR_RESET);
//             printf("    %s难度: %s%s%s\n\n", COLOR_WHITE, COLOR_YELLOW,
//                    diff == 0 ? "简单" : diff == 1 ? "普通" : "困难", COLOR_RESET);
            
//             bool hasWeapon = false;
//             for (int i = 0; i < playerData.inventoryCount; i++) {
//                 if (playerData.inventory[i].type == "weapon") {
//                     hasWeapon = true;
//                     break;
//                 }
//             }
            
//             if (!hasWeapon) {
//                 printf("    %s??  警告：你还没有购买任何武器！%s\n", COLOR_RED, COLOR_RESET);
//                 printf("    建议先去市场购买武器和弹药再开始行动。\n\n");
//                 printf("    %s按 [Y] 继续行动，按其他键返回主菜单...%s", COLOR_YELLOW, COLOR_RESET);
//                 int choice = _getch();
//                 if (choice != 'y' && choice != 'Y') {
//                     continue;
//                 }
//             } else {
//                 printf("    %s按任意键进入战场...%s", COLOR_GREEN, COLOR_RESET);
//                 _getch();
//             }
            
//             GameEngine g(charIndex, diff, mapType == 0 ? MAP_ZERO_DAM : MAP_SPACE_BASE);
//             g.start();
//         }
//         else if (k == '3') {
//             showPreBattlePreparation(currentMap);
//         }
//         else if (k == '4') {
//             showInstructions();
//         }
//         else if (k == '5') {
//             showMarket();
//         }
//         else if (k == '6') {
//             showTradingPost();
//         }
//         else if (k == '7') {
//             showInventory();
//         }
//         else if (k == '8') {
//             printf("\n\n    %s感谢游玩三角洲行动！%s\n\n", COLOR_GOLD, COLOR_RESET);
//             break;
//         }
//         else if (k == '9') {
//             saveGame();
//         }
//         else if (k == '0') {
//             loadGame();
//         }
//         else if (k == 't' || k == 'T') {
//             showSecretService();
//         }
//         else if (k == 'm' || k == 'M') {
//             showMultiplayerMenu();
//         }
//         else if (k == 'w' || k == 'W') {
//             showWarehouse();
//         }
//         else if (k == 'e' || k == 'E') {
//             showEquipment();
//         }
//     }
    
//     if (g_lanGame) {
//         g_lanGame->disconnect();
//         delete g_lanGame;
//         g_lanGame = nullptr;
//     }
    
//     return 0;
// }
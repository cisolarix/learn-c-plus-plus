#include <iostream>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <string>

using namespace std;

// 增大地图规模，增加游戏复杂度
const int M_SIZE = 20;
const int E_COUNT = 5;  // 减少敌人数量
const int OBSTACLE_COUNT = 30;
const int MAX_SAFES = 3; // 保险箱数量

struct Unit {
    int x, y;
    bool live;
    int hp;
};

// 物品结构
struct Item {
    string name;
    int price;
    int rarity; // 0=普通 1=稀有 2=史诗 3=传说
    string type; // weapon, med, material
};

// 保险箱结构
struct Safe {
    int x, y;
    bool locked;
    int code[4]; // 4位密码
    int currentInput[4];
    int inputPos;
    Item reward;
    bool opened;
};

// 全局玩家数据
struct PlayerData {
    int money;
    Item inventory[50];
    int inventoryCount;
} playerData = {1000, {}, 0};

// 角色属性结构
struct Character {
    string name;
    int maxHp;
    int baseEnergy;
    int skill1Cost;
    int skill2Cost;
    string skill1Name;
    string skill2Name;
    string description;
    char icon;  // 角色外观图标
};

// 14个角色定义（三角洲行动特种兵）
Character characters[14] = {
    {"突击兵", 150, 100, 30, 50, "闪光弹", "冲击手雷", "高生命突击手，火力压制", '@'},
    {"侦察兵", 90, 120, 25, 45, "侦察无人机", "沉默步伐", "快速移动，信息获取", 'W'},
    {"破门手", 100, 100, 40, 60, "破门炸药", "战术盾牌", "近战爆破专家，强攻突破", 'K'},
    {"医疗兵", 80, 110, 20, 50, "医疗包", "肾上腺素", "战地医疗，生命恢复", '+'},
    {"轻装兵", 85, 130, 35, 55, "烟雾弹", "快速部署", "极速机动，战术撤离", '>'},
    {"支援兵", 110, 90, 40, 70, "弹药补给", "防御工事", "后勤支援，阵地防守", 'Y'},
    {"工程兵", 95, 100, 30, 60, "地雷陷阱", "电磁脉冲", "工程爆破，区域控制", 'G'},
    {"黑客", 100, 115, 45, 65, "电子干扰", "系统入侵", "信息战专家，技术破坏", 'H'},
    {"重装兵", 120, 95, 35, 55, "防爆盾牌", "重型护甲", "防护专家，前线坦克", 'D'},
    {"通讯兵", 90, 140, 25, 40, "战术标记", "卫星扫描", "通讯支持，情报共享", 'L'},
    {"潜入者", 85, 105, 30, 50, "钩锁绳索", "消音武器", "隐蔽渗透，垂直机动", '&'},
    {"幽灵", 100, 100, 40, 60, "光学伪装", "热源抑制", "隐身专家，无声击杀", '?'},
    {"狂战士", 130, 80, 50, 70, "狂暴模式", "近战强化", "高攻低防，近战之王", 'U'},
    {"神射手", 95, 110, 35, 55, "稳定瞄准", "穿甲弹", "精准狙击，远程输出", 'A'}
};

// 函数前向声明
int getMouseClick();

class GameEngine {
    Unit p = {2, 2, true, 100};
    Unit enemies[E_COUNT];
    bool enemyAlert[E_COUNT];
    int enemyType[E_COUNT];  // 敌人类型 0=普通 1=精英 2=BOSS
    Unit obstacles[OBSTACLE_COUNT];
    Unit bullet = {0, 0, false, 0};
    Unit bullets[5];
    int playerDir = 0;     // 玩家朝向 0=上 1=右 2=下 3=左
    Unit extractionPoint;  // 撤离点
    int extractionTimer = 0; // 撤离计时器（需要停留5秒=33帧）
    bool missionComplete = false; // 任务完成标志
    int bulletDirX[4] = {0, 1, 0, -1};  // 子弹X方向
    int bulletDirY[4] = {-1, 0, 1, 0};  // 子弹Y方向
    
    // 技能系统
    int stealthTimer = 0;
    int berserkTimer = 0;
    int energy = 100;
    int shieldHp = 0;      // 护盾值（深蓝专属）
    int poisonTimer[E_COUNT]; // 中毒计时器（蛊专属）
    bool canFly = false;   // 飞行状态（蝶专属）
    
    // 武器系统
    int currentWeapon = 0; // 0=主武器 1=副武器 2=刀
    int primaryAmmo = 30;   // 主武器弹药
    int secondaryAmmo = 15; // 副武器弹药
    int reloadTimer = 0;    // 换弹计时器1
    int skill1Cooldown = 0; // 技能1冷却
    int skill2Cooldown = 0; // 技能2冷却
    
    // 角色和难度
    int selectedChar = 11;
    int difficulty = 1;
    Character* currentChar;
    int score = 0;          // 积分系统
    
    // 保险箱系统
    Safe safes[MAX_SAFES];
    Safe* currentSafe = nullptr; // 当前正在解码的保险箱
    bool isHacking = false;      // 是否正在解码

public:
    GameEngine(int charIndex, int diff) {
        selectedChar = charIndex;
        difficulty = diff;
        currentChar = &characters[charIndex];
        
        SetConsoleOutputCP(65001);
        srand((unsigned) time(0));

        // 根据角色设置初始属性
        p.hp = currentChar->maxHp;
        energy = currentChar->baseEnergy;
        
        // 初始化
        for(int i=0; i<E_COUNT; i++) poisonTimer[i] = 0;
        for(int i=0; i<5; i++) bullets[i] = {0, 0, false, 0};
        
        // 设置撤离点在地图右上角
        extractionPoint = {M_SIZE - 3, 2, true, 0};
        extractionTimer = 0;
        missionComplete = false;

        // 根据难度生成敌人
        int enemyHp = 30 + difficulty * 20;
        for (int i = 0; i < E_COUNT; i++) {
            enemies[i] = {rand() % (M_SIZE - 4) + 2, rand() % (M_SIZE / 2) + 2, true, enemyHp};
            enemyAlert[i] = false;
            // 随机生成敌人类型
            int r = rand() % 100;
            if(r < 10) enemyType[i] = 2;      // 10% BOSS
            else if(r < 35) enemyType[i] = 1; // 25% 精英
            else enemyType[i] = 0;             // 65% 普通
            
            // BOSS更强
            if(enemyType[i] == 2) enemies[i].hp *= 2;
            else if(enemyType[i] == 1) enemies[i].hp = (int)(enemies[i].hp * 1.5);
        }
        
        // 根据难度生成障碍物
        int obstacleCount = OBSTACLE_COUNT - difficulty * 8;
        for (int i = 0; i < obstacleCount; i++) {
            obstacles[i] = {rand() % (M_SIZE - 2) + 1, rand() % (M_SIZE - 2) + 1, true, 0};
        }
        for (int i = obstacleCount; i < OBSTACLE_COUNT; i++) {
            obstacles[i].live = false;
        }
        
        // 生成保险箱
        Item possibleRewards[20] = {
            {"AWM狙击步枪", 1200, 3, "weapon"},
            {"SCAR-H突击步枪", 900, 2, "weapon"},
            {"黄金条", 850, 2, "material"},
            {"情报硬盘", 1100, 3, "material"},
            {"现金包", 650, 2, "material"},
            {"军用医疗包", 400, 1, "med"},
            {"战术头盔", 500, 2, "armor"},
            {"夜视仪", 720, 2, "equipment"},
            {".308狙击弹药", 350, 1, "ammo"},
            {"GPS定位器", 480, 1, "equipment"},
            {"钻石项链", 1500, 3, "material"},
            {"古董花瓶", 1200, 3, "material"},
            {"军用笔记本电脑", 950, 2, "material"},
            {"名贵手表", 800, 2, "material"},
            {"艺术画作", 1100, 3, "material"},
            {"稀有邮票", 700, 2, "material"},
            {"白磷手雷", 600, 2, "equipment"},
            {"热成像仪", 1300, 3, "equipment"},
            {"便携式无人机", 950, 2, "equipment"},
            {"三级防弹插板", 800, 2, "armor"}
        };
        
        for(int i = 0; i < MAX_SAFES; i++) {
            safes[i].x = rand() % (M_SIZE - 4) + 2;
            safes[i].y = rand() % (M_SIZE - 4) + 2;
            safes[i].locked = true;
            safes[i].opened = false;
            safes[i].inputPos = 0;
            // 生成4位随机密码
            for(int j = 0; j < 4; j++) {
                safes[i].code[j] = rand() % 10;
                safes[i].currentInput[j] = -1;
            }
            safes[i].reward = possibleRewards[rand() % 20];
        }
        isHacking = false;
        currentSafe = nullptr;
    }

    void draw() {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});

        printf("==========================================\n");
        printf(" 角色: %s | 难度: %s\n", currentChar->name.c_str(), 
               difficulty==0?"简单":difficulty==1?"普通":"困难");
        printf(" 生命: %-3d", p.hp);
        if(shieldHp > 0) printf("(+%d盾) ", shieldHp);
        printf("| 能量: %-3d", energy);
        if(extractionTimer > 0) printf(" | 撤离中: %.1f秒", extractionTimer/6.67);
        printf("\n");
        printf(" 状态: ");
        if(stealthTimer > 0) printf("[%s %d] ", currentChar->skill1Name.c_str(), stealthTimer/5);
        if(berserkTimer > 0) printf("[%s %d] ", currentChar->skill2Name.c_str(), berserkTimer/5);
        if(canFly) printf("[飞行] ");
        if(stealthTimer <=0 && berserkTimer <=0 && !canFly) printf("正常");
        printf("\n==========================================\n");

        for (int y = 0; y < M_SIZE; y++) {
            for (int x = 0; x < M_SIZE; x++) {
                if (x == p.x && y == p.y) {
                    // 根据状态显示角色外观
                    char icon = currentChar->icon;
                    
                    if(shieldHp > 0) {
                        // 护盾状态：括号包裹
                        printf("[%c]", icon);
                    } else if(berserkTimer > 0) {
                        // 狂暴状态：双星号
                        printf("*%c*", icon);
                    } else if(stealthTimer > 0) {
                        // 隐身状态：小写+点
                        printf(".%c.", (char)tolower(icon));
                    } else if(canFly) {
                        // 飞行状态：翼膀
                        printf("^%c^", icon);
                    } else {
                        printf("%c ", icon);
                    }
                }
                else {
                    bool hit = false;
                    
                    // 检查敌人（根据类型和状态显示）
                    for (int i = 0; i < E_COUNT; i++)
                        if (enemies[i].live && enemies[i].x == x && enemies[i].y == y) {
                            // 中毒状态
                            if(poisonTimer[i] > 0) {
                                printf("~ ");
                            }
                            // 警觉状态
                            else if(enemyAlert[i]) {
                                if(enemyType[i] == 2) printf("M ");      // BOSS警觉
                                else if(enemyType[i] == 1) printf("E "); // 精英警觉
                                else printf("! ");                        // 普通警觉
                            }
                            // 平静状态
                            else {
                                if(enemyType[i] == 2) printf("X ");      // BOSS
                                else if(enemyType[i] == 1) printf("e "); // 精英
                                else printf("o ");                        // 普通
                            }
                            hit = true;
                            break;
                        }
                    
                    // 检查撤离点
                    if (!hit && extractionPoint.x == x && extractionPoint.y == y) {
                        printf("◎");
                        hit = true;
                    }
                    
                    // 检查障碍物
                    if (!hit) {
                        for (int i = 0; i < OBSTACLE_COUNT; i++)
                            if (obstacles[i].live && obstacles[i].x == x && obstacles[i].y == y) {
                                printf("# ");
                                hit = true;
                                break;
                            }
                    }
                    
                    // 检查保险箱
                    if (!hit) {
                        for (int i = 0; i < MAX_SAFES; i++) {
                            if (safes[i].x == x && safes[i].y == y && !safes[i].opened) {
                                if (safes[i].locked) {
                                    printf("$ "); // 上锁的保险箱
                                } else {
                                    printf("☆"); // 已解锁未开启
                                }
                                hit = true;
                                break;
                            }
                        }
                    }
                    
                    // 子弹已改为即时命中，不再显示飞行中的子弹
                    
                    if (!hit) {
                        if (y == 0 || y == M_SIZE - 1 || x == 0 || x == M_SIZE - 1) printf("= ");
                        else printf(". ");
                    }
                }
            }
            printf("\n");
        }
        
        // 底部UI：技能和武器栏
        printf("\n========================================\n");
        printf(" 技能栏: ");
        
        // 技能1状态
        if(skill1Cooldown > 0) {
            printf("[Q:%s CD:%.1fs] ", currentChar->skill1Name.c_str(), skill1Cooldown/10.0);
        } else if(energy >= currentChar->skill1Cost) {
            printf("[Q:%s ★就绪] ", currentChar->skill1Name.c_str());
        } else {
            printf("[Q:%s 能量不足] ", currentChar->skill1Name.c_str());
        }
        
        // 技能2状态
        if(skill2Cooldown > 0) {
            printf("[E:%s CD:%.1fs]\n", currentChar->skill2Name.c_str(), skill2Cooldown/10.0);
        } else if(energy >= currentChar->skill2Cost) {
            printf("[E:%s ★就绪]\n", currentChar->skill2Name.c_str());
        } else {
            printf("[E:%s 能量不足]\n", currentChar->skill2Name.c_str());
        }
        
        printf(" 武器栏: ");
        
        // 主武器
        if(currentWeapon == 0) printf("►►");
        printf("[F1:主武器 %d/30] ", primaryAmmo);
        
        // 副武器
        if(currentWeapon == 1) printf("►►");
        printf("[F2:副武器 %d/15] ", secondaryAmmo);
        
        // 刀
        if(currentWeapon == 2) printf("►►");
        printf("[F3:近战刀 ∞]\n");
        
        // 换弹提示
        if(reloadTimer > 0) {
            printf(" 正在换弹... %.1fs\n", reloadTimer/10.0);
        }
        
        printf("========================================\n");
        printf("方向键:转向 | 空格:射击 | R:换弹 | F:交互\n");
        
        // 解码界面
        if (isHacking && currentSafe != nullptr) {
            printf("\n╔════════════════════════════════════╗\n");
            printf("║     保险箱解码系统 - 输入密码     ║\n");
            printf("╚════════════════════════════════════╝\n");
            printf("  密码提示: [");
            for (int i = 0; i < 4; i++) {
                if (currentSafe->currentInput[i] >= 0) {
                    printf("%d", currentSafe->currentInput[i]);
                } else {
                    printf("_");
                }
                if (i < 3) printf(" ");
            }
            printf("]\n");
            printf("  输入数字 0-9 | ESC 取消\n");
            
            // 显示线索（仅部分数字）
            printf("  线索: 第1位是 %d\n", currentSafe->code[0]);
        }
    }

    bool isObstacle(int x, int y) {
        for (int i = 0; i < OBSTACLE_COUNT; i++)
            if (obstacles[i].live && obstacles[i].x == x && obstacles[i].y == y)
                return true;
        return false;
    }
    
    // 检查玩家是否在九宫格范围内
    bool inAlertRange(int ex, int ey) {
        return abs(ex - p.x) <= 1 && abs(ey - p.y) <= 1;
    }
    
    void update() {
        // 冷却计时递减
        if(skill1Cooldown > 0) skill1Cooldown--;
        if(skill2Cooldown > 0) skill2Cooldown--;
        if(reloadTimer > 0) reloadTimer--;
        
        if (_kbhit()) {
            int k = _getch();
            
            // 处理方向键和F键（224是扩展键前缀）
            if (k == 224 || k == 0) {
                k = _getch();
                if (k == 72) playerDir = 0;      // 上箭头
                else if (k == 77) playerDir = 1;  // 右箭头
                else if (k == 80) playerDir = 2;  // 下箭头
                else if (k == 75) playerDir = 3;  // 左箭头
                // F键武器切换
                else if(k == 59) currentWeapon = 0; // F1 主武器
                else if(k == 60) currentWeapon = 1; // F2 副武器
                else if(k == 61) currentWeapon = 2; // F3 刀
                return;
            }
            
            // R键换弹
            if((k == 'r' || k == 'R') && reloadTimer == 0) {
                if(currentWeapon == 0 && primaryAmmo < 30) {
                    reloadTimer = 20; // 2秒换弹
                    primaryAmmo = 30;
                } else if(currentWeapon == 1 && secondaryAmmo < 15) {
                    reloadTimer = 15; // 1.5秒换弹
                    secondaryAmmo = 15;
                }
                return;
            }
            
            // F键与保险箱交互
            if (k == 'f' || k == 'F') {
                checkSafeInteraction();
                return;
            }
            
            // 如果正在解码保险箱，处理数字输入
            if (isHacking && currentSafe != nullptr) {
                if (k >= '0' && k <= '9') {
                    int digit = k - '0';
                    hackSafe(digit);
                    return;
                } else if (k == 27) { // ESC取消
                    isHacking = false;
                    currentSafe = nullptr;
                    return;
                }
            }
            
            int newX = p.x, newY = p.y;
            
            // WASD移动
            if (k == 'w' || k == 'W') newY--;
            if (k == 's' || k == 'S') newY++;
            if (k == 'a' || k == 'A') newX--;
            if (k == 'd' || k == 'D') newX++;
            
            // 空格键射击（按玩家朝向）
            if (k == ' ' && reloadTimer == 0) {
                // 根据当前武器判断是否能射击
                bool canShoot = false;
                
                if(currentWeapon == 0 && primaryAmmo > 0) { // 主武器
                    primaryAmmo--;
                    canShoot = true;
                } else if(currentWeapon == 1 && secondaryAmmo > 0) { // 副武器
                    secondaryAmmo--;
                    canShoot = true;
                } else if(currentWeapon == 2) { // 刀（近战）
                    canShoot = true;
                }
                
                if(canShoot) {
                    // 即时射击 - 扫描朝向的所有格子
                    instantShoot();
                }
            }
            
            // Q 键技能1
            if ((k == 'q' || k == 'Q') && energy >= currentChar->skill1Cost && stealthTimer <= 0 && skill1Cooldown <= 0) {
                activateSkill1();
            }
            
            // E 键技能2
            if ((k == 'e' || k == 'E') && energy >= currentChar->skill2Cost && berserkTimer <= 0 && skill2Cooldown <= 0) {
                activateSkill2();
            }

            // 边界和障碍物检测
            newX = max(1, min(M_SIZE - 2, newX));
            newY = max(1, min(M_SIZE - 2, newY));
            // 蝶可以飞越障碍物
            if (canFly || !isObstacle(newX, newY)) {
                p.x = newX;
                p.y = newY;
            }
        }
        
        // 状态倒计时与能量恢复
        if (stealthTimer > 0) stealthTimer--;
        if (berserkTimer > 0) berserkTimer--;
        if (shieldHp > 0 && rand()%10 == 0) shieldHp--; // 护盾缓慢衰减
        
        // 露娜：能量恢复特别快
        int energyRegen = (selectedChar == 9) ? 2 : 1;
        if (energy < currentChar->baseEnergy) energy += energyRegen;
        
        // 蜂医：持续治疗
        if(selectedChar == 3 && stealthTimer > 0 && p.hp < currentChar->maxHp && rand()%3==0) {
            p.hp++;
        }

        // 子弹已改为即时命中，不再需要移动逻辑

        for (int i = 0; i < E_COUNT; i++) {
            if (!enemies[i].live) continue;
            
            // 蛊：持续中毒伤害
            if(poisonTimer[i] > 0) {
                poisonTimer[i]--;
                if(rand()%5 == 0) enemies[i].hp -= 5;
                if(enemies[i].hp <= 0) {
                    enemies[i].live = false;
                    score += 100;
                }
            }
            
            // 警觉状态检测 (隐身时不会被惊动)
            if (stealthTimer <= 0 && inAlertRange(enemies[i].x, enemies[i].y)) {
                // 玩家在九宫格内且未隐身，有30%概率被惊动
                if (!enemyAlert[i] && rand() % 10 < 3) {
                    enemyAlert[i] = true;
                }
            } else {
                // 玩家离开九宫格或隐身，有20%概率恢复平静
                if (enemyAlert[i] && rand() % 10 < 2) {
                    enemyAlert[i] = false;
                }
            }
            
            // 只有警觉状态且玩家未隐身才会追逐
            if (enemyAlert[i] && stealthTimer <= 0 && rand() % 6 == 0) {
                // AI 慢速移动 - 随机选择X或Y方向移动，避开障碍物
                int newX = enemies[i].x, newY = enemies[i].y;
                if (rand() % 2 == 0) {
                    if (enemies[i].x < p.x) newX++;
                    else if (enemies[i].x > p.x) newX--;
                } else {
                    if (enemies[i].y < p.y) newY++;
                    else if (enemies[i].y > p.y) newY--;
                }
                // 只有不是障碍物且在边界内才移动
                if (newX > 0 && newX < M_SIZE - 1 && newY > 0 && newY < M_SIZE - 1 && !isObstacle(newX, newY)) {
                    enemies[i].x = newX;
                    enemies[i].y = newY;
                }
            }
            
            // 只有警觉且玩家不在狂暴状态才攻击
            if (enemyAlert[i] && berserkTimer <= 0 && enemies[i].x == p.x && enemies[i].y == p.y) {
                int damage = 1 + difficulty;
                // 骇爪：接触秒杀敌人
                if(selectedChar == 2 && berserkTimer > 0) {
                    enemies[i].live = false;
                    score += 100;
                } 
                // 深蓝：护盾吸收伤害
                else if(shieldHp > 0) {
                    shieldHp -= damage;
                    if(shieldHp < 0) {
                        p.hp += shieldHp;
                        shieldHp = 0;
                    }
                } else {
                    p.hp -= damage;
                }
            }
        }
        
        // 检查玩家是否在撤离点
        if (p.x == extractionPoint.x && p.y == extractionPoint.y) {
            extractionTimer++;
            if (extractionTimer >= 33) { // 5秒 * 6.67帧/秒 ≈ 33帧
                missionComplete = true;
            }
        } else {
            extractionTimer = 0;
        }
    }
    
    void instantShoot() {
        // 即时射击 - 扫描朝向直到命中敌人或障碍物
        int range = (currentWeapon == 2) ? 1 : M_SIZE; // 刀只有1格射程
        int checkX = p.x;
        int checkY = p.y;
        
        for(int dist = 1; dist <= range; dist++) {
            checkX += bulletDirX[playerDir];
            checkY += bulletDirY[playerDir];
            
            // 越界检查
            if(checkX <= 0 || checkX >= M_SIZE-1 || checkY <= 0 || checkY >= M_SIZE-1) break;
            
            // 障碍物阻挡
            if(isObstacle(checkX, checkY)) break;
            
            // 检查是否命中敌人
            for (int i = 0; i < E_COUNT; i++) {
                if (enemies[i].live && enemies[i].x == checkX && enemies[i].y == checkY) {
                    // 蛊：子弹附带毒素
                    if(selectedChar == 6) {
                        poisonTimer[i] = 30;
                    }
                    enemies[i].live = false;
                    score += 100;
                    
                    // 银翼：穿透效果，可以继续打
                    if(selectedChar == 13 && currentWeapon == 0) {
                        continue;
                    }
                    return; // 普通命中后停止
                }
            }
        }
    }
    
    void checkSafeInteraction() {
        // 检查玩家附近是否有保险箱
        for (int i = 0; i < MAX_SAFES; i++) {
            if (!safes[i].opened && abs(safes[i].x - p.x) <= 1 && abs(safes[i].y - p.y) <= 1) {
                if (safes[i].locked) {
                    // 开始解码
                    isHacking = true;
                    currentSafe = &safes[i];
                    currentSafe->inputPos = 0;
                    for(int j = 0; j < 4; j++) currentSafe->currentInput[j] = -1;
                } else {
                    // 打开保险箱获取奖励
                    safes[i].opened = true;
                    playerData.inventory[playerData.inventoryCount++] = safes[i].reward;
                    playerData.money += safes[i].reward.price / 2; // 获得一半价值的金钱
                }
                return;
            }
        }
    }
    
    void hackSafe(int digit) {
        if (currentSafe == nullptr || currentSafe->inputPos >= 4) return;
        
        currentSafe->currentInput[currentSafe->inputPos] = digit;
        currentSafe->inputPos++;
        
        // 检查是否输入完成
        if (currentSafe->inputPos >= 4) {
            bool correct = true;
            for (int i = 0; i < 4; i++) {
                if (currentSafe->currentInput[i] != currentSafe->code[i]) {
                    correct = false;
                    break;
                }
            }
            
            if (correct) {
                // 解码成功
                currentSafe->locked = false;
                isHacking = false;
                score += 200;
            } else {
                // 解码失败，重置
                currentSafe->inputPos = 0;
                for(int j = 0; j < 4; j++) currentSafe->currentInput[j] = -1;
            }
        }
    }
    
    void activateSkill1() {
        energy -= currentChar->skill1Cost;
        stealthTimer = 40;
        skill1Cooldown = 80; // 8秒冷却
        
        // 特殊效果
        if(selectedChar == 3) { // 蜂医：治疗
            p.hp = min(p.hp + 30, currentChar->maxHp);
        } else if(selectedChar == 10) { // 蝶：飞行
            canFly = true;
        } else if(selectedChar == 9) { // 露娜：能量恢复
            energy = min(energy + 20, currentChar->baseEnergy);
        }
    }
    
    void activateSkill2() {
        energy -= currentChar->skill2Cost;
        berserkTimer = 30;
        skill2Cooldown = 100; // 10秒冷却
        
        // 特殊效果  
        if(selectedChar == 8) { // 深蓝：护盾
            shieldHp = 50;
        } else if(selectedChar == 3) { // 蜂医：群体治疗
            p.hp = currentChar->maxHp;
        } else if(selectedChar == 10) { // 蝶：蝶舞
            canFly = true;
            stealthTimer = 20;
        }
    }

    void playIntroAnimation() {
        // 第一阶段：直升机从远处飞来
        for(int frame = 0; frame < 10; frame++) {
            system("cls");
            
            // 计算直升机位置
            int heliPos = frame * 2;
            
            // 打印缩小的直升机
            for(int i = 0; i < heliPos; i++) printf(" ");
            printf("  _//_\n");
            for(int i = 0; i < heliPos; i++) printf(" ");
            printf(" [VH-1]>>\n");
            for(int i = 0; i < heliPos; i++) printf(" ");
            printf(" o    o\n");
            
            printf("\n >> 直升机接近投放点... <<\n\n");
            
            // 在下方显示地图
            draw();
            
            Sleep(60);
        }
        
        // 第二阶段：玩家空降
        for(int drop = 0; drop < 6; drop++) {
            system("cls");
            
            // 直升机悬停（缩小版）
            for(int j = 0; j < 15; j++) printf(" ");
            printf("  _//_\n");
            for(int j = 0; j < 15; j++) printf(" ");
            printf(" [VH-1]>>\n");
            for(int j = 0; j < 15; j++) printf(" ");
            printf(" o | o\n");
            
            // 绳索和玩家
            for(int rope = 0; rope < drop; rope++) {
                for(int j = 0; j < 17; j++) printf(" ");
                printf("|\n");
            }
            
            if(drop < 5) {
                for(int j = 0; j < 16; j++) printf(" ");
                printf("[%c]\n", currentChar->icon);
            }
            
            if(drop < 5) {
                printf("\n >> %s 正在空降... <<\n\n", currentChar->name.c_str());
            } else {
                printf("\n >> 着陆成功！准备作战！<<\n\n");
            }
            
            // 在下方显示地图
            draw();
            
            Sleep(100);
        }
        
        Sleep(300);
        printf("\n    按任意键开始任务...");
        _getch();
    }
    
    void start() {
        playIntroAnimation();
        
        while (p.hp > 0 && !missionComplete) {
            draw();
            update();
            Sleep(50);
        }
        system("cls");
        printf("\n\n");
        if (p.hp <= 0) {
            printf("    ========================================\n");
            printf("    ||      任务失败 - MISSION FAILED    ||\n");
            printf("    ========================================\n");
            printf("\n    干员阵亡，任务终止\n\n");
        } else if (missionComplete) {
            printf("    ========================================\n");
            printf("    ||     撤离成功 - EXTRACTION COMPLETE ||\n");
            printf("    ========================================\n");
            printf("\n    干员: %s\n", currentChar->name.c_str());
            printf("    存活生命值: %d\n", p.hp);
            printf("    难度: %s\n\n", difficulty==0?"简单":difficulty==1?"普通":"困难");
            printf("    任务完成！\n\n");
        }
        getMouseClick();
    }
};

// 鼠标点击检测函数（返回Y坐标或特殊值）
int getMouseClick() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    
    INPUT_RECORD inputRecord;
    DWORD events;
    
    while(true) {
        ReadConsoleInput(hStdin, &inputRecord, 1, &events);
        
        if(inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {
            char key = inputRecord.Event.KeyEvent.uChar.AsciiChar;
            if(key >= '1' && key <= '6') {
                SetConsoleMode(hStdin, mode);
                return key - '0';
            }
            // 任意其他按键也返回（用于"按任意键继续"场景）
            if(key == 13 || key == 27 || key == 32) { // Enter, ESC, Space
                SetConsoleMode(hStdin, mode);
                return 0;
            }
        }
        
        if(inputRecord.EventType == MOUSE_EVENT) {
            MOUSE_EVENT_RECORD mer = inputRecord.Event.MouseEvent;
            // 左键点击（包括按下和释放）
            if(mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                int x = mer.dwMousePosition.X;
                int y = mer.dwMousePosition.Y;
                
                SetConsoleMode(hStdin, mode);
                // 主菜单特殊处理
                if(x >= 4 && x <= 20 && y >= 13 && y <= 18) {
                    return y - 12; // 返回1-6对应菜单项
                }
                // 其他情况返回Y坐标
                return y;
            }
            // 右键点击返回-1
            else if(mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
                SetConsoleMode(hStdin, mode);
                return -1;
            }
            // 任何鼠标按键按下都响应
            else if(mer.dwButtonState != 0) {
                SetConsoleMode(hStdin, mode);
                return mer.dwMousePosition.Y;
            }
        }
    }
}

void showMainMenu() {
    system("cls");
    printf("\n\n");
    printf("    ==========================================\n");
    printf("    ||                                      ||\n");
    printf("    ||      DELTAFORCE v4.0               ||\n");
    printf("    ||      三角洲特种部队                 ||\n");
    printf("    ||                                      ||\n");
    printf("    ==========================================\n\n");
    printf("    玩家资金: $%d\n", playerData.money);
    printf("    仓库物品: %d/50\n\n", playerData.inventoryCount);
    printf("    【主菜单】\n\n");
    printf("    [1] 开始任务\n");
    printf("    [2] 操作说明\n");
    printf("    [3] 玩家仓库\n");
    printf("    [4] 市场商店\n");
    printf("    [5] 交易行\n");
    printf("    [6] 退出游戏\n\n");
    printf("    ▶ 点击选项或按数字键\n");
}

void showInstructions() {
    system("cls");
    printf("\n\n");
    printf("    ========== 操作说明 ==========\n\n");
    printf("    【任务目标】\n");
    printf("      到达撤离点并生存！\n");
    printf("      收集保险箱奖励提升装备！\n\n");
    printf("    【操作说明】\n");
    printf("      WASD键   - 移动角色\n");
    printf("      方向键   - 转换朝向（上下左右）\n");
    printf("      空格键   - 按朝向射击\n");
    printf("      Q键      - 技能1（消耗能量）\n");
    printf("      E键      - 技能2（消耗能量）\n");
    printf("      F键      - 与保险箱交互\n");
    printf("      R键      - 换弹\n\n");
    printf("    【地图元素】\n");
    printf("      =   - 边界墙壁\n");
    printf("      #   - 障碍物（可阻挡）\n");
    printf("      ◎  - 撤离点（停留5秒撤离）\n");
    printf("      $   - 上锁的保险箱\n");
    printf("      ☆  - 已解锁的保险箱\n");
    printf("      o   - 普通敌人（平静）\n");
    printf("      !   - 普通敌人（警觉）\n");
    printf("      e/E - 精英敌人（平静/警觉）\n");
    printf("      X/M - BOSS（平静/警觉）\n");
    printf("      ~   - 中毒敌人\n\n");
    printf("    【保险箱系统】\n");
    printf("      - 靠近保险箱按F键开始解码\n");
    printf("      - 输入4位数字密码解锁\n");
    printf("      - 系统会给第1位密码提示\n");
    printf("      - 解锁后再按F获取奖励\n\n");
    printf("    【战术提示】\n");
    printf("      - 靠近敌人九宫格才会惊动AI\n");
    printf("      - 利用方向键切换朝向精准射击\n");
    printf("      - 不同干员有独特技能\n");
    printf("      - 能量会自动恢复\n");
    printf("      - 收集物品可在市场交易\n\n");
    printf("    点击屏幕返回主菜单...");
    getMouseClick();
}

void showInventory() {
    system("cls");
    printf("\n\n");
    printf("    ╔════════════════════════════════════════╗\n");
    printf("    ║          玩家仓库系统                ║\n");
    printf("    ╚════════════════════════════════════════╝\n\n");
    printf("    资金: $%d | 物品数量: %d/50\n\n", playerData.money, playerData.inventoryCount);
    
    if (playerData.inventoryCount == 0) {
        printf("    仓库空空如也...\n");
    } else {
        printf("    【物品列表】\n");
        for (int i = 0; i < playerData.inventoryCount; i++) {
            Item& item = playerData.inventory[i];
            string rarityStr = item.rarity == 3 ? "传说" : item.rarity == 2 ? "史诗" : item.rarity == 1 ? "稀有" : "普通";
            printf("    [%d] %s (%s) - $%d\n", i+1, item.name.c_str(), rarityStr.c_str(), item.price);
        }
    }
    
    printf("\n    点击屏幕返回...");
    getMouseClick();
}

void showMarket() {
    system("cls");
    printf("\n\n");
    printf("    ╔════════════════════════════════════════╗\n");
    printf("    ║       官方商店 - Delta Store         ║\n");
    printf("    ╚════════════════════════════════════════╝\n\n");
    printf("    你的资金: $%d\n\n", playerData.money);
    
    Item marketItems[30] = {
        // 武器类
        {"M4A1突击步枪", 600, 1, "weapon"},
        {"AK-47突击步枪", 550, 1, "weapon"},
        {"MP5冲锋枪", 450, 1, "weapon"},
        {"Vector冲锋枪", 520, 1, "weapon"},
        {"Glock-19手枪", 250, 0, "weapon"},
        {"Desert Eagle手枪", 380, 1, "weapon"},
        // 弹药类
        {"5.56弹药箱(120发)", 200, 0, "ammo"},
        {"7.62弹药箱(90发)", 250, 1, "ammo"},
        {"9mm弹药箱(150发)", 150, 0, "ammo"},
        {".45ACP弹药箱(100发)", 180, 0, "ammo"},
        {"12号霰弹(25发)", 220, 0, "ammo"},
        // 医疗消耗品
        {"绷带", 50, 0, "med"},
        {"医疗包", 100, 0, "med"},
        {"止血带", 80, 0, "med"},
        {"急救箱", 180, 1, "med"},
        {"肾上腺素注射器", 150, 1, "med"},
        {"吗啡止痛剂", 120, 0, "med"},
        // 防具类
        {"一级防弹衣", 250, 0, "armor"},
        {"二级防弹衣", 450, 1, "armor"},
        {"三级防弹衣", 700, 2, "armor"},
        {"战术头盔", 400, 1, "armor"},
        {"防暴盾牌", 550, 1, "armor"},
        // 装备类
        {"夜视仪", 500, 2, "equipment"},
        {"战术背包", 300, 1, "equipment"},
        {"对讲机", 180, 0, "equipment"},
        {"GPS定位器", 400, 1, "equipment"},
        // 投掷物和工具
        {"破片手雷", 200, 0, "equipment"},
        {"闪光弹", 150, 0, "equipment"},
        {"烟雾弹", 120, 0, "equipment"},
        {"C4遥控炸药", 800, 2, "equipment"}
    };
    
    printf("    【商品列表】\n");
    for (int i = 0; i < 30; i++) {
        string rarityStr = marketItems[i].rarity == 3 ? "传说" : marketItems[i].rarity == 2 ? "史诗" : 
                          marketItems[i].rarity == 1 ? "稀有" : "普通";
        printf("    [%d] %s (%s) - $%d\n", i+1, marketItems[i].name.c_str(), rarityStr.c_str(), marketItems[i].price);
    }
    
    printf("\n    点击物品编号购买 | 右键返回\n");
    
    // 鼠标输入
    int clickResult = getMouseClick();
    int index = -1;
    
    // 根据Y坐标计算选择的物品（【商品列表】在第8行，第一个物品在第9行）
    if (clickResult > 0 && clickResult >= 9 && clickResult < 39) {
        index = clickResult - 9;
        if (index >= 30) index = -1; // 超出范围
    } else if (clickResult == -1) {
        return; // 右键返回
    }
    
    if (index >= 0 && index < 30) {
        if (playerData.money >= marketItems[index].price && playerData.inventoryCount < 50) {
            playerData.money -= marketItems[index].price;
            playerData.inventory[playerData.inventoryCount++] = marketItems[index];
            printf("\n\n    购买成功！%s 已加入仓库！\n", marketItems[index].name.c_str());
        } else if (playerData.money < marketItems[index].price) {
            printf("\n\n    资金不足！\n");
        } else {
            printf("\n\n    仓库已满！\n");
        }
        Sleep(1500);
    }
}

void showTradingPost() {
    while(true) {
        system("cls");
        printf("\n\n");
        printf("    ╔════════════════════════════════════════╗\n");
        printf("    ║       玩家交易行 - Player Market     ║\n");
        printf("    ╚════════════════════════════════════════╝\n\n");
        printf("    你的资金: $%d | 仓库: %d/50\n\n", playerData.money, playerData.inventoryCount);
        
        printf("    【交易选项】\n");
        printf("    [1] 从其他玩家处购买物品\n");
        printf("    [2] 出售物品到交易行\n");
        printf("    [0] 返回主菜单\n\n");
        printf("    点击选项: ");
        
        int clickY = getMouseClick();
        
        if (clickY == -1 || clickY == 11) return; // 右键或点击返回
        
        // 购买模式
        if (clickY == 9) {
            system("cls");
            printf("\n\n");
            printf("    ╔════════════════════════════════════════╗\n");
            printf("    ║       交易行 - 购买物品              ║\n");
            printf("    ╚════════════════════════════════════════╝\n\n");
            printf("    你的资金: $%d\n\n", playerData.money);
            
            // 模拟其他玩家挂出的物品
            Item tradingItems[40] = {
                // 顶级武器
                {"AWM狙击步枪", 1400, 3, "weapon"},
                {"Barrett M82狙击枪", 1800, 3, "weapon"},
                {"SCAR-H突击步枪", 750, 2, "weapon"},
                {"HK416突击步枪", 820, 2, "weapon"},
                {"M4A1突击步枪", 550, 1, "weapon"},
                {"AK-74M突击步枪", 580, 1, "weapon"},
                {"Kriss Vector冲锋枪", 480, 1, "weapon"},
                {"MP7冲锋枪", 520, 1, "weapon"},
                // 高价值变卖物
                {"黄金条", 950, 2, "material"},
                {"钻石项链", 1600, 3, "material"},
                {"古董花瓶", 1300, 3, "material"},
                {"情报硬盘", 1150, 3, "material"},
                {"现金包", 550, 2, "material"},
                {"军用笔记本电脑", 880, 2, "material"},
                {"名贵手表", 780, 2, "material"},
                {"艺术画作", 1250, 3, "material"},
                {"稀有邮票", 680, 2, "material"},
                {"珍珠耳环", 850, 2, "material"},
                {"红宝石戒指", 1100, 3, "material"},
                {"古董钱币", 600, 2, "material"},
                // 高级装备
                {"三级防弹衣", 650, 2, "armor"},
                {"三级防弹插板", 750, 2, "armor"},
                {"战术头盔", 380, 1, "armor"},
                {"防暴盾牌", 520, 1, "armor"},
                {"夜视仪", 580, 2, "equipment"},
                {"热成像仪", 1350, 3, "equipment"},
                {"GPS定位器", 420, 1, "equipment"},
                {"便携式无人机", 980, 2, "equipment"},
                {"战术背包", 280, 1, "equipment"},
                {"对讲机", 150, 0, "equipment"},
                // 弹药和消耗品
                {".308狙击弹药(60发)", 320, 1, "ammo"},
                {"7.62弹药箱(90发)", 280, 1, "ammo"},
                {"5.56弹药箱(120发)", 220, 0, "ammo"},
                {"军用医疗包", 350, 1, "med"},
                {"急救箱", 200, 1, "med"},
                {"肾上腺素注射器", 180, 1, "med"},
                {"白磷手雷", 650, 2, "equipment"},
                {"C4遥控炸药", 850, 2, "equipment"},
                {"破片手雷", 220, 0, "equipment"},
                {"闪光弹", 160, 0, "equipment"}
            };
            
            printf("    【玩家挂售商品】\n");
            for (int i = 0; i < 40; i++) {
                string rarityStr = tradingItems[i].rarity == 3 ? "传说" : tradingItems[i].rarity == 2 ? "史诗" : 
                                  tradingItems[i].rarity == 1 ? "稀有" : "普通";
                
                // 显示按键提示
                char keyDisplay[10];
                if(i < 9) sprintf(keyDisplay, "%d", i+1);
                else if(i == 9) sprintf(keyDisplay, "0");
                else if(i < 36) sprintf(keyDisplay, "%c", 'A' + (i-10));
                else sprintf(keyDisplay, "Pg%d", i-35);
                
                printf("    [%s] %s (%s) - $%d\n", keyDisplay, tradingItems[i].name.c_str(), rarityStr.c_str(), tradingItems[i].price);
            }
            
            printf("\n    点击物品编号购买 | 右键返回\n");
            
            int clickResult = getMouseClick();
            int index = -1;
            
            // 【玩家挂售商品】在第8行，第一个物品在第9行
            if (clickResult > 0 && clickResult >= 9 && clickResult < 49) {
                index = clickResult - 9;
                if (index >= 40) index = -1;
            } else if (clickResult == -1) {
                continue;
            }
            
            if (index >= 0 && index < 40) {
                if (playerData.money >= tradingItems[index].price && playerData.inventoryCount < 50) {
                    playerData.money -= tradingItems[index].price;
                    playerData.inventory[playerData.inventoryCount++] = tradingItems[index];
                    printf("\n\n    购买成功！%s 已加入你的仓库！\n", tradingItems[index].name.c_str());
                } else if (playerData.money < tradingItems[index].price) {
                    printf("\n\n    资金不足！需要 $%d\n", tradingItems[index].price);
                } else {
                    printf("\n\n    仓库已满！\n");
                }
                Sleep(2000);
            }
        }
        // 出售模式
        else if (clickY == 10) {
            system("cls");
            printf("\n\n");
            printf("    ╔════════════════════════════════════════╗\n");
            printf("    ║       交易行 - 出售物品              ║\n");
            printf("    ╚════════════════════════════════════════╝\n\n");
            printf("    你的资金: $%d\n\n", playerData.money);
            
            if (playerData.inventoryCount == 0) {
                printf("    你没有可出售的物品！\n");
                printf("\n    点击任意处返回...");
                getMouseClick();
                continue;
            }
            
            printf("    【你的物品】\n");
            for (int i = 0; i < playerData.inventoryCount; i++) {
                Item& item = playerData.inventory[i];
                string rarityStr = item.rarity == 3 ? "传说" : item.rarity == 2 ? "史诗" : 
                                  item.rarity == 1 ? "稀有" : "普通";
                int sellPrice = (int)(item.price * 0.85); // 85%价格出售（交易行比商店回收高）
                printf("    [%d] %s (%s) - 售价: $%d\n", i+1, item.name.c_str(), rarityStr.c_str(), sellPrice);
            }
            
            printf("\n    点击物品编号出售 | 右键返回\n");
            
            int clickResult = getMouseClick();
            
            if (clickResult == -1) continue;
            
            // 【你的物品】在第8行，第一个物品在第9行
            if (clickResult >= 9 && clickResult < 9 + playerData.inventoryCount) {
                int index = clickResult - 9;
                if (index < playerData.inventoryCount) {
                    int sellPrice = (int)(playerData.inventory[index].price * 0.85);
                    playerData.money += sellPrice;
                    printf("\n\n    出售成功！获得 $%d\n", sellPrice);
                    
                    // 移除物品
                    for (int i = index; i < playerData.inventoryCount - 1; i++) {
                        playerData.inventory[i] = playerData.inventory[i + 1];
                    }
/*  */                    playerData.inventoryCount--;
                    Sleep(2000);
                }
            }
        }
    }
}

void showCharacterDetail(int index) {
    system("cls");
    Character& c = characters[index];
    printf("\n");
    printf("    ╔════════════════════════════════════════════╗\n");
    printf("    ║          干员详细资料 [%c]               ║\n", c.icon);
    printf("    ╚════════════════════════════════════════════╝\n\n");
    printf("    【代号】 %s\n\n", c.name.c_str());
    printf("    【基础属性】\n");
    printf("      ♥ 生命值: %d\n", c.maxHp);
    printf("      ⚡ 能量值: %d\n", c.baseEnergy);
    printf("      ✦ 外观标识: %c\n\n", c.icon);
    printf("    【技能配置】\n");
    printf("      [Q] %s (消耗: %d 能量)\n", c.skill1Name.c_str(), c.skill1Cost);
    printf("      [E] %s (消耗: %d 能量)\n\n", c.skill2Name.c_str(), c.skill2Cost);
    printf("    【干员特性】\n");
    printf("      %s\n\n", c.description.c_str());
    printf("    ════════════════════════════════════════════\n");
    printf("    按 [Space] 选择此干员 | 按其他键返回列表\n");
}

int showCharacterSelection() {
    int currentPage = 0;
    int charsPerPage = 7;
    int totalPages = (14 + charsPerPage - 1) / charsPerPage;
    
    while(true) {
        system("cls");
        printf("\n");
        printf("    ╔════════════════════════════════════════════╗\n");
        printf("    ║            干员选择中心                  ║\n");
        printf("    ╚════════════════════════════════════════════╝\n\n");
        printf("    第 %d/%d 页 | 方向键翻页 | 数字键查看详情\n\n", currentPage + 1, totalPages);
        
        int start = currentPage * charsPerPage;
        int end = min(start + charsPerPage, 14);
        
        for(int i = start; i < end; i++) {
            char key;
            if(i < 9) key = '1' + i;
            else if(i == 9) key = '0';
            else if(i == 10) key = 'A';
            else if(i == 11) key = 'B';
            else if(i == 12) key = 'C';
            else key = 'D';
            
            printf("    [%c] %-10s | HP:%-3d EN:%-3d | %c\n", 
                   key, characters[i].name.c_str(), 
                   characters[i].maxHp, characters[i].baseEnergy,
                   characters[i].icon);
        }
        
        printf("\n    ════════════════════════════════════════════\n");
        printf("    操作提示:\n");
        printf("      点击干员查看详细资料\n");
        printf("      左右键翻页    右键返回\n");
        printf("\n    点击选择: ");
        
        int clickY = getMouseClick();
        
        // 右键返回
        if(clickY == -1) {
            return -1;
        }
        
        // 点击翻页区域（假设在最底部）
        if(clickY == 26) { // 左翻页
            if(currentPage > 0) currentPage--;
            continue;
        }
        if(clickY == 27) { // 右翻页
            if(currentPage < totalPages - 1) currentPage++;
            continue;
        }
        
        // 点击干员（从Y=10开始显示，每个干员占2-3行）
        int charIndex = -1;
        if(clickY >= 10 && clickY < 25) {
            int lineFromStart = clickY - 10;
            charIndex = currentPage * 5 + (lineFromStart / 3); // 每个干员约3行
            if(charIndex >= 14) charIndex = -1;
        }
        
        if(charIndex >= 0 && charIndex < 14) {
            showCharacterDetail(charIndex);
            printf("\n\n    点击确认选择此干员，右键返回");
            int confirm = getMouseClick();
            if(confirm > 0) { // 左键确认
                return charIndex;
            }
        }
    }
}

int main() {
    SetConsoleOutputCP(65001);
    int choice;  // 改为int类型存储选择结果
    
    while(true) {
        // 显示主菜单
        showMainMenu();
        choice = getMouseClick();
        
        if(choice == 6) {
            // 退出游戏
            system("cls");
            printf("\n\n    感谢游玩！\n\n");
            Sleep(1000);
            return 0;
        }
        else if(choice == 2) {
            // 显示说明
            showInstructions();
            continue;
        }
        else if(choice == 3) {
            // 玩家仓库
            showInventory();
            continue;
        }
        else if(choice == 4) {
            // 市场商店
            showMarket();
            continue;
        }
        else if(choice == 5) {
            // 交易行
            showTradingPost();
            continue;
        }
        else if(choice == 1) {
            // 开始游戏流程
            bool gameStarted = false;
            
            while(!gameStarted) {
                // 难度选择
                system("cls");
                printf("\n");
                printf("    =======  猛攻！！！  =======\n");
                printf("    ===========================\n\n");
                printf("    选择难度:\n\n");
                printf("    [1] 普通\n");
                printf("    [2] 机密\n");
                printf("    [3] 绝密\n\n");
                printf("    点击选择难度: ");
                
                int diff = 1;
                int diffClick = getMouseClick();
                if(diffClick == 3) diff = 0;      // Y=3 -> 普通
                else if(diffClick == 5) diff = 2;  // Y=5 -> 绝密
                else diff = 1;                     // Y=4 -> 机密
                
                // 角色选择 - 使用新的详细界面
                int charIndex = showCharacterSelection();
                
                // 如果用户按ESC返回，重新选择难度
                if(charIndex == -1) {
                    continue;
                }
                
                system("cls");
                printf("\n    开始任务...\n    角色: %s\n    难度: %s\n\n    点击屏幕继续...", 
                       characters[charIndex].name.c_str(),
                       diff==0?"简单":diff==1?"普通":"困难");
                getMouseClick();
                system("cls");
                
                GameEngine g(charIndex, diff);
                g.start();
                gameStarted = true;
            }
        }
    }
    
    return 0;
}

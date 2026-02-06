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

struct Unit {
    int x, y;
    bool live;
    int hp;
};

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

// 14个角色定义
Character characters[14] = {
    {"威龙", 150, 100, 30, 50, "龙吟", "龙威", "高生命坦克，技能CD短", '@'},
    {"红狼", 90, 120, 25, 45, "狼嘴", "血怒", "快速移动，低能量消耗", 'W'},
    {"骇爪", 100, 100, 40, 60, "利爪", "致命一击", "近战专家，接触秒杀", 'K'},
    {"蜂医", 80, 110, 20, 50, "治疗", "群体治疗", "生命恢复，持续治疗", '+'},
    {"疾风", 85, 130, 35, 55, "疾跑", "残影", "极速移动，连续闪避", '>'},
    {"牧羊人", 110, 90, 40, 70, "召唤", "羊群护卫", "召唤辅助，范围防御", 'Y'},
    {"蛊", 95, 100, 30, 60, "毒雾", "蛊术", "毒素攻击，持续伤害", 'G'},
    {"比特", 100, 115, 45, 65, "入侵", "控制", "黑客技能，控制敌人", 'H'},
    {"深蓝", 120, 95, 35, 55, "护盾", "深海之力", "护盾吸收，额外防护", 'D'},
    {"露娜", 90, 140, 25, 40, "月光", "满月祝福", "能量恢复极快", 'L'},
    {"蝶", 85, 105, 30, 50, "羽化", "蝶舞", "飞行模式，穿越障碍", '&'},
    {"无名", 100, 100, 40, 60, "隐身", "狂暴", "全能平衡型标准技能", '?'},
    {"乌鲁鲁", 130, 80, 50, 70, "战吼", "狂战", "狂战士高伤低防", 'U'},
    {"银翼", 95, 110, 35, 55, "精准", "多重射击", "精准射击，多段伤害", 'A'}
};

class GameEngine {
    Unit p = {2, 2, true, 100};
    Unit enemies[E_COUNT];
    bool enemyAlert[E_COUNT];
    int enemyType[E_COUNT];  // 敌人类型 0=普通 1=精英 2=BOSS
    Unit obstacles[OBSTACLE_COUNT];
    Unit bullet = {0, 0, false, 0};
    Unit bullets[5];
    int score = 0;
    
    // 技能系统
    int stealthTimer = 0;
    int berserkTimer = 0;
    int energy = 100;
    int shieldHp = 0;      // 护盾值（深蓝专属）
    int poisonTimer[E_COUNT]; // 中毒计时器（蛊专属）
    bool canFly = false;   // 飞行状态（蝶专属）
    
    // 角色和难度
    int selectedChar = 11;
    int difficulty = 1;
    Character* currentChar;

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

        // 加载存档
        ifstream f("save.txt");
        int savedHp, savedScore;
        if (f >> savedHp >> savedScore) {
            if (savedScore > score) score = savedScore;
        }
        f.close();

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
    }

    void draw() {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});

        printf("==========================================\n");
        printf(" 角色: %s | 难度: %s\n", currentChar->name.c_str(), 
               difficulty==0?"简单":difficulty==1?"普通":"困难");
        printf(" 生命: %-3d", p.hp);
        if(shieldHp > 0) printf("(+%d盾) ", shieldHp);
        printf("| 分数: %-4d | 能量: %-3d\n", score, energy);
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
                    
                    // 检查障碍物
                    if (!hit) {
                        for (int i = 0; i < OBSTACLE_COUNT; i++)
                            if (obstacles[i].live && obstacles[i].x == x && obstacles[i].y == y) {
                                printf("# ");
                                hit = true;
                                break;
                            }
                    }
                    
                    // 检查子弹
                    if (!hit && bullet.live && bullet.x == x && bullet.y == y) {
                        printf("| ");
                        hit = true;
                    }
                    
                    // 检查银翼多重子弹
                    if(!hit && selectedChar == 13) {
                        for(int i=0; i<5; i++) {
                            if(bullets[i].live && bullets[i].x == x && bullets[i].y == y) {
                                printf("i ");
                                hit = true;
                                break;
                            }
                        }
                    }
                    
                    if (!hit) {
                        if (y == 0 || y == M_SIZE - 1 || x == 0 || x == M_SIZE - 1) printf("= ");
                        else printf(". ");
                    }
                }
            }
            printf("\n");
        }
        printf("WASD:移动 | 空格:射击 | Q:%s(%d) | E:%s(%d)\n", 
               currentChar->skill1Name.c_str(), currentChar->skill1Cost,
               currentChar->skill2Name.c_str(), currentChar->skill2Cost);
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
        if (_kbhit()) {
            int k = _getch();
            int newX = p.x, newY = p.y;
            
            if (k == 'w' || k == 'W') newY--;
            if (k == 's' || k == 'S') newY++;
            if (k == 'a' || k == 'A') newX--;
            if (k == 'd' || k == 'D') newX++;
            
            // 射击
            if (k == ' ') {
                // 银翼：多重射击
                if(selectedChar == 13) {
                    for(int i=0; i<5; i++) {
                        if(!bullets[i].live) {
                            bullets[i] = {p.x + (i-2), p.y - 1, true, 0};
                            break;
                        }
                    }
                } else if (!bullet.live || berserkTimer > 0) {
                    bullet = {p.x, p.y - 1, true, 0};
                }
            }
            
            // Q 键技能1
            if ((k == 'q' || k == 'Q') && energy >= currentChar->skill1Cost && stealthTimer <= 0) {
                activateSkill1();
            }
            
            // E 键技能2
            if ((k == 'e' || k == 'E') && energy >= currentChar->skill2Cost && berserkTimer <= 0) {
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

        if (bullet.live) {
            bullet.y--;
            if (bullet.y <= 0 || isObstacle(bullet.x, bullet.y)) bullet.live = false;
            checkBulletHit(bullet);
        }
        
        // 银翼多重子弹
        if(selectedChar == 13) {
            for(int i=0; i<5; i++) {
                if(bullets[i].live) {
                    bullets[i].y--;
                    if (bullets[i].y <= 0 || isObstacle(bullets[i].x, bullets[i].y)) 
                        bullets[i].live = false;
                    checkBulletHit(bullets[i]);
                }
            }
        }

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
    }
    
    void checkBulletHit(Unit& b) {
        for (int i = 0; i < E_COUNT; i++) {
            if (enemies[i].live && enemies[i].x == b.x && enemies[i].y == b.y) {
                // 蛊：子弹附带毒素
                if(selectedChar == 6) {
                    poisonTimer[i] = 30;
                }
                enemies[i].live = b.live = false;
                score += 100;
                ofstream f("save.txt");
                f << p.hp << " " << score;
                f.close();
                break;
            }
        }
    }
    
    void activateSkill1() {
        energy -= currentChar->skill1Cost;
        stealthTimer = 40;
        
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

    void start() {
        while (p.hp > 0 && score < E_COUNT * 100) {
            draw();
            update();
            Sleep(150);
        }
        system("cls");
        if (p.hp <= 0) {
            printf("任务失败，存档清空\n");
            ofstream f("save.txt");
            f << 100 << " " << 0;
            f.close();
        } else printf("任务成功！\n");
        _getch();
    }
};

void showMainMenu() {
    system("cls");
    printf("\n\n");
    printf("    ==========================================\n");
    printf("    ||                                      ||\n");
    printf("    ||      特种兵作战 v2.0              ||\n");
    printf("    ||      SPECIAL OPS MISSION           ||\n");
    printf("    ||                                      ||\n");
    printf("    ==========================================\n\n");
    printf("    【主菜单】\n\n");
    printf("    [1] 开始游戏\n");
    printf("    [2] 游戏说明\n");
    printf("    [3] 退出游戏\n\n");
    printf("    请选择 (1-3): ");
}

void showInstructions() {
    system("cls");
    printf("\n\n");
    printf("    ========== 游戏说明 ==========\n\n");
    printf("    【游戏目标】\n");
    printf("      消灭所有敌人并生存到最后！\n\n");
    printf("    【操作说明】\n");
    printf("      WASD - 移动角色\n");
    printf("      空格 - 射击\n");
    printf("      Q键  - 技能1（消耗能量）\n");
    printf("      E键  - 技能2（消耗能量）\n\n");
    printf("    【地图元素】\n");
    printf("      =   - 边界墙壁\n");
    printf("      #   - 障碍物（可阻挡）\n");
    printf("      o   - 普通敌人（平静）\n");
    printf("      !   - 普通敌人（警觉）\n");
    printf("      e/E - 精英敌人（平静/警觉）\n");
    printf("      X/M - BOSS（平静/警觉）\n");
    printf("      ~   - 中毒敌人\n\n");
    printf("    【战斗技巧】\n");
    printf("      - 靠近敌人九宫格才会惊动AI\n");
    printf("      - 利用隐身技能潜行穿越\n");
    printf("      - 不同角色有独特技能\n");
    printf("      - 能量会自动恢复\n\n");
    printf("    按任意键返回主菜单...");
    _getch();
}

int main() {
    SetConsoleOutputCP(65001);
    char choice;  // 声明choice变量在更大的作用域
    
    while(true) {
        // 显示主菜单
        showMainMenu();
        choice = _getch();
        
        if(choice == '3') {
            // 退出游戏
            system("cls");
            printf("\n\n    感谢游玩！\n\n");
            Sleep(1000);
            return 0;
        }
        else if(choice == '2') {
            // 显示说明
            showInstructions();
            continue;
        }
        else if(choice == '1') {
            // 开始游戏
            break;
        }
    }
    
    // 难度选择
    system("cls");
    printf("\n");
    printf("    =======  特种兵作战  =======\n");
    printf("    ===========================\n\n");
    printf("    选择难度:\n\n");
    printf("    [1] 简单 - p\n");
    printf("    [2] 普通 - 正常任务\n");
    printf("    [3] 困难 - 生死考验\n\n");
    printf("    请选择 (1-3): ");
    
    int diff = 1;
    choice = _getch();
    if(choice == '1') diff = 0;
    else if(choice == '3') diff = 2;
    else diff = 1;
    
    // 角色选择
    system("cls");
    printf("\n    选择你的特种兵:\n\n");
    for(int i=0; i<14; i++) {
        printf("    [%c] %s - %s\n", 
               i<9?('1'+i):(i==9?'0':(i==10?'A':(i==11?'B':(i==12?'C':'D')))),
               characters[i].name.c_str(),
               characters[i].description.c_str());
    }
    printf("\n    请选择: ");
    
    int charIndex = 11; // 默认无名
    choice = _getch();
    if(choice >= '1' && choice <= '9') charIndex = choice - '1';
    else if(choice == '0') charIndex = 9;
    else if(choice == 'a' || choice == 'A') charIndex = 10;
    else if(choice == 'b' || choice == 'B') charIndex = 11;
    else if(choice == 'c' || choice == 'C') charIndex = 12;
    else if(choice == 'd' || choice == 'D') charIndex = 13;
    
    system("cls");
    printf("\n    开始任务...\n    角色: %s\n    难度: %s\n\n    按任意键继续...", 
           characters[charIndex].name.c_str(),
           diff==0?"简单":diff==1?"普通":"困难");
    _getch();
    system("cls");
    
    GameEngine g(charIndex, diff);
    g.start();
    return 0;
}

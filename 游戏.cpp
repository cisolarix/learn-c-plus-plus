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
        printf("方向键:转向 | 鼠标:射击 | R:换弹\n");
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
                int weaponDamage = 10;
                
                if(currentWeapon == 0 && primaryAmmo > 0) { // 主武器
                    primaryAmmo--;
                    canShoot = true;
                    weaponDamage = 10;
                } else if(currentWeapon == 1 && secondaryAmmo > 0) { // 副武器
                    secondaryAmmo--;
                    canShoot = true;
                    weaponDamage = 15; // 副武器伤害更高
                } else if(currentWeapon == 2) { // 刀（近战）
                    canShoot = true;
                    weaponDamage = 50; // 近战伤害很高但射程短
                }
                
                if(canShoot) {
                    // 银翼：多重射击（仅主武器）
                    if(selectedChar == 13 && currentWeapon == 0) {
                        for(int i=0; i<5; i++) {
                            if(!bullets[i].live) {
                                bullets[i] = {p.x + bulletDirX[playerDir], p.y + bulletDirY[playerDir], true, playerDir};
                                break;
                            }
                        }
                    } else if (!bullet.live || berserkTimer > 0) {
                        bullet = {p.x + bulletDirX[playerDir], p.y + bulletDirY[playerDir], true, playerDir};
                    }
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

        if (bullet.live) {
            // 根据子弹方向移动
            bullet.x += bulletDirX[bullet.hp];
            bullet.y += bulletDirY[bullet.hp];
            if (bullet.x <= 0 || bullet.x >= M_SIZE-1 || bullet.y <= 0 || bullet.y >= M_SIZE-1 || isObstacle(bullet.x, bullet.y)) 
                bullet.live = false;
            checkBulletHit(bullet);
        }
        
        // 银翼多重子弹
        if(selectedChar == 13) {
            for(int i=0; i<5; i++) {
                if(bullets[i].live) {
                    bullets[i].x += bulletDirX[bullets[i].hp];
                    bullets[i].y += bulletDirY[bullets[i].hp];
                    if (bullets[i].x <= 0 || bullets[i].x >= M_SIZE-1 || bullets[i].y <= 0 || bullets[i].y >= M_SIZE-1 || isObstacle(bullets[i].x, bullets[i].y)) 
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
    
    void checkBulletHit(Unit& b) {
        for (int i = 0; i < E_COUNT; i++) {
            if (enemies[i].live && enemies[i].x == b.x && enemies[i].y == b.y) {
                // 蛊：子弹附带毒素
                if(selectedChar == 6) {
                    poisonTimer[i] = 30;
                }
                enemies[i].live = b.live = false;
                break;
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
        system("cls");
        
        // 第一阶段：直升机从远处飞来
        for(int frame = 0; frame < 15; frame++) {
            system("cls");
            printf("\n\n\n\n");
            
            // 计算直升机位置
            int heliPos = frame * 3;
            
            // 打印侧面视角直升机
            for(int i = 0; i < heliPos; i++) printf(" ");
            printf("      __//__\n");
            for(int i = 0; i < heliPos; i++) printf(" ");
            printf("  ___=========___\n");
            for(int i = 0; i < heliPos; i++) printf(" ");
            printf(" |  [  VH-1  ]  |>>\n");
            for(int i = 0; i < heliPos; i++) printf(" ");
            printf(" |______________|__\n");
            for(int i = 0; i < heliPos; i++) printf(" ");
            printf("   o          o\n");
            
            printf("\n\n\n");
            printf("    >> 直升机接近投放点... <<\n");
            Sleep(80);
        }
        
        // 第二阶段：悬停准备投放
        for(int i = 0; i < 3; i++) {
            system("cls");
            printf("\n\n\n\n");
            for(int j = 0; j < 30; j++) printf(" ");
            printf("      __//__\n");
            for(int j = 0; j < 30; j++) printf(" ");
            printf("  ___=========___\n");
            for(int j = 0; j < 30; j++) printf(" ");
            printf(" |  [  VH-1  ]  |>>\n");
            for(int j = 0; j < 30; j++) printf(" ");
            printf(" |_____[|||]____|__\n");
            for(int j = 0; j < 30; j++) printf(" ");
            printf("   o          o\n");
            printf("\n\n\n");
            printf("    >> 抵达投放点，准备快速绳降... <<\n");
            Sleep(500);
        }
        
        // 第三阶段：玩家滑索降落
        for(int drop = 0; drop < 12; drop++) {
            system("cls");
            printf("\n\n\n\n");
            
            // 直升机（侧面视角）
            for(int j = 0; j < 30; j++) printf(" ");
            printf("      __//__\n");
            for(int j = 0; j < 30; j++) printf(" ");
            printf("  ___=========___\n");
            for(int j = 0; j < 30; j++) printf(" ");
            printf(" |  [  VH-1  ]  |>>\n");
            for(int j = 0; j < 30; j++) printf(" ");
            printf(" |_____[|||]____|__\n");
            for(int j = 0; j < 30; j++) printf(" ");
            printf("   o    |     o\n");
            
            // 绳索
            for(int rope = 0; rope < drop; rope++) {
                for(int j = 0; j < 38; j++) printf(" ");
                printf("|\n");
            }
            
            // 玩家角色
            if(drop < 11) {
                for(int j = 0; j < 37; j++) printf(" ");
                printf("[%c]\n", currentChar->icon);
                for(int j = 0; j < 37; j++) printf(" ");
                printf("/|\\\n");
                for(int j = 0; j < 37; j++) printf(" ");
                printf("/ \\\n");
            }
            
            printf("\n");
            for(int ground = drop; ground < 11; ground++) printf("\n");
            printf("    ========================================\n");
            printf("            === 着陆区域 ===\n");
            
            if(drop < 11) {
                printf("\n    >> %s 正在快速绳降... <<\n", currentChar->name.c_str());
            } else {
                printf("\n    >> 着陆成功！准备作战！<<\n");
            }
            
            Sleep(150);
        }
        
        Sleep(800);
        
        // 任务简报
        system("cls");
        printf("\n\n\n");
        printf("    ==========================================\n");
        printf("              任务简报\n");
        printf("    ==========================================\n\n");
        printf("    干员: %s\n", currentChar->name.c_str());
        printf("    难度: %s\n", difficulty==0?"简单":difficulty==1?"普通":"困难");
        printf("    目标: 消灭所有敌对目标\n");
        printf("    武器: 已配备主副武器及近战刀具\n\n");
        printf("    ==========================================\n");
        printf("\n    按任意键开始任务...\n");
        _getch();
    }
    
    void start() {
        playIntroAnimation();
        
        while (p.hp > 0 && !missionComplete) {
            draw();
            update();
            Sleep(150);
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
        _getch();
    }
};

void showMainMenu() {
    system("cls");
    printf("\n\n");
    printf("    ==========================================\n");
    printf("    ||                                      ||\n");
    printf("    ||      DELTAFORCE v3.0               ||\n");
    printf("    ||      三角洲特种部队                 ||\n");
    printf("    ||                                      ||\n");
    printf("    ==========================================\n\n");
    printf("    【主菜单】\n\n");
    printf("    [1] 开始任务\n");
    printf("    [2] 操作说明\n");
    printf("    [3] 退出游戏\n\n");
    printf("    请选择 (1-3): ");
}

void showInstructions() {
    system("cls");
    printf("\n\n");
    printf("    ========== 操作说明 ==========\n\n");
    printf("    【任务目标】\n");
    printf("      到达撤离点并生存！\n\n");
    printf("    【操作说明】\n");
    printf("      WASD键   - 移动角色\n");
    printf("      方向键   - 转换朝向（上下左右）\n");
    printf("      空格键   - 按朝向射击\n");
    printf("      Q键      - 技能1（消耗能量）\n");
    printf("      E键      - 技能2（消耗能量）\n\n");
    printf("    【地图元素】\n");
    printf("      =   - 边界墙壁\n");
    printf("      #   - 障碍物（可阻挡）\n");
    printf("      ◎  - 撤离点（停留5秒撤离）\n");
    printf("      o   - 普通敌人（平静）\n");
    printf("      !   - 普通敌人（警觉）\n");
    printf("      e/E - 精英敌人（平静/警觉）\n");
    printf("      X/M - BOSS（平静/警觉）\n");
    printf("      ~   - 中毒敌人\n\n");
    printf("    【战术提示】\n");
    printf("      - 靠近敌人九宫格才会惊动AI\n");
    printf("      - 利用方向键切换朝向精准射击\n");
    printf("      - 不同干员有独特技能\n");
    printf("      - 能量会自动恢复\n\n");
    printf("    按任意键返回主菜单...");
    _getch();
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
        printf("      按数字/字母键 - 查看干员详细资料\n");
        printf("      < > - 翻页    ESC - 返回上一级\n");
        printf("\n    请选择: ");
        
        char choice = _getch();
        
        // ESC返回
        if(choice == 27) {
            return -1;
        }
        
        // 处理方向键 (Windows下方向键是两个字符)
        if(choice == 0 || choice == -32) {
            choice = _getch();
            if(choice == 75) { // 左箭头
                if(currentPage > 0) currentPage--;
                continue;
            }
            if(choice == 77) { // 右箭头
                if(currentPage < totalPages - 1) currentPage++;
                continue;
            }
        }
        
        // 用字母代替方向键
        if(choice == '<' || choice == ',') { // < 或 ,
            if(currentPage > 0) currentPage--;
            continue;
        }
        if(choice == '>' || choice == '.') { // > 或 .
            if(currentPage < totalPages - 1) currentPage++;
            continue;
        }
        
        // 查看详情
        int charIndex = -1;
        if(choice >= '1' && choice <= '9') charIndex = choice - '1';
        else if(choice == '0') charIndex = 9;
        else if(choice == 'a' || choice == 'A') charIndex = 10;
        else if(choice == 'b' || choice == 'B') charIndex = 11;
        else if(choice == 'c' || choice == 'C') charIndex = 12;
        else if(choice == 'd' || choice == 'D') charIndex = 13;
        
        if(charIndex >= 0 && charIndex < 14) {
            showCharacterDetail(charIndex);
            char confirm = _getch();
            if(confirm == ' ' || confirm == 13) { // 空格或回车确认
                return charIndex;
            }
        }
    }
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
            // 开始游戏流程
            bool gameStarted = false;
            
            while(!gameStarted) {
                // 难度选择
                system("cls");
                printf("\n");
                printf("    =======  特种兵作战  =======\n");
                printf("    ===========================\n\n");
                printf("    选择难度:\n\n");
                printf("    [1] 简单 - 练习模式\n");
                printf("    [2] 普通 - 正常任务\n");
                printf("    [3] 困难 - 生死考验\n\n");
                printf("    请选择 (1-3): ");
                
                int diff = 1;
                choice = _getch();
                if(choice == '1') diff = 0;
                else if(choice == '3') diff = 2;
                else diff = 1;
                
                // 角色选择 - 使用新的详细界面
                int charIndex = showCharacterSelection();
                
                // 如果用户按ESC返回，重新选择难度
                if(charIndex == -1) {
                    continue;
                }
                
                system("cls");
                printf("\n    开始任务...\n    角色: %s\n    难度: %s\n\n    按任意键继续...", 
                       characters[charIndex].name.c_str(),
                       diff==0?"简单":diff==1?"普通":"困难");
                _getch();
                system("cls");
                
                GameEngine g(charIndex, diff);
                g.start();
                gameStarted = true;
            }
        }
    }
    
    return 0;
}

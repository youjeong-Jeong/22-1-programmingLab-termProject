//2171011 정유정
// 슈팅게임
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>

// 색상 정의
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15

#define PLAYER 'U' //player표시
#define BLANK ' ' // ' ' 로하면 흔적이 지워진다 
#define BULLET "|"

#define ENEMY 'V'
#define EBULLET '*'
#define ENEMY2 'Y'
#define E2BULLET '!'
#define BBULLET 'o'
#define POWER 'P'
#define WING '='
#define LWING '<'
#define RWING '>' 

#define ESC 0x1b //  ESC 누르면 종료

#define UP		'w' // WASD로 이동
#define DOWN	's'
#define LEFT	'a'
#define RIGHT	'd'
#define SPACE	' ' // 스페이스바로 발사

#define WIDTH 60
#define HEIGHT 40

int Delay = 50; // 50 msec delay, 이 값을 줄이면 속도가 빨라진다.
int keep_moving = 1; // 1:계속이동, 0:한칸씩이동.
int oldx, oldy, newx, newy; //bullet과 공유
int enewx, enewy; //ebullet 과 공유
int e2newx, e2newy; //e2bullet 과 공유
int bossx;
int bossy = 3;
int score = 0;
int power = 1;
int called = 0;
int life = 5;
int bosslife = 500;

int powers[WIDTH][HEIGHT] = { 0 };
int power_count = 0;

int bullet[WIDTH][HEIGHT] = { 0 }; //나의 총알
int bullet_count = 0;
int ebullet[WIDTH][HEIGHT] = { 0 };// 적 1의 총알
int ebullet_count = 0;
int e2bullet[WIDTH][HEIGHT] = { 0 };// 적 2의 총알
int e2bullet_count = 0;
int bossbullet[WIDTH][HEIGHT] = { 0 };
int bossbullet_count = 0;
int rainbullet[WIDTH][HEIGHT] = { 0 };
int rainbullet_count = 0;
int rainbullet_op = 0;

int powerinterval = 4; //파워 표시 간격
int enemyinterval = 3; //적 표시 간격
int enemy2interval = 4; // 적 2 표시간격
int enemy[WIDTH][HEIGHT] = { 0 };
int enemy_count = 0;
int enemy2[WIDTH][HEIGHT] = { 0 };
int enemy2_count = 0;
int boss[WIDTH][HEIGHT] = { 0 };
int boss_count = 0;
int bossmove = 0;

int frame_count = 0; // frame_count로 속도 조절
int p_frame_sync = 1; // 1프레임 마다 이동
int p_frame_sync_start = 0;
int bullet_frame_sync = 1; // 1 프레임 마다 총알을 움직인다
int ebullet_move_frame_sync = 2; // 2 프레임마다 발사된 적의 총알 움직임
int e2bullet_move_frame_sync = 1; // 1 프레임마다 발사된 적2의 총알 움직임
int enemy_frame_sync = 5; // 5 프레임 마다 적1을 움직인다
int enemy2_frame_sync = 4; //4 프레임마다 적2을 움직임
int boss_frame_sync = 1; //1 프레임마다 보스 움직임
int ebullet_frame_sync = 50; // 50 프레임마다 적의 총알 발사
int e2bullet_frame_sync = 40; // 40 프레임마다 적2의 총알 발사
int bossbullet_frame_sync = 10;
int bulletrain_frame_sync = 15;
int power_frame_sync = 1;



void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);// WIN32API 함수입니다. 이건 알필요 없어요
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

void removeCursor(void) { // 커서를 안보이게 한다

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

// 화면 지우기고 원하는 배경색으로 설정한다.
void cls(int bg_color, int text_color)
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);

}

void putplayer(int x, int y, char ch)
{
	gotoxy(x, y);
	putchar(ch);
}

void eraseplayer(int x, int y)
{
	gotoxy(x, y);
	putchar(BLANK);
}

void showscore()
{
	textcolor(YELLOW1, BLACK);
	int i = 0;
	gotoxy(20, 0);
	printf("BOSS ");
	gotoxy(25, 0);
	for (; i < 6; i++) {
		printf("□");
	}
	if (score >= 5 && score < 10) {
		gotoxy(25, 0);
		printf("■");
	}
	else if (score >= 10 && score < 15) {
		gotoxy(25, 0);
		printf("■■");
	}
	else if (score >= 15 && score < 20) {
		gotoxy(25, 0);
		printf("■■■");
	}
	else if (score >= 20 && score < 25) {
		gotoxy(25, 0);
		printf("■■■■");
	}
	else if (score >= 25 && score < 30) {
		gotoxy(25, 0);
		printf("■■■■■");
	}
	else if (score >= 30) {
		gotoxy(25, 0);
		printf("■■■■■■");
	}
	textcolor(WHITE, BLACK);
}

void showlife() {
	int i = 0;
	textcolor(WHITE, BLACK);
	gotoxy(0, 0);
	printf("LIFE ");

	textcolor(RED1, BLACK);
	gotoxy(5, 0);
	for (; i < life; i++) {
		printf("♥");
	}
	for (; i < 5; i++) {
		printf("♡");
	}

	textcolor(WHITE, BLACK);
}

void showpower() {
	textcolor(CYAN2, BLACK);
	gotoxy(43, 0);
	printf("POWER : %d", power);
	textcolor(WHITE, BLACK);
}

void show_bosslife() {
	textcolor(GREEN2, BLACK);
	gotoxy(18, 0);
	printf("                        ");
	gotoxy(20, 0);
	if (bosslife <= 0) {
		bosslife = 0;
	}
	printf("BOSS LIFE :%3d", bosslife);
	textcolor(WHITE, BLACK);
}

void showendMessage() {
	textcolor(WHITE, BLACK);
	gotoxy(54, 0);
	printf("(ESC)");
	textcolor(WHITE, BLACK);
}

void show_power() {
	int x, y;
	x = rand() % WIDTH;
	y = rand() % (HEIGHT - 18) + 1;  // 위쪽에 배치

	if (x > WIDTH - 1) {
		x = WIDTH - 2;
	}
	if (x < 1) {
		x = 1;
	}
	textcolor(CYAN2, BLACK);
	gotoxy(x, y);
	putchar(POWER);
	gotoxy(x-1, y);
	putchar(LWING);
	gotoxy(x+1, y);
	putchar(RWING);
	powers[x][y] = 1;
	power_count++;
	textcolor(BLACK, WHITE);
}

void player(unsigned char ch) {
	int move_flag = 0;
	static unsigned char last_ch = 0;

	if (called == 0) { // 처음 또는 Restart
		oldx = 30, oldy = 35, newx = 30, newy = 35;
		putplayer(oldx, oldy, PLAYER);
		called = 1;
		last_ch = 0;
		ch = 0;
	}

	// 같은 방향으로 key 가 들어오면 무시한다.
	if (last_ch == ch && frame_count % p_frame_sync != 0)
		return;
	if (keep_moving && ch == 0)
		ch = last_ch;
	last_ch = ch;

	switch (ch) {
	case UP:
		if (oldy > 1)
			newy = oldy - 1;
		else { // 벽에 부딛치면 방향을 반대로 이동
			newy = oldy + 1;
			last_ch = DOWN;
		}
		move_flag = 1;
		break;
	case DOWN:
		if (oldy < HEIGHT - 1)
			newy = oldy + 1;
		else {
			newy = oldy - 1;
			last_ch = UP;
		}
		move_flag = 1;
		break;
	case LEFT:
		if (oldx > 0)
			newx = oldx - 1;
		else {
			newx = oldx + 1;
			last_ch = RIGHT;
		}
		move_flag = 1;
		break;
	case RIGHT:
		if (oldx < WIDTH - 1)
			newx = oldx + 1;
		else {
			newx = oldx - 1;
			last_ch = LEFT;
		}
		move_flag = 1;
		break;
	}
	if (move_flag == 1) {
		eraseplayer(oldx, oldy);
		putplayer(newx, newy, PLAYER);
		oldx = newx; // 마지막 위치를 기억한다.
		oldy = newy;
	}
}


void show_enemy() {
	int x, y;
	x = rand() % WIDTH;
	y = 1;
	textcolor(GREEN2, BLACK);
	gotoxy(x - 1, y);
	putchar(WING);
	gotoxy(x, y);
	putchar(ENEMY);
	gotoxy(x + 1, y);
	putchar(WING);
	enemy[x][y] = 1;
	enemy_count++;
	textcolor(WHITE, BLACK);
}

void show_enemy2() {
	int x, y;
	x = rand() % WIDTH;
	y = 1;
	textcolor(MAGENTA2, BLACK);
	gotoxy(x, y);
	putchar(ENEMY2);
	enemy2[x][y] = 1;
	enemy2_count++;
	textcolor(WHITE, BLACK);
}

void shot_ebullet() {
	ebullet[enewx][enewy + 1] = 1;
	ebullet_count++;
	gotoxy(enewx, enewy + 1);
	textcolor(RED2, BLACK);
	putchar(EBULLET);
	textcolor(WHITE, BLACK);
}

void shot_e2bullet() {
	e2bullet[e2newx][e2newy + 1] = 1;
	e2bullet_count++;
	gotoxy(e2newx, e2newy + 1);
	textcolor(RED1, BLACK);
	putchar(E2BULLET);
	textcolor(WHITE, BLACK);
}

void shot_bossbullet() {
	bossbullet[bossx + 5][bossy + 6] = 1;
	bossbullet[bossx + 6][bossy + 6] = 1;
	bossbullet[bossx + 13][bossy + 6] = 1;
	bossbullet[bossx + 14][bossy + 6] = 1;
	bossbullet_count += 4;
	textcolor(RED2, BLACK);
	gotoxy(bossx + 5, bossy + 6);
	putchar(BBULLET);
	gotoxy(bossx + 6, bossy + 6);
	putchar(BBULLET);
	gotoxy(bossx + 13, bossy + 6);
	putchar(BBULLET);
	gotoxy(bossx + 14, bossy + 6);
	putchar(BBULLET);
	textcolor(WHITE, BLACK);
}

void shot_rainbullet() {
	int x, y;
	x = WIDTH;
	y = 1;
	int r = rand() % 5;
	rainbullet_op = rand() % 3 + 1;
	if (rainbullet_op == 1) {
		for (int i = r; i < WIDTH; i += 5) {
			rainbullet[i][y] = 1;
			rainbullet_count++;
			gotoxy(i, y);
			textcolor(CYAN2, BLACK);
			putchar(E2BULLET);
		}
	}
	else if (rainbullet_op == 2) {
		for (int i = r; i < WIDTH; i += 5) {
			rainbullet[i][y] = 1;
			rainbullet_count++;
			gotoxy(i, y);
			textcolor(CYAN2, BLACK);
			putchar(E2BULLET);
			y++;
		}
	}
	else if (rainbullet_op == 3) {
		y = 15;
		for (int i = r; i < WIDTH; i += 5) {
			rainbullet[i][y] = 1;
			rainbullet_count++;
			gotoxy(i, y);
			textcolor(CYAN2, BLACK);
			putchar(E2BULLET);
			y--;
		}
	}
	textcolor(WHITE, BLACK);
}

void move_ebullet() {
	int x, y, eby;
	int new_ebullet[WIDTH][HEIGHT] = { 0 };

	if (ebullet_count == 0) return;

	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (ebullet[x][y]) {
				eby = y + 1;
				if (eby > HEIGHT - 1) {
					gotoxy(x, y);
					putchar(BLANK);
					new_ebullet[x][y] = 0;
					--ebullet_count;
				}
				else {
					gotoxy(x, y);
					textcolor(BLACK, BLACK);
					putchar(BLANK); // erase bullet
					textcolor(RED2, BLACK);
					gotoxy(x, eby);
					putchar(EBULLET);
					new_ebullet[x][eby] = 1;
					textcolor(WHITE, BLACK);
					if (new_ebullet[newx][newy]) { //적의 총알에 플레이어가 맞았을 때
						--life;
						showlife();
						gotoxy(x, eby);
						putchar(BLANK); // erase bullet
						new_ebullet[x][eby] = 0;
					}
				}
			}
		}
	}
	memcpy(ebullet, new_ebullet, sizeof(new_ebullet)); // 한번에 ebullet 위치를 조정한다.
}

void move_e2bullet() {
	int x, y, e2by;
	int new_ebullet[WIDTH][HEIGHT] = { 0 };

	if (e2bullet_count == 0) return;

	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (e2bullet[x][y]) {
				e2by = y + 1;
				if (e2by > HEIGHT - 1) {
					gotoxy(x, y);
					putchar(BLANK);
					new_ebullet[x][y] = 0;
					--e2bullet_count;
				}
				else {
					gotoxy(x, y);
					textcolor(BLACK, BLACK);
					putchar(BLANK); // erase bullet
					textcolor(RED2, BLACK);
					gotoxy(x, e2by);
					putchar(E2BULLET);
					new_ebullet[x][e2by] = 1;
					textcolor(WHITE, BLACK);
					if (new_ebullet[newx][newy]) { //적의 총알에 플레이어가 맞았을 때
						--life;
						showlife();
						gotoxy(x, e2by);
						putchar(BLANK); // erase bullet
						new_ebullet[x][e2by] = 0;
					}
				}
			}
		}
	}
	memcpy(e2bullet, new_ebullet, sizeof(new_ebullet)); // 한번에 e2bullet 위치를 조정한다.
}

void move_bossbullet() {
	int x, y, bossby;
	int new_bossbullet[WIDTH][HEIGHT] = { 0 };

	if (bossbullet_count == 0) return;

	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (bossbullet[x][y]) {
				bossby = y + 1;
				if (bossby > HEIGHT - 1) {
					gotoxy(x, y);
					putchar(BLANK);
					new_bossbullet[x][y] = 0;
					--bossbullet_count;
				}
				else {
					gotoxy(x, y);
					textcolor(BLACK, BLACK);
					putchar(BLANK); // erase bullet
					textcolor(RED2, BLACK);
					gotoxy(x, bossby);
					putchar(BBULLET);
					new_bossbullet[x][bossby] = 1;
					textcolor(WHITE, BLACK);
					if (new_bossbullet[newx][newy]) { //적의 총알에 플레이어가 맞았을 때
						--life;
						if (life <= 0) {
							boss_count = 0;
						}
						showlife();
						gotoxy(x, bossby);
						putchar(BLANK); // erase bullet
						new_bossbullet[x][bossby] = 0;
					}
				}
			}
		}
	}
	memcpy(bossbullet, new_bossbullet, sizeof(new_bossbullet)); // 한번에 ebullet 위치를 조정한다.
}

void move_rainbullet() {
	int x, y, rby;
	int new_rbullet[WIDTH][HEIGHT] = { 0 };

	if (rainbullet_count == 0) return;

	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (rainbullet[x][y]) {
				rby = y + 1;
				if (rby > HEIGHT - 1) {
					gotoxy(x, y);
					putchar(BLANK);
					new_rbullet[x][y] = 0;
					--rainbullet_count;
				}
				else {
					gotoxy(x, y);
					textcolor(BLACK, BLACK);
					putchar(BLANK); // erase bullet
					int c1;
					c1 = rand() % 8 + 8;
					textcolor(c1, BLACK);
					gotoxy(x, rby);
					putchar(E2BULLET);
					new_rbullet[x][rby] = 1;
					textcolor(WHITE, BLACK);
					if (new_rbullet[newx][newy]) { //적의 총알에 플레이어가 맞았을 때
						--life;
						if (life <= 0) {
							boss_count = 0;
						}
						showlife();
						gotoxy(x, rby);
						putchar(BLANK); // erase bullet
						new_rbullet[x][rby] = 0;
					}
				}
			}
		}
	}
	memcpy(rainbullet, new_rbullet, sizeof(new_rbullet)); // 한번에 ebullet 위치를 조정한다.
}

void move_enemy() {
	int x, y, dx, dy;
	int newenemy[WIDTH][HEIGHT] = { 0 };

	if (enemy_count == 0) //enemy가 없으면
		return;
	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (enemy[x][y]) {
				dx = rand() % 7 - 3; // -3 -2 -1 0 1 2 3
				dy = rand() % 2; // 0 1
				enewx = x + dx;
				enewy = y + dy;
				if (enewx + 1 >= WIDTH) enewx = WIDTH - 2;
				if (enewx - 1 < 0) enewx = 2;
				if (y >= HEIGHT - 1) {
					gotoxy(x - 1, y);
					putchar(BLANK);
					gotoxy(x, y);
					putchar(BLANK); // erase enemy
					gotoxy(x + 1, y);
					putchar(BLANK);
					newenemy[x][y] = 0;
					--enemy_count;
				}
				else {
					gotoxy(x, y);
					textcolor(BLACK, BLACK);
					gotoxy(x - 1, y);
					putchar(BLANK);
					gotoxy(x, y);
					putchar(BLANK); // erase enemy
					gotoxy(x + 1, y);
					putchar(BLANK);
					textcolor(GREEN2, BLACK);
					gotoxy(enewx - 1, enewy);
					putchar(WING);
					gotoxy(enewx, enewy);
					putchar(ENEMY);
					gotoxy(enewx + 1, enewy);
					putchar(WING);
					newenemy[enewx][enewy] = 1; // 이동된 enemy의 좌표
					textcolor(WHITE, BLACK);
					if (frame_count % ebullet_frame_sync == 0) {
						shot_ebullet();
					}
					if (newenemy[newx][newy]) { //적이 플레이어에게 닿았을 때
						--life;
						showlife();
						gotoxy(enewx - 1, enewy);
						putchar(BLANK);
						gotoxy(enewx, enewy);
						putchar(BLANK);
						gotoxy(enewx + 1, enewy);
						putchar(BLANK); // erase enemy
						newenemy[enewx][enewy] = 0;
					}
				}
			}
		}
	}
	memcpy(enemy, newenemy, sizeof(newenemy));
}

void move_enemy2() {
	int x, y, dx, dy;
	int newenemy[WIDTH][HEIGHT] = { 0 };

	if (enemy2_count == 0) //enemy가 없으면
		return;
	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (enemy2[x][y]) {
				dx = rand() % 5 - 2; // -2 -1 0 1 2 
				dy = rand() % 2; // 0 1
				e2newx = x + dx;
				e2newy = y + dy;
				if (e2newx + 1 >= WIDTH) e2newx = WIDTH - 2;
				if (e2newx - 1 < 0) e2newx = 2;
				if (y >= HEIGHT - 1) {
					gotoxy(x - 1, y);
					putchar(BLANK);
					gotoxy(x, y);
					putchar(BLANK); // erase enemy2
					gotoxy(x + 1, y);
					putchar(BLANK);
					newenemy[x][y] = 0;
					--enemy2_count;
				}
				else {
					gotoxy(x, y);
					textcolor(BLACK, BLACK);
					gotoxy(x - 1, y);
					putchar(BLANK);
					gotoxy(x, y);
					putchar(BLANK); // erase enemy2
					gotoxy(x + 1, y);
					putchar(BLANK);
					textcolor(MAGENTA2, BLACK);
					gotoxy(e2newx - 1, e2newy);
					putchar(WING);
					gotoxy(e2newx, e2newy);
					putchar(ENEMY2);
					gotoxy(e2newx + 1, e2newy);
					putchar(WING);
					newenemy[e2newx][e2newy] = 1; // 이동된 enemy2의 좌표
					textcolor(WHITE, BLACK);
					if (frame_count % e2bullet_frame_sync == 0) {
						shot_e2bullet();
					}
					if (newenemy[newx][newy]) { //적이 플레이어에게 닿았을 때
						--life;
						showlife();
						gotoxy(e2newx - 1, e2newy);
						putchar(BLANK);
						gotoxy(e2newx, e2newy);
						putchar(BLANK);
						gotoxy(e2newx + 1, e2newy);
						putchar(BLANK); // erase enemy2
						newenemy[e2newx][e2newy] = 0;
					}
				}
			}
		}
	}
	memcpy(enemy2, newenemy, sizeof(newenemy));
}

void show_boss() {
	bossy = 3;
	textcolor(RED2, BLACK);
	gotoxy(bossx, bossy);
	printf("$$$              $$$"); //20
	boss[bossx][bossy] = 1;
	boss[bossx + 19][bossy] = 1;
	gotoxy(bossx, bossy + 1);
	printf(" $$$$$$$$$$$$$$$$$$ ");
	boss[bossx + 1][bossy] = 1;
	boss[bossx + 18][bossy] = 1;
	gotoxy(bossx, bossy + 2);
	printf(" $$$$$$$$$$$$$$$$$$ ");
	boss[bossx + 1][bossy] = 1;
	boss[bossx + 18][bossy] = 1;
	gotoxy(bossx, bossy + 3);
	printf("  $$$$$$$$$$$$$$$$  ");
	boss[bossx + 2][bossy] = 1;
	boss[bossx + 3][bossy] = 1;
	boss[bossx + 16][bossy] = 1;
	boss[bossx + 17][bossy] = 1;
	gotoxy(bossx, bossy + 4);
	printf("    $$$$$$$$$$$$    ");
	boss[bossx + 4][bossy] = 1;
	boss[bossx + 7][bossy] = 1;
	boss[bossx + 8][bossy] = 1;
	boss[bossx + 9][bossy] = 1;
	boss[bossx + 10][bossy] = 1;
	boss[bossx + 11][bossy] = 1;
	boss[bossx + 12][bossy] = 1;
	boss[bossx + 15][bossy] = 1;
	gotoxy(bossx, bossy + 5);
	printf("     $$      $$     ");
	boss[bossx + 5][bossy] = 1;
	boss[bossx + 6][bossy] = 1;
	boss[bossx + 13][bossy] = 1;
	boss[bossx + 14][bossy] = 1;
	textcolor(WHITE, BLACK);
}

void boss_die() {
	int y;
	y = 3;
	textcolor(GRAY2, BLACK);
	gotoxy(bossx, y);
	printf("  $                $"); //20
	boss[bossx][y] = 1;
	boss[bossx + 19][y] = 1;
	gotoxy(bossx, y + 1);
	printf(" $$$  $$$$  $$$$  $ ");
	boss[bossx + 1][y] = 1;
	boss[bossx + 18][y] = 1;
	gotoxy(bossx, y + 2);
	printf("   $$$$  $$$$  $$ $ ");
	boss[bossx + 1][y] = 1;
	boss[bossx + 18][y] = 1;
	gotoxy(bossx, y + 3);
	printf("  $$$  $$$ $ $$  $  ");
	boss[bossx + 2][y] = 1;
	boss[bossx + 3][y] = 1;
	boss[bossx + 16][y] = 1;
	boss[bossx + 17][y] = 1;
	gotoxy(bossx, y + 4);
	printf("    $  $$  $$$ $    ");
	boss[bossx + 4][y] = 1;
	boss[bossx + 7][y] = 1;
	boss[bossx + 8][y] = 1;
	boss[bossx + 9][y] = 1;
	boss[bossx + 10][y] = 1;
	boss[bossx + 11][y] = 1;
	boss[bossx + 12][y] = 1;
	boss[bossx + 15][y] = 1;
	gotoxy(bossx, y + 5);
	printf("      $       $     ");
	boss[bossx + 5][y] = 1;
	boss[bossx + 6][y] = 1;
	boss[bossx + 13][y] = 1;
	boss[bossx + 14][y] = 1;
	textcolor(WHITE, BLACK);
}

void remove_boss() {
	int y;
	y = 3;
	textcolor(BLACK, BLACK);
	gotoxy(bossx, y);
	printf("                    "); //20
	boss[bossx][y] = 0;
	boss[bossx + 19][y] = 0;
	gotoxy(bossx, y + 1);
	printf("                    ");
	boss[bossx + 1][y] = 0;
	boss[bossx + 18][y] = 0;
	gotoxy(bossx, y + 2);
	printf("                    ");
	boss[bossx + 1][y] = 0;
	boss[bossx + 18][y] = 0;
	gotoxy(bossx, y + 3);
	printf("                    ");
	boss[bossx + 2][y] = 0;
	boss[bossx + 3][y] = 0;
	boss[bossx + 16][y] = 0;
	boss[bossx + 17][y] = 0;
	gotoxy(bossx, y + 4);
	printf("                    ");
	boss[bossx + 4][y] = 0;
	boss[bossx + 7][y] = 0;
	boss[bossx + 8][y] = 0;
	boss[bossx + 9][y] = 0;
	boss[bossx + 10][y] = 0;
	boss[bossx + 11][y] = 0;
	boss[bossx + 12][y] = 0;
	boss[bossx + 15][y] = 0;
	gotoxy(bossx, y + 5);
	printf("                    ");
	boss[bossx + 5][y] = 0;
	boss[bossx + 6][y] = 0;
	boss[bossx + 13][y] = 0;
	boss[bossx + 14][y] = 0;
	textcolor(WHITE, BLACK);

}

void move_boss() {
	remove_boss();
	if (bossx >= WIDTH - 21) {
		bossmove = 1;
	}
	else if (bossx < 1) {
		bossmove = 0;
	}

	if (bossmove == 1) {
		bossx--;
	}
	if (bossmove == 0) {
		bossx++;
	}
	if (frame_count % bossbullet_frame_sync == 0) {
		shot_bossbullet();
	}

	show_boss();
}

void move_power() {
	int x, y;

	if (power_count == 0)
		return;
	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (powers[x][y]) {
				textcolor(CYAN2, BLACK);
				gotoxy(x, y);
				putchar(POWER);
				gotoxy(x-1, y);
				putchar(LWING);
				gotoxy(x+1, y);
				putchar(RWING);
				textcolor(WHITE, BLACK);
			}		
		}
	}
}

void draw_box(int x1, int y1, int x2, int y2, char* ch)
{
	for (int x = x1; x < x2; x += 2) {
		gotoxy(x, y1);
		printf("%s", ch);
		gotoxy(x, y2);
		printf("%s", ch);
	}
	for (int y = y1; y < y2 + 1; y++) {
		gotoxy(x1, y);
		printf("%s", ch);
		gotoxy(x2, y);
		printf("%s", ch);
	}
}

void draw_hline(int y, int x1, int x2, char ch)
{
	gotoxy(x1, y);
	for (; x1 <= x2; x1++)
		putchar(ch);
}

void shotBullet() {
	bullet[newx][newy - 1] = 1;
	bullet_count++;
	gotoxy(newx, newy - 1);
	printf(BULLET);

}

void move_bullet() { //총알을 움직인다
	int x, y, by;
	int newbullet[WIDTH][HEIGHT] = { 0 };

	if (bullet_count == 0) return;

	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			if (bullet[x][y]) {
				by = y - 1;
				if (by < 1) {
					gotoxy(x, y);
					putchar(BLANK); // erase bullet;
					newbullet[x][y] = 0;
					--bullet_count;
					//textcolor(BLACK,BLACK);
					//textcolor(WHITE,BLACK);
				}
				else {
					gotoxy(x, y);
					textcolor(BLACK, BLACK);
					printf(" "); // erase bullet
					textcolor(WHITE, BLACK);
					gotoxy(x, by);
					printf(BULLET);
					newbullet[x][by] = 1;
					++bullet_count;
					textcolor(WHITE, BLACK);
					if (enemy[x][by]) { // 플레이어의 총알이 적1을 맞췄을 때
						gotoxy(x-1, by);
						putchar(BLANK);
						gotoxy(x, by);
						putchar(BLANK);
						gotoxy(x+1, by);
						putchar(BLANK);
						newbullet[x][by] = 0;
						--bullet_count;
						++score;
						showscore();
						enemy[x][by] = 0;
						enemy_count--;
					}
					if (enemy2[x][by]) { // 플레이어의 총알이 적2을 맞췄을 때
						gotoxy(x - 1, by);
						putchar(BLANK);
						gotoxy(x, by);
						putchar(BLANK);
						gotoxy(x + 1, by);
						putchar(BLANK);
						newbullet[x][by] = 0;
						--bullet_count;
						score += 2; // 점수 2배
						showscore();
						enemy2[x][by] = 0;
						enemy2_count--;
					}
					if (boss[x][by]) { //플레이어의 총알이 보스를 맞췄을 때
						gotoxy(x, by);
						putchar(BLANK);
						newbullet[x][by] = 0;
						--bullet_count;
						bosslife -= power;
						show_bosslife();
					}
					if (powers[x][by]) { // 총알로 power를 맞췄을 때
						gotoxy(x, by);
						putchar(BLANK);
						gotoxy(x-1, by);
						putchar(BLANK);
						gotoxy(x+1, by);
						putchar(BLANK);
						powers[x][by] = 0;
						power++;
						showpower();
					}
				}
			}
		}
	}
	memcpy(bullet, newbullet, sizeof(newbullet)); // 한번에 bullet 위치를 조정한다.
}


void init_game() { //게임 시작

	int x, y;
	char cmd[100];

	srand((unsigned)time(NULL));

	bullet_count = 0; // 총알 초기화
	ebullet_count = 0;
	e2bullet_count = 0;
	bossbullet_count = 0;
	rainbullet_count = 0;
	power_count = 0;
	bossmove = 0;
	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			bullet[x][y] = 0;
			ebullet[x][y] = 0;
			e2bullet[x][y] = 0;
			bossbullet[x][y] = 0;
			rainbullet[x][y] = 0;
			powers[x][y] = 0;
		}
	}

	enemy_count = 0;//적 초기화
	enemy2_count = 0;
	for (x = 0; x < WIDTH; x++) {
		for (y = 0; y < HEIGHT; y++) {
			enemy[x][y] = 0;
			enemy2[x][y] = 0;
			boss[x][y] = 0;
		}
	}

	//변수 초기화
	oldx, oldy, newx, newy;
	score = 0;
	bosslife = 500;
	called = 0;
	frame_count = 0; // frame_count로 속도 조절
	p_frame_sync = 1; // 1프레임 마다 이동
	p_frame_sync_start = 0;
	bullet_frame_sync = 1; // 1 프레임 마다 총알을 움직인다
	ebullet_move_frame_sync = 2; // 2 프레임마다 발사된 적의 총알 움직임
	enemy_frame_sync = 5; // 5 프레임 마다 적을 움직인다
	ebullet_frame_sync = 50; // 50 프레임마다 적의 총알 발사
	oldx, oldy, newx, newy; //bullet과 공유

	player(0);

	removeCursor();

	system("cls");
	cls(BLACK, WHITE);
	sprintf(cmd, "mode con cols=%d lines=%d", WIDTH, HEIGHT);
	system(cmd); // 사이즈 조정

}

int main()
{
	unsigned char ch; // 특수키 0xe0 을 입력받으려면 unsigned char 로 선언해야 함
	int keep_moving, move_flag = 0;
	int power_time = 0;
	int enemy_time = 0;
	int enemy2_time = 0;
	int rainbullet_time = 0;
	int run_time, start_time;
	char cmd[100];

START:

	system("cls");
	cls(BLACK, WHITE);
	sprintf(cmd, "mode con cols=%d lines=%d", WIDTH, HEIGHT);
	system(cmd); // 사이즈 조정
	removeCursor();
	
	while (1) {
		int c1, c2;
		do { // 색을 변경하면서 출력
			c1 = rand() % 15 + 1;
			c2 = rand() % 15 + 1;
		} while (c1 == c2);
		Sleep(300);
		removeCursor();
		textcolor(c1, BLACK);
		gotoxy(25, 10);
		printf("SHOOTING GAME");
		draw_box(19, 8, 41, 12, "□");
		gotoxy(15, 15);
		printf("--- PRESS (SPACEBAR) TO START ---");
		textcolor(WHITE, BLACK);
		gotoxy(13, 18);
		printf("W : UP / S : DOWN / A : LEFT / D : RIGHT");
		gotoxy(13, 20);
		printf("SPACEBAR : SOOT BULLET");

		gotoxy(13, 23);
		textcolor(GREEN2, BLACK);
		printf("=V=");
		textcolor(WHITE, BLACK);
		printf(" : 1 point");
		gotoxy(13, 25);
		textcolor(MAGENTA2, BLACK);
		printf("=Y=");
		textcolor(WHITE, BLACK);
		printf(" : 2 point");
		gotoxy(13, 27);
		textcolor(CYAN2, BLACK);
		printf("<P>");
		textcolor(WHITE, BLACK);
		printf(" : POWER + 1");
		textcolor(WHITE, BLACK);
		textcolor(WHITE, BLACK);

		if (_kbhit()) {
			ch = _getch();
			if (ch == ' ')
				break;
		}
	}
	system("cls");

	boss_count = 0;
	life = 5;
	power = 1;
	enemy_time = 0;
	enemy2_time = 0;
	power_time = 0;
BOSS:
	newx = oldx = 30;
	newy = oldy = 35;
	bossx = 20;
	init_game();
	start_time = (unsigned)time(NULL);
	removeCursor();
	showscore();
	if (boss_count == 1) {
		show_bosslife();
	}
	showlife();
	showpower();
	showendMessage();

	putplayer(oldx, oldy, PLAYER);
	ch = 0; //초기 값 정지상태
	keep_moving = 0;

	//game start
	while (1) {
		//적 나타나는 시간
		run_time = (unsigned)time(NULL) - start_time;
		if (run_time > power_time && (run_time % powerinterval == 0) && boss_count == 0) {
			show_power();
			power_time = run_time; // 마지막 power 표시 시간 기억
		}
		if (run_time > enemy_time && (run_time % enemyinterval == 0) && boss_count == 0) {
			show_enemy();
			enemy_time = run_time; // 마지막 enemy 표시 시간 기억
		}
		if (run_time > enemy2_time && (run_time % enemy2interval == 0) && boss_count == 0) {
			show_enemy2();
			enemy2_time = run_time; // 마지막 enemy2 표시 시간 기억
		}

		if (life <= 0) { //생명이 다 닳으면
			break;
		}
		if (bosslife <= 0) { //보스의 생명이 다 닳으면
			break;
		}
		if (_kbhit() == 1) {//키보드가 눌러져있으면
			ch = _getch(); //key값을 읽는다
			//esc누르면 종료
			if (ch == ESC) {
				gotoxy(15, 20);
				exit(0);
				break;
			}

			if (ch == UP || ch == DOWN || ch == LEFT || ch == RIGHT || ch == SPACE) {
				switch (ch) {
				case UP:
				case DOWN:
				case LEFT:
				case RIGHT:
					player(ch);
					keep_moving = 1;
					if (frame_count % p_frame_sync == 0)
						player(0);
					break;
				case SPACE:
					shotBullet();
					break;
				default: // WASD 가 아닌 경우 멈춘다
					if (frame_count % p_frame_sync == 0)
						player(0);
				}
			}
		}
		else {
			// keyboard 가 눌려지지 않으면 계속 움직인다.
			// 이동중이던 방향으로 계속 이동
			if (frame_count % p_frame_sync == 0)
				player(0);
		}
		if (frame_count % bullet_frame_sync == 0) {
			move_bullet(); // bullet 의 위치를 변경한다.
		}
		if (frame_count % ebullet_move_frame_sync == 0) {
			move_ebullet(); // ebullet 의 위치를 변경한다.
		}
		if (frame_count % e2bullet_move_frame_sync == 0) {
			move_e2bullet(); // e2bullet 의 위치를 변경한다.
		}
		if (frame_count % enemy_frame_sync == 0) {
			move_enemy(); // enemy 의 위치를 변경한다.
		}
		if (frame_count % enemy2_frame_sync == 0) {
			move_enemy2(); // enemy2 의 위치를 변경한다.
		}
		if (frame_count % power_frame_sync == 0) { // power가 내 총알이 아닌 다른 것에 닿아 화면에서 사라지는 것을 방지 하기 위해 
			move_power();
		}
		if (boss_count == 1) {
			if (frame_count % boss_frame_sync == 0) {
				move_boss(); // boss 의 위치를 변경한다.
				move_bossbullet(); // bossbullet 위치를 변경한다.
				move_rainbullet(); // rainbullet 위치를 변경
			}
			if (frame_count % bulletrain_frame_sync == 0) {
				shot_rainbullet();
			}
		}
		Sleep(Delay); // Delay 값을 줄이고
		frame_count++; // frame_count 값으로 속도 조절을 한다.

		if (score >= 30) { //점수가 30 점이 넘으면 보스 등장
			if (boss_count == 0) {
				system("cls");
				while (1) {
					int c1, c2;
					do { // 색을 변경하면서 출력
						c1 = rand() % 15 + 1;
						c2 = rand() % 15 + 1;
					} while (c1 == c2);
					Sleep(300);
					textcolor(c1, BLACK);
					gotoxy(25, 13);
					printf("!!! BOSS !!!");
					gotoxy(25, 15);
					printf("!!! BOSS !!!");
					gotoxy(25, 17);
					printf("!!! BOSS !!!");
					textcolor(CYAN2, BLACK);
					gotoxy(24, 20);
					printf("YOUR POWER : %d", power);
					textcolor(c1, BLACK);
					gotoxy(19, 22);
					printf("--- PRESS (S) TO START ---");
					textcolor(WHITE, BLACK);
					if (_kbhit()) {
						ch = _getch();
						if (ch == 's')
							break;
					}
				}
			}
			boss_count = 1;
			score = 0;
			system("cls");
			removeCursor();
			show_boss();
			goto BOSS;
		}

	}

	//end
	while (1) {
		int c1, c2;
		do { // 색을 변경하면서 Game Over 출력
			c1 = rand() % 15 + 1;
			c2 = rand() % 15 + 1;
		} while (c1 == c2);
		Sleep(200);
		if (boss_count == 0) {
			textcolor(c1, BLACK);
			gotoxy(22, 28);
			printf("** Game Over **");
		}
		else if (boss_count == 1) {
			remove_boss();
			boss_die();
			textcolor(c1, BLACK);
			gotoxy(18, 28);
			printf("*** !!! YOU WIN !!! ***");
		}
		else {
			textcolor(c1, BLACK);
			gotoxy(22, 28);
			printf("** Game Over **");
		}

		gotoxy(14, 30);
		printf("Hit (R) to Restart (ESC) to Quit");
		if (_kbhit()) {
			ch = _getch();
			if (ch == 'r' || ch == ESC)
				break;
		}
	}
	if (ch == 'r') {
		boss_count = 0;
		goto START;
	}
	gotoxy(0, HEIGHT - 1);
}

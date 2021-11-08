#include<stdio.h>
#include<graphics.h>
#include<time.h>

#define BULLET_NUM	15
#define ENEMY_NUM	10

#define ENEMY_SMALL		0
#define ENEMY_MIDDLE	1
#define ENEMY_BIG		2

enum window
{
	width = 480,
	height = 700,
};

struct plane
{
	int x;
	int y;
	bool alive;	//是否存活
	int width;	//图片宽度
	int height;	//图片高度
	int hp;		//血量
	int type;	//敌机类型
	int fall;	//陨落
}player,bullet[BULLET_NUM],enemy[ENEMY_NUM];

IMAGE image_background;
IMAGE image_player[2];
IMAGE image_bullet;
IMAGE image_enemy[3];
IMAGE image_enemy_fall[3][6];

void drawAlpha(IMAGE* picture, int  picture_x, int picture_y); //x为要载入图片的X坐标，y为Y坐标

bool timer_arrive(int id, int ms)
{
	static DWORD t[100];
	if (clock() - t[id] > ms)
	{
		t[id] = clock();
		return true;
	}
	return false;
}


//导入图片
void load_image(void)
{
	loadimage(&image_background, "./images/background.png");
	//加载选手图片
	loadimage(&image_player[0],"./images/me1.png");
	loadimage(&image_player[1], "./images/me2.png");
	//加载子弹
	loadimage(&image_bullet, "./images/bullet1.png");
	//加载敌机图片
	loadimage(&image_enemy[0], "./images/enemy1.png");
	loadimage(&image_enemy[1], "./images/enemy2.png");
	loadimage(&image_enemy[2], "./images/enemy3_n1.png");


	loadimage(&image_enemy_fall[0][0], "./images/enemy1_down1.png");
	loadimage(&image_enemy_fall[0][1], "./images/enemy1_down2.png");
	loadimage(&image_enemy_fall[0][2], "./images/enemy1_down3.png");
	loadimage(&image_enemy_fall[0][3], "./images/enemy1_down4.png");

	loadimage(&image_enemy_fall[1][0], "./images/enemy2_down1.png");
	loadimage(&image_enemy_fall[1][1], "./images/enemy2_down2.png");
	loadimage(&image_enemy_fall[1][2], "./images/enemy2_down3.png");
	loadimage(&image_enemy_fall[1][3], "./images/enemy2_down4.png");

	loadimage(&image_enemy_fall[2][0], "./images/enemy3_down1.png");
	loadimage(&image_enemy_fall[2][1], "./images/enemy3_down2.png");
	loadimage(&image_enemy_fall[2][2], "./images/enemy3_down3.png");
	loadimage(&image_enemy_fall[2][3], "./images/enemy3_down4.png");
	loadimage(&image_enemy_fall[2][4], "./images/enemy3_down3.png");
	loadimage(&image_enemy_fall[2][5], "./images/enemy3_down4.png");

}

//生成敌机参数
void enemy_hp(int index)
{
	int num = rand();
	if (num % 20 == 0)
	{
		enemy[index].type = ENEMY_BIG;
		enemy[index].hp = 3;
		enemy[index].height = 258;
		enemy[index].width = 169;
	}
	else if (num % 20 == 2 || num % 20 == 4)
	{
		enemy[index].type = ENEMY_MIDDLE;
		enemy[index].hp = 2;
		enemy[index].height = 99;
		enemy[index].width = 69;
	}
	else
	{
		enemy[index].type = ENEMY_SMALL;
		enemy[index].hp = 1;
		enemy[index].height = 43;
		enemy[index].width = 57;
	}
}

//游戏初始化
void game_init(void)
{
	initgraph(width, height);
	player.x = width / 2 - 51;
	player.y = height - 126;
	player.alive = true;

	for (int i = 0; i < BULLET_NUM; i++)
	{
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].alive = false;
	}

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].alive = false;
		enemy_hp(i);
	}

}

//游戏绘制函数
void game_draw(void)
{
	static UINT16 cnt;
	static int fall_cnt[ENEMY_NUM];
	int plane_type, plane_step;

	load_image();
	putimage(0, 0, &image_background);

	if (timer_arrive(0, 10))
	{
		cnt = (cnt + 1) % 10;
	}

	if (cnt < 5)
	{
		drawAlpha(&image_player[0], player.x, player.y);
	}
	else
	{
		drawAlpha(&image_player[1], player.x, player.y);
	}

	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].alive)
		{
			drawAlpha(&image_bullet, bullet[i].x, bullet[i].y);
		}
	}
	//绘制敌机
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].alive)
		{
			if (enemy[i].type == ENEMY_BIG)
			{
				drawAlpha(&image_enemy[2], enemy[i].x, enemy[i].y);
			}
			else if (enemy[i].type == ENEMY_MIDDLE)
			{
				drawAlpha(&image_enemy[1], enemy[i].x, enemy[i].y);
			}
			else if (enemy[i].type == ENEMY_SMALL)
			{
				drawAlpha(&image_enemy[0], enemy[i].x, enemy[i].y);
			}
		}

		if (enemy[i].fall)
		{
			if (timer_arrive(20+i, 250))
			{
				fall_cnt[i]++;
				plane_type = enemy[i].type;
				plane_step = fall_cnt[i];
				drawAlpha(&image_enemy_fall[plane_type][plane_step], enemy[i].x, enemy[i].y);
				
				if (fall_cnt[i] > 3)
				{
					enemy[i].fall = false;
					fall_cnt[i] = 0;
				}
			}
		}
	}

}




void creat_bullet(void)
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (!bullet[i].alive)
		{
			bullet[i].x = player.x+60;
			bullet[i].y = player.y;
			bullet[i].alive = true;
			
			break;
		}
	}
}

void creat_enemy(void)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].alive)
		{
			enemy[i].x = rand()% (width - 100);
			enemy[i].y = rand()%150;
			enemy[i].alive = true;
			enemy[i].fall = false;
			enemy_hp(i);
			break;
		}
	}
}


void bullet_move(int speed)
{
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].alive)
		{
			bullet[i].y -= speed;

			if (bullet[i].y < 0)
			{
				bullet[i].x = 0;
				bullet[i].y = 0;
				bullet[i].alive = false;
			}
		}
	}

}

void enemy_move(int speed)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].alive)
		{
			enemy[i].y += speed;

			if (enemy[i].y > height)
			{
				enemy[i].alive = false;
			}
		}
	}
}

void attack_plane(void)
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].alive)
		{
			continue;
		}

		for (int j = 0; j < BULLET_NUM; j++)
		{
			if (!bullet[j].alive)
			{
				continue;
			}

			if (bullet[j].x > enemy[i].x && bullet[j].x < enemy[i].x+enemy[i].width
			&& bullet[j].y > enemy[i].y && bullet[j].y < enemy[i].y + enemy[i].height)
			{
				bullet[j].alive = false;
				enemy[i].hp--;

				if (enemy[i].hp <= 0)
				{
					enemy[i].alive = false;
					enemy[i].fall = true;
				}

			}
		}



	}
}

//角色移动，上下左右
void player_move(int speed)
{
	if (GetAsyncKeyState(VK_UP))
	{
		player.y -= speed;
		player.y = (player.y < 0) ? 0 : player.y;
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		player.y += speed;
		player.y = (player.y >= height - 126) ? height - 126 : player.y;
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		player.x -= speed;
		player.x = (player.x < -51) ? -51 : player.x;
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		player.x += speed;
		player.x = (player.x >= width-51) ? width -51 : player.x;
	}

	if (GetAsyncKeyState(VK_SPACE) && timer_arrive(2, 100))
	{
		creat_bullet();
	}
}

int main(void)
{
	game_init();
	//双缓冲显示
	BeginBatchDraw();
	while (1)
	{
		game_draw();
		FlushBatchDraw();
		player_move(5);
		bullet_move(2);
		if (timer_arrive(1,1000))
		{
			creat_enemy();
		}

		if (timer_arrive(3, 10))
		{
			enemy_move(1);
		}

		attack_plane();
	}
	EndBatchDraw();
	return 0;
}

// 载入PNG图并去透明部分
void drawAlpha(IMAGE * picture, int  picture_x, int picture_y) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				if ((ix + picture_x) >= 0 && (ix + picture_x) <= graphWidth)	//防止出边界后循环显示
				{
					dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
					int dr = ((dst[dstX] & 0xff0000) >> 16);
					int dg = ((dst[dstX] & 0xff00) >> 8);
					int db = dst[dstX] & 0xff;
					draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
						| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
						| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
				}
			}
		}
	}
}


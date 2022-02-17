#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

// 미사일 구조체
struct BULLET
{
	int x; // 미사일 x 좌표
	int y; // 미사일 y 좌표
	bool fire; // 미사일 발사 상태
};

//배열로 배경을 좌표를 만든다
char bg[25][80];

//플레이어 좌표
int playerX;
int playerY;

//적 좌표
int enemyX;
int enemyY;

// 미사일 갯수
BULLET playerBullet[20];

// 함수 설계
void ClearScreen(); // 화면을 지운다
void GameMain(); // 게임 전체를 관리하는 함수
void PrintScreen(); // 화면을 그려주는 함수
void KeyControl(); //키 관련된 함수
void BulletDraw(); // 미사일을 그려주는 함수
void PlayerDraw(); //플레이어 그리는 함수
void EnemyDraw(); // 적 그리는 함수
void EnemyMove(); //적 움직이는 함수
void ClashEnemyAndBullet(); // 충돌처리 함수

int main()
{
	/// 랜덤 함수 준비
	srand((unsigned)time(NULL));
	// 플레이어 좌표 위치
	playerX = 0;
	playerY = 12;

	// 적 좌표 위치
	enemyX = 77;
	enemyY = 12;

	// 플레이어 미사일  초기화 (미사일 발사전 준비 상태)
	for (int i = 0; i < 20; i++)
	{
		playerBullet[i].x = 0;
		playerBullet[i].y = 0;
		playerBullet[i].fire = false;
	}

	//현재 초단위 시간을 받아온다
	int dwTime = GetTickCount();

	// 게임 실행 무한 루프
	while (true)
	{
		//0.05초 지연
		if (dwTime + 50 < GetTickCount())
		{
			dwTime = GetTickCount();
			// 지우고
			ClearScreen();
			// 플레이어나 적이 움직이고
			GameMain();
			//그려주고
			PrintScreen();
		}
	}
	return 0;
}

void ClearScreen()
{
	// 콘솔창에 그려진 모든 걸 지움 
	system("cls");

	//배열에 공백으로 넣어서 지움
	for (int y = 0; y < 25; y++)
	{
		for (int x = 0; x < 80; x++)
		{
			bg[y][x] = ' ';
		}
	}
}

void GameMain()
{
	// 키를 입력하는 부분
	KeyControl();
	// 미사일ㅇ를 그려준다
	BulletDraw();
	// 플레이어를 그려준다
	PlayerDraw();
	// 적을 그려준다
	EnemyMove();
	// 적을 그려주는 부분 
	EnemyDraw();
	// 미사일 적의 충돌
	ClashEnemyAndBullet();
}

void PrintScreen()
{
	bg[24][79] = '\0'; // 맨 마지막 배열에 널 문자를 넣어준다
	printf("%s", bg); // 문자열로 전체 배경을 표현한다 

	// 24 에 79까지 사용
}

void KeyControl()
{
	char pressKey;

	//키가 입력되면 잡아준다
	if (_kbhit())
	{
		pressKey = _getch(); // 키가 입력되어 변수에 저장
		switch (pressKey)
		{
			case 72: //위쪽 방향
				playerY--;
				if (playerY < 1)
					playerY = 1;
				break;
			case 75:
				if (playerX < 0)
					playerX = 0;
				break;
			case 77: // 오른쪽
				playerX++;
				if (playerX > 75)
					playerX = 75;
				break;
			case 80:
				//아래쪽 방향키
				playerY++;
				if (playerY > 23)
					playerY = 23;
				break;
			case 32:
				// 스페이스바 키가 눌리면
				// 스페이스바 키가 눌리면 총알 발사
				for (int i = 0; i < 20; i++)
				{
					// 미사일이 준비되어 있는 상태인 경우에만 발사 가능 
					if (playerBullet[i].fire == false)
					{
						playerBullet[i].fire = true;
						// 플레이어 앞에서 미사일을 쏘기 +5
						playerBullet[i].x = playerX + 5;
						playerBullet[i].y = playerY;
						// 한발만 발사하기
						break;
					}
				}
				break;
		}
	}
}

void BulletDraw()
{
	// 미사일 전체 20개 중에
	for (int i = 0; i < 20; i++)
	{
		// 플레이어 미사일 날아가고 있는 상태 true
		if (playerBullet[i].fire == true)
		{
			// 미사일을 그린다
			bg[playerBullet[i].y][playerBullet[i].x - 1] = '-';
			bg[playerBullet[i].y][playerBullet[i].x - 0] = '>';
			// 미사일 앞으로 +1 
			playerBullet[i].x++;

			//미사일이 80을 넘어갈 경우
			if (playerBullet[i].x > 79)
			{
				//미사일 준비상태로 전환
				playerBullet[i].fire = false;
			}

		}
	}
}

void PlayerDraw()
{
	bg[playerY - 1][playerX + 1] = '>';
	bg[playerY - 1][playerX + 0] = '-';
	bg[playerY + 0][playerX + 1] = '>';
	bg[playerY + 0][playerX + 2] = '>';
	bg[playerY + 0][playerX + 3] = '>';
	bg[playerY + 1][playerX + 0] = '-';
	bg[playerY + 1][playerX + 1] = '>';


}

void EnemyDraw()
{
	bg[enemyY][enemyX - 2] = '<';
	bg[enemyY][enemyX - 1] = '-';
	bg[enemyY][enemyX + 0] = 'O';
	bg[enemyY][enemyX + 1] = '-';
	bg[enemyY][enemyX + 2] = '>';
}

void EnemyMove()
{
	/// 적은 왼쪽으로 오게 한다
	enemyX--;

	//적이 왼쪽으로 다 올 경우
	if (enemyX < 2)
	{
		// 적을 랜덤하게 Y쪽 좌표를 바꿔준다
		enemyX = 77;
		enemyY = (rand() % 20) + 2;
	}
}

//충돌 처리 
void ClashEnemyAndBullet()
{
	//미사일 20개 전체 판별
	for (int i = 0; i < 20; i++)
	{
		//미사일이 살아있고
		if (playerBullet[i].fire == true)
		{
			// 적과 미사일의 y 값이 같고
			if (playerBullet[i].y == enemyY)
			{
				// 플레이어 미사일 X 값이 같은 지 판별
				// X 값은 적의 크기 만큼 X-1 0 X+1 까지 세개 좌표로 잘 충돌되게 판별


				if (playerBullet[i].x >= (enemyX - 1)
					&& playerBullet[i].x <= (enemyX + 1))
				{
					// 충돌되면 적 y값 바꿔줌
					enemyX = 77;
					enemyY = (rand() % 20) + 2;
					playerBullet[i].fire = false;
				}
			}
		}
	}
}

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <cstdlib>
#include <string>


using namespace std;

//刪除上一個畫面
void Clear(){
  COORD scrn;
  HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
  scrn.X = 0;
  scrn.Y = 0;
  SetConsoleCursorPosition(hOuput,scrn);
  return;
}


//遊戲初始化
void SetUp(bool *gameover, int *headX, int *headY, const int width, const int length, int *foodX, int *foodY, int *obstacleX, int *obstacleY, int *powerX, int *powerY,  char *direction){

    //遊戲未結束
    *gameover= 0;

    //蛇頭座標初始化在地圖中心
    *headX= width/2;
    *headY= length/2;

  //direction 設置為暫停狀態
  *direction= 'p';

  //使用 rand() 隨機生成食物座標
    flag1:
    *foodX= rand() % (width -1) +1;
    *foodY= rand() % (length -1) +1;
    if( *headX==*foodX && *headY==*foodY ) goto flag1; //食物與起點重疊時，重新產生食物

  //使用 rand() 隨機生成障礙座標
    flag2:
    *obstacleX= rand() % (width -1) +1;
    *obstacleY= rand() % (length -1) +1;
    if( *headX==*obstacleX && *headY==*obstacleY ) goto flag2; //障礙與起點重疊時，重新產生障礙

  //使用 rand() 隨機生成道具座標
    flag3:
    *powerX= rand() % (width -1) +1;
    *powerY= rand() % (length -1) +1;
    if( *headX==*powerX && *headY==*powerY ) goto flag2; //道具與起點重疊時，重新產生道具

	return;
}

//繪製遊戲畫面
void Draw(bool SpeedOn, int *headX, int *headY, const int width, const int length, int lives, int score, int *foodX, int *foodY, int *obstacleX, int *obstacleY, int *powerX, int *powerY, int **bodyX, int **bodyY, int current_length){ //產生遊戲畫面


    if(SpeedOn== true){
        Sleep(50);
    }
    else{
        Sleep(150);
    }
    Clear();
    //繪製上半部邊框
    for(int i=0; i< width+2 ; i++){
        cout << "#";
    }
    cout << endl;


    for(int i=0; i<length ; i++){
        for(int j=0; j<width+1 ; j++){

        if(j==0) cout << "#"; // 左邊界
        if (j == width ) cout << "#"; // 右邊界

        if( j==(*headX) && i==(*headY) ) cout << "O";  // 蛇頭

        else if (j == (*foodX) && i == *foodY) cout << "*";  // 食物

        else if (j == *obstacleX && i == *obstacleY) cout << "X";  // 障礙物

        else if (j == *powerX && i == *powerY) cout << "+";  // 增強道具

        else{
            bool printSnake= false;
            for(int k=0; k<current_length; k++){
                if((*bodyX)[k]== j && (*bodyY)[k]== i){
                    cout << "o";  // 蛇身
                    printSnake= true;

                    }
            }
            if(!printSnake) cout << " ";  // 空白區域

        }


        }
        cout << endl;
    }

    //繪製下半部邊框
    for(int i=0; i<width+2 ; i++){
        cout << "#";
    }
    cout << endl;

    //記錄分數
    cout << "Score: " << score << endl;

    //記錄生命
    cout << "Lives: " << lives << endl;




	return;
}


void Input(char *direction){ //偵測玩家使用WASD變向

    if(kbhit()){
        *direction= getch();
    }

	return;
}

void handleCollision(bool *gameover, int *lives){

        //發出警報音效
        cout<< "\a";

        //扣除生命
        (*lives)--;

        //生命歸零則遊戲結束
        if((*lives)==0){
            (*gameover)= 1;

        }
        return;

}


void applyPowerUp(bool *SpeedOn, int kind, int *current_length, int *lives) {

    int Speed=0, Shrink=1, ExtraLife=2;

    switch(kind){
        case 0:
            *SpeedOn= true;
            Sleep(50);
            break;

        case 1:
            (*current_length)--;
            break;

        case 2:
            (*lives)++;
            break;


    }

    return;
}

void Algorithm(bool *SpeedOn, bool *gameover, int *headX, int *headY, const int width, const int length, int *lives, int *score, int *foodX, int *foodY, int *obstacleX, int *obstacleY, int *powerX, int *powerY, int **bodyX, int **bodyY, int *current_length, char *direction){

    //記錄蛇身前一個位置
    int preX= (*bodyX)[0];
    int preY= (*bodyY)[0];
    int pre2X, pre2Y;
    (*bodyX)[0]= *headX;
    (*bodyY)[0]= *headY;


    //移動蛇身
    for(int i=1; i<*current_length; i++){
        pre2X= (*bodyX)[i];
        pre2Y= (*bodyY)[i];

        (*bodyX)[i]= preX;
        (*bodyY)[i]= preY;

        preX= pre2X;
        preY= pre2Y;
    }



    //蛇頭轉向
    switch(*direction){
        case 'w'://Up
            (*headY)--;
            break;

        case 'a'://Left
            (*headX)--;
            break;

        case 's'://Down
            (*headY)++;
            break;

        case 'd'://Right
            (*headX)++;
            break;

        default:
            break;


    }

    //邊界碰撞檢測: 碰撞後生命不為零則從另一側穿回
    if((*headX)>= width){
        handleCollision(&(*gameover), &(*lives));
        if((*gameover)==1) return;
        else (*headX)= 0;

    }

    else if((*headX) < 0){
        handleCollision(&(*gameover), &(*lives));
        if((*gameover)==1) return;
        else (*headX)= width-1;

    }

    if((*headY)>= length){
        handleCollision(&(*gameover), &(*lives));
        if((*gameover)==1) return;
        else (*headY)= 0;

    }

    else if((*headY) < 0){
        handleCollision(&(*gameover), &(*lives));
        if((*gameover)==1) return;
        else (*headY)= length-1;

    }

    //碰撞自身檢測:
    for(int i=0; i<*current_length; i++){
        if( (*bodyX)[i]==(*headX) && (*bodyY)[i]==(*headY)){
            handleCollision(&(*gameover), &(*lives));
            if((*gameover)==1) return;

            //碰撞後生命不為零則回到起點
            else{
                *headX= width/2;
                *headY= length/2;
                *direction= 'p';
            }
        }

    }

    // 檢測是否吃到食物
    if((*foodX)==(*headX) && (*foodY)==(*headY)){
        (*score)+= 20; //增加分數
        (*current_length)++; //延長蛇身
        *foodX= rand() % (width -1) +1; //重新生成食物
        *foodY= rand() % (length -1) +1; //重新生成食物
    }

    // 檢測是否撞到障礙
    if((*obstacleX)==(*headX) && (*obstacleY)==(*headY)){
        (*score)-= 20; //減少分數
        if((*current_length)!=0) (*current_length)--; //縮短蛇身
        else handleCollision(&(*gameover), &(*lives));
        *obstacleX= rand() % (width -1) +1; //重新生成障礙
        *obstacleY= rand() % (length -1) +1; //重新生成障礙
    }

    // 檢測是否吃到增強道具
    if((*powerX)==(*headX) && (*powerY)==(*headY)){
        *SpeedOn= false; //重置道具狀態

        flag4:
        int kind= rand() %3;
        if( kind==1 && (*current_length)==0) goto flag4;//長度無法減少時，不會觸發縮小道具


        applyPowerUp(&*SpeedOn, kind, &*current_length, &*lives);
        *powerX= rand() % (width -1) +1; //重新生成道具
        *powerY= rand() % (length -1) +1; //重新生成道具
    }


	return;
}




// 詢問是否重新開始
int TryAgain() {
    char choice;
    cout << "Do you want to start again?(y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') return 1;
    else return 0;
}

void RuleDescription(){

    cout << "Welcome to SNAKE." << endl;
    cout << "During the game, you move the snake by controlling its direction with the keyboard." << endl;
    cout << "(w: up, a: left, s: down, d: right)" << endl << endl;
    cout << "Meaning of symbols in the game map:" << endl;
    cout << "O: the head of the snake" << endl;
    cout << "o: the snake's body" << endl;
    cout << "#: map border" << endl;
    cout << "The snake must avoid hitting itself or the map boundary," << endl;
    cout << "otherwise its life will be reduced by one." << endl << endl;
    cout << "X: Obstacle" << endl;
    cout << "If you hit an “X”, 20 points are deducted from your score." << endl;
    cout << "At the same time, the snake's length is shortened by one." << endl;
    cout << "When the snake is at its shortest length, your life will be deducted." << endl << endl;
    cout << "*: Food" << endl;
    cout << "If you hit a “*”, your score is increased by 20 points." << endl;
    cout << "At the same time, the snake's length is increased by one." << endl << endl;
    cout << "+: Enhancements" << endl;
    cout << "Touching a “+” will trigger one of the following three items at random." << endl;
    cout << "Accelerator: Temporarily increases the speed of the snake, making it more difficult to maneuver." << endl;
    cout << "Shrink: Shrink the length of the snake to give the player more room to move." << endl;
    cout << "Extra Life: Gain an extra chance to continue the game when you hit a wall or your own body." << endl << endl;
    cout << "The game ends when your life is zero." << endl;
    cout << "Reach 120 points and you win the game." << endl;
    cout << "This is the end of the introduction." << endl << endl << endl;


}

void MapSize(int *width, int *length){

    cout << "Please enter the width and length of the map you want to play." << endl;
    cout << "The width and length can be different, and the minimum is 5." << endl;

    while(1){
        cout << "Width:  Length:  " << endl;
        cin >>  *width >>  *length;
        if(*width>=5 && *length>=5) break;
        else cout << "The minimum length and width is 5, please re-enter." << endl;

    }
    return;
}

void ScoreJudgment(int score){
    // 檢查是否勝利
    if (score >= 120) {
        cout << "Congratulations on your game win! " << endl;

    }
    else cout << "Game over! Your score is: " << score << endl;
    return;


}

int main(){

    //介紹遊戲規則
    RuleDescription();

    //設定地圖大小
    int width, length;
    restart:
    MapSize(&width, &length);

    //清除以上文字
    std::system("cls");

    //初始化亂數表以作出隨機亂數
    srand(time(NULL));

    //宣告所需變數
    int headX=0, headY=0, foodX=0, foodY=0, obstacleX=0, obstacleY=0, powerX=0, powerY=0;
    int current_length=0, score=20;
    int *bodyX = (int *) malloc(2000 * sizeof(int));
    int *bodyY = (int *) malloc(2000 * sizeof(int));

    bool SpeedOn= false;
    char direction= 'p';
    int lives=1;
    bool gameover= 0;

    SetUp( &gameover, &headX, &headY, width, length, &foodX, &foodY, &obstacleX, &obstacleY, &powerX, &powerY, &direction);


    while(!gameover){
        Draw(SpeedOn, &headX, &headY, width, length, lives, score, &foodX, &foodY, &obstacleX, &obstacleY, &powerX, &powerY, &bodyX, &bodyY, current_length);
        Input(&direction);
        Algorithm(&SpeedOn, &gameover, &headX, &headY, width, length, &lives, &score, &foodX, &foodY, &obstacleX, &obstacleY, &powerX, &powerY, &bodyX, &bodyY, &current_length, &direction);

    }
    Draw(SpeedOn, &headX, &headY, width, length, lives, score, &foodX, &foodY, &obstacleX, &obstacleY, &powerX, &powerY, &bodyX, &bodyY, current_length);
    free(bodyX);
    free(bodyY);
    ScoreJudgment(score);


    if(TryAgain()==1) goto restart;


    return 0;
}

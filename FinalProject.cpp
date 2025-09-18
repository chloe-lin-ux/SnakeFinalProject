#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <cstdlib>
#include <string>


using namespace std;

//�R���W�@�ӵe��
void Clear(){
  COORD scrn;
  HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
  scrn.X = 0;
  scrn.Y = 0;
  SetConsoleCursorPosition(hOuput,scrn);
  return;
}


//�C����l��
void SetUp(bool *gameover, int *headX, int *headY, const int width, const int length, int *foodX, int *foodY, int *obstacleX, int *obstacleY, int *powerX, int *powerY,  char *direction){

    //�C��������
    *gameover= 0;

    //�D�Y�y�Ъ�l�Ʀb�a�Ϥ���
    *headX= width/2;
    *headY= length/2;

  //direction �]�m���Ȱ����A
  *direction= 'p';

  //�ϥ� rand() �H���ͦ������y��
    flag1:
    *foodX= rand() % (width -1) +1;
    *foodY= rand() % (length -1) +1;
    if( *headX==*foodX && *headY==*foodY ) goto flag1; //�����P�_�I���|�ɡA���s���ͭ���

  //�ϥ� rand() �H���ͦ���ê�y��
    flag2:
    *obstacleX= rand() % (width -1) +1;
    *obstacleY= rand() % (length -1) +1;
    if( *headX==*obstacleX && *headY==*obstacleY ) goto flag2; //��ê�P�_�I���|�ɡA���s���ͻ�ê

  //�ϥ� rand() �H���ͦ��D��y��
    flag3:
    *powerX= rand() % (width -1) +1;
    *powerY= rand() % (length -1) +1;
    if( *headX==*powerX && *headY==*powerY ) goto flag2; //�D��P�_�I���|�ɡA���s���͹D��

	return;
}

//ø�s�C���e��
void Draw(bool SpeedOn, int *headX, int *headY, const int width, const int length, int lives, int score, int *foodX, int *foodY, int *obstacleX, int *obstacleY, int *powerX, int *powerY, int **bodyX, int **bodyY, int current_length){ //���͹C���e��


    if(SpeedOn== true){
        Sleep(50);
    }
    else{
        Sleep(150);
    }
    Clear();
    //ø�s�W�b�����
    for(int i=0; i< width+2 ; i++){
        cout << "#";
    }
    cout << endl;


    for(int i=0; i<length ; i++){
        for(int j=0; j<width+1 ; j++){

        if(j==0) cout << "#"; // �����
        if (j == width ) cout << "#"; // �k���

        if( j==(*headX) && i==(*headY) ) cout << "O";  // �D�Y

        else if (j == (*foodX) && i == *foodY) cout << "*";  // ����

        else if (j == *obstacleX && i == *obstacleY) cout << "X";  // ��ê��

        else if (j == *powerX && i == *powerY) cout << "+";  // �W�j�D��

        else{
            bool printSnake= false;
            for(int k=0; k<current_length; k++){
                if((*bodyX)[k]== j && (*bodyY)[k]== i){
                    cout << "o";  // �D��
                    printSnake= true;

                    }
            }
            if(!printSnake) cout << " ";  // �ťհϰ�

        }


        }
        cout << endl;
    }

    //ø�s�U�b�����
    for(int i=0; i<width+2 ; i++){
        cout << "#";
    }
    cout << endl;

    //�O������
    cout << "Score: " << score << endl;

    //�O���ͩR
    cout << "Lives: " << lives << endl;




	return;
}


void Input(char *direction){ //�������a�ϥ�WASD�ܦV

    if(kbhit()){
        *direction= getch();
    }

	return;
}

void handleCollision(bool *gameover, int *lives){

        //�o�Xĵ������
        cout<< "\a";

        //�����ͩR
        (*lives)--;

        //�ͩR�k�s�h�C������
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

    //�O���D���e�@�Ӧ�m
    int preX= (*bodyX)[0];
    int preY= (*bodyY)[0];
    int pre2X, pre2Y;
    (*bodyX)[0]= *headX;
    (*bodyY)[0]= *headY;


    //���ʳD��
    for(int i=1; i<*current_length; i++){
        pre2X= (*bodyX)[i];
        pre2Y= (*bodyY)[i];

        (*bodyX)[i]= preX;
        (*bodyY)[i]= preY;

        preX= pre2X;
        preY= pre2Y;
    }



    //�D�Y��V
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

    //��ɸI���˴�: �I����ͩR�����s�h�q�t�@����^
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

    //�I���ۨ��˴�:
    for(int i=0; i<*current_length; i++){
        if( (*bodyX)[i]==(*headX) && (*bodyY)[i]==(*headY)){
            handleCollision(&(*gameover), &(*lives));
            if((*gameover)==1) return;

            //�I����ͩR�����s�h�^��_�I
            else{
                *headX= width/2;
                *headY= length/2;
                *direction= 'p';
            }
        }

    }

    // �˴��O�_�Y�쭹��
    if((*foodX)==(*headX) && (*foodY)==(*headY)){
        (*score)+= 20; //�W�[����
        (*current_length)++; //�����D��
        *foodX= rand() % (width -1) +1; //���s�ͦ�����
        *foodY= rand() % (length -1) +1; //���s�ͦ�����
    }

    // �˴��O�_�����ê
    if((*obstacleX)==(*headX) && (*obstacleY)==(*headY)){
        (*score)-= 20; //��֤���
        if((*current_length)!=0) (*current_length)--; //�Y�u�D��
        else handleCollision(&(*gameover), &(*lives));
        *obstacleX= rand() % (width -1) +1; //���s�ͦ���ê
        *obstacleY= rand() % (length -1) +1; //���s�ͦ���ê
    }

    // �˴��O�_�Y��W�j�D��
    if((*powerX)==(*headX) && (*powerY)==(*headY)){
        *SpeedOn= false; //���m�D�㪬�A

        flag4:
        int kind= rand() %3;
        if( kind==1 && (*current_length)==0) goto flag4;//���׵L�k��֮ɡA���|Ĳ�o�Y�p�D��


        applyPowerUp(&*SpeedOn, kind, &*current_length, &*lives);
        *powerX= rand() % (width -1) +1; //���s�ͦ��D��
        *powerY= rand() % (length -1) +1; //���s�ͦ��D��
    }


	return;
}




// �߰ݬO�_���s�}�l
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
    cout << "If you hit an ��X��, 20 points are deducted from your score." << endl;
    cout << "At the same time, the snake's length is shortened by one." << endl;
    cout << "When the snake is at its shortest length, your life will be deducted." << endl << endl;
    cout << "*: Food" << endl;
    cout << "If you hit a ��*��, your score is increased by 20 points." << endl;
    cout << "At the same time, the snake's length is increased by one." << endl << endl;
    cout << "+: Enhancements" << endl;
    cout << "Touching a ��+�� will trigger one of the following three items at random." << endl;
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
    // �ˬd�O�_�ӧQ
    if (score >= 120) {
        cout << "Congratulations on your game win! " << endl;

    }
    else cout << "Game over! Your score is: " << score << endl;
    return;


}

int main(){

    //���йC���W�h
    RuleDescription();

    //�]�w�a�Ϥj�p
    int width, length;
    restart:
    MapSize(&width, &length);

    //�M���H�W��r
    std::system("cls");

    //��l�ƶüƪ�H�@�X�H���ü�
    srand(time(NULL));

    //�ŧi�һ��ܼ�
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

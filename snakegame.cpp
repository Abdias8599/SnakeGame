#include <iostream>
#include <cstdlib>
#include <ncurses.h> // input output
#include <unistd.h> // sleep function

//global variables
bool gameOver;
const int WIDTH = 20;
const int HEIGHT = 20;

//creates snake head
int x, y;
int fruitX, fruitY;
int score;

typedef enum DIRECTION {STOP =0, LEFT, RIGHT, UP, DOWN}
Dir;
Dir dir;

//creates snake tail
int tailX[50], tailY[50];
int numTail=0;

void Initialize()
{
    // functions from ncurses api
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    gameOver = false;
    dir = STOP;
    x = WIDTH/2; 
    y = HEIGHT/2; 
    fruitX = (rand() % WIDTH)+1; 
    fruitY = (rand() % HEIGHT)+1; 
    score = 0;  
}



void Render()
{
    //calls clear
    clear();
    
    for (int i=0; i<HEIGHT+2; i++)
    {
        for (int j=0; j<WIDTH+2; j++)
        {
            if (i==0 | i==21) //prints map
            {
                mvprintw(i, j, "-");
            }
            else if (j==0 | j==21)
            {
                mvprintw(i, j, "|");
            } 
            else if (i==y && j==x) //snake head
            {
                mvprintw(i, j, "0");
            }
            else if (i==fruitY && j==fruitX) //fruit
            {
                mvprintw(i, j, "@");
            }
            else
            {   
                for (int k=0; k<numTail; k++) //draws the tail
                {
                    if (tailX[k]==j && tailY[k]==i)
                    {
                        mvprintw(i,j, "o");
                    }
                }

            }
            
        }
    }    

    //prints out game information and how to play
    mvprintw(3, 29, "------------------------------------");
    mvprintw(4, 29, "|    Snake Game Made by Abdias     |");
    mvprintw(5, 29, "------------------------------------");
    mvprintw(7, 29, "Use arrow keys to control snake");
    mvprintw(9, 43, "   ^ ");
    mvprintw(10, 43, "<  v  >");
    mvprintw(14, 29, "** Colliding with wall or tail will cause snake to die. ");

    mvprintw(16, 29, "$$ Score = %d", score);
//  mvprintw(17, 29, "$$ Remain Time = %d", current_time);
    refresh();
}

void Poll_Input()
{
    //checks if input exists, using from ncruses
    keypad(stdscr, TRUE); 
    halfdelay(1); 

    int c = getch(); 

    //definition of keys
    switch(c)
    {
        case KEY_LEFT:
            dir = LEFT;
            break;
        case KEY_RIGHT:
            dir = RIGHT;
            break;
        case KEY_DOWN: 
            dir = UP;
            break;
        case KEY_UP:
            dir = DOWN;
            break;
        case KEY_EXIT:
            gameOver = true;
            break;

    }    

}


void Update()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    // add new position and other one copy
    for (int i=1; i<numTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }


    switch (dir)
    {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y++;
            break;
        case DOWN:
            y--;
            break;
        default: 
            break;
    }

// if snake goes outside from the map, game will be over    
    if (x>WIDTH || x<1 || y<1 || y>HEIGHT)
    {
        gameOver=true;
    }

// if snake eats the fruit on the map, score will increase    
    if (x==fruitX && y==fruitY)
    {
        score ++;
        fruitX = (rand() % WIDTH)+1;
        fruitY = (rand() % HEIGHT)+1;
        numTail++;
    }

    for (int i=0; i<numTail; i++)
    {
        if(tailX[i]==x && tailY[i]==y)
        {
            gameOver=true;
        }
        
    }

}


int main(void)
{
    Initialize();

   while(!gameOver)
    {
        Render();
        Poll_Input();
        Update();

    }
    
    getch();
    sleep(1); 
    endwin();
    
    //prints out loss, and high score
    std::cout << "You lose! High Score: " << score;

    return 0;
}
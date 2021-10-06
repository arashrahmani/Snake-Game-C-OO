// snake game by Arash_Rahmani
// please see all this file before using that
// it's object-oriented implementation of this popular game

#include <stdlib.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <ctime>
#include <termio.h>
#include <random>
using namespace std;

struct point{
    int x;
    int y;
    point(int a=0, int b=0){
        x=a;
        y=b;
    }
};

int bytesWaiting;
int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if ( !initialized ) {
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized=true;
    }
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

class snake{
public:
    snake(){}
    snake(int center_y, int center_x){
        body.push_back(point(center_x, center_y));
    }
    void set_dir();
    void add();
    void update(bool);
    vector<point> get_body(){
        return body;
    }
private:
    char dir;
    vector<point> body;
    vector<point> previous_pos;
    char previous_dir;
};
void snake::add(){
    body.push_back(previous_pos[previous_pos.size()-1]);
}
void snake::update(bool hit){
    previous_pos=body;
    if (hit){
        previous_dir=dir;
        cin>>dir;
    if(body.size()>1)
        set_dir();
    }
    switch(dir){
        case('w'):
            --body[0].x;
            for(int i=1; i<body.size(); i++){
                body[i]=previous_pos[i-1];
            }
            break;
        case('s'):
            ++body[0].x;
            for(int i=1; i<body.size(); i++){
                body[i]=previous_pos[i-1];
            }
            break;
        case('a'):
            --body[0].y;
            for(int i=1; i<body.size(); i++){
                body[i]=previous_pos[i-1];
            }
            break;
        case('d'):
            ++body[0].y;
            for(int i=1; i<body.size(); i++){
                body[i]=previous_pos[i-1];
            }
            break;
    }
}
void snake::set_dir(){
    switch(dir){
        case('w'):
            if(previous_dir=='s')
                dir='s';
            break;
        case('s'):
            if(previous_dir=='w')
                dir='w';
            break;
        case('a'):
            if(previous_dir=='d')
                dir='d';
            break;
        case('d'):
            if(previous_dir=='a')
                dir='a';
            break;
    }
}
class blocks{
public:
    blocks(){}
    blocks(int, int);
    vector<point> get_walls(){ return walls; }
private:
    vector<point> walls;
};
blocks::blocks(int w, int h){
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            if(i==0)
                walls.push_back(point (i, j));
            else if(j==0)
                walls.push_back(point (i, j));
            else if(i==h-1)
                walls.push_back(point (i, j));
            else if(j==w-1)
                walls.push_back(point (i, j));
        }
    }
}
    
class game{
public:
    game(int, int);
    void food_generator();
    void update(uint);
    void chk_losing();
    void start();
private:
    int width;
    int height;
    int score=0;
    bool losing=false;
    blocks b1;
    snake  s1;
    vector<point> walls;
    vector<point> body ;
    point food;
};
game::game(int w, int h){
        width=w;
        height=h;
        b1=blocks(w, h);
        food_generator();
        s1=snake(w/2, h/2);
        body =s1.get_body();
        walls=b1.get_walls();
     }
    void game:: start(){
        string all_cells[height][width+1];
        while(!losing){
            if(_kbhit()){
                s1.update(true);
            }
            else{
                s1.update(false);
            }
            for(int i=0; i<height; i++){
                for(int j=0; j<width+1; j++){
                    all_cells[i][j]=' ';
                    for(int z=0; z<walls.size(); z++){
                        if(i==walls[z].x && j==walls[z].y)
                            all_cells[i][j]='#';
                    }
                    if(i==food.x && j==food.y)
                        all_cells[i][j]='O';
                    for(int k=0; k<body.size(); k++){
                        if(i==body[k].x && j==body[k].y)
                            all_cells[i][j]='0';
                    }
                    if(j==width){
                        all_cells[i][j]="\n";
                    }
                    if(body[0].x==food.x && body[0].y==food.y){
                        s1.add();
                        food_generator();
                        score++;
                    }
                    cout<<all_cells[i][j];
                }
            }
            cout<<"score:\t"<<score<<endl;
            chk_losing();
        }
    }
void game::update(uint time_nanosecs){
    this_thread::sleep_for(chrono::nanoseconds(time_nanosecs));
    system("clear");
}
void game::food_generator(){
    srand(time(0));
    std::mt19937 mersenne(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<> x_range(1, height-2);
    std::uniform_int_distribution<> y_range(1, width -2);
    int rand_x=x_range(mersenne);
    int rand_y=y_range(mersenne);
    food.x=rand_x;
    food.y=rand_y;
}
void game::chk_losing(){
    for(int i=0; i<walls.size(); i++){
        if(body[0].x==walls[i].x && body[0].y==walls[i].y){
            losing=true;
            cout<<"\t GAME OVER :) \n";
        }
    }
    if(!losing){
        body=s1.get_body();
        update(100000000);
    }
}

int main(){
    game g1(40, 21);
    g1.start();
}


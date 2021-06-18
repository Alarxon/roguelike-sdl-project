#include "level.hpp"

//#include <stdio.h>
//#include <stdlib.h>

//#include <vector>
//#include <utility>
//#include <string>
#include <iostream>
#include <algorithm>
//#include <random>
//#include <ctime>

using namespace std;

int width = 0;
int height = 0;
int max_rooms = 0;
int min_room_xy = 0;
int max_room_xy = 0;
bool rooms_overlap = false;
int random_connections = 0;
int random_spurs = 0;

mt19937 gen;

vector<vector<char>> level;
vector<vector<int>> room_list;
vector<vector<pair<int,int>> > corridor_list;

//Function generate room implementation
vector<int> gen_room(int x, int y, int direction){
    //room variables
    int final_x = 0;
    int final_y = 0;
    int father_corridor = 0;
    int w = 0;
    int h = 0;

    //rand int between min_room_xy and max_room_xy for the width and height room
    uniform_int_distribution<> wh_rand(min_room_xy, max_room_xy); //inclusive
    w = wh_rand(gen);
    h = wh_rand(gen);

    //North
    if(direction == 0){
        //rand int between 1 and the room y minus the new height - 1
        //cout << "North: " << 1 << " " << (y - h - 1) << "\n";
        uniform_int_distribution<> y_rand(1, (y - h - 1)); //inclusive
        final_y = y_rand(gen);
        final_x = x;
        //South
        father_corridor = 2;
    }
    //West
    else if(direction == 1){
        //rand int between the room x plus the new width + 1 and the level width - 1
        //cout << "West: " << (x + w + 1) << " "  << (width - 1) << "\n";
        uniform_int_distribution<> x_rand( (x + w + 1), (width - 1) ); //inclusive
        final_x = x_rand(gen);
        final_y = y;
        //East
        father_corridor = 3;
    }
    //South
    else if(direction == 2){
        //rand int between the room y plus the new height + 1 and height - 1
        //cout << "South: " << (y + h + 1) << " " <<  (height - 1) << "\n";
        uniform_int_distribution<> y_rand( (y + h + 1) , (height - 1) ); //inclusive
        final_y = y_rand(gen);
        final_x = x;
        //North
        father_corridor = 0;
    }
    //East
    else if(direction == 3){
        //rand int between the room x plus the new width + 1 and the level width - 1
        //cout << "East: " << 1 << " " << (x - w - 1) << "\n";
        uniform_int_distribution<> x_rand( 1, (x - w - 1) ); //inclusive
        final_x = x_rand(gen);
        final_y = y;
        //West
        father_corridor = 1;
    }

    //Temporal room vector
    //cout << "***" << final_x << " " << final_y << " " << w << " " << h << " " << father_corridor << "\n";
    vector<int> temp_vector{final_x, final_y, w, h, father_corridor};

    return temp_vector;
}

//Function if a point is between other two points
bool point_between_points(pair<int,int> a, pair<int,int> b, pair<int,int> c){
    int crossproduct = (c.second - a.second) * (b.first - a.first) - (c.first - a.first) * (b.second - a.second);
    if(crossproduct != 0)
        return false;
    int dotproduct = (c.first - a.first) * (b.first - a.first) + (c.second - a.second)*(b.second - a.second);
    if(dotproduct < 0)
        return false;
    int squaredlengthba = (b.first - a.first)*(b.first - a.first) + (b.second - a.second)*(b.second - a.second);
    if(dotproduct > squaredlengthba)
        return false;
    return true;
}

//Function check room overlaping or out of bounds implementation
bool room_overlapping(vector<int> room){
    int x = room[0];
    int y = room[1];
    int w = room[2];
    int h = room[3];

    //Out of bounds
    if((x + w) >= width || x <= 0){
        return true;
    }
    if((y + h) >= height || y <= 0){
        return true;
    }

    //room overlap
    for (int i = 0; i < (int)room_list.size(); i++){
        //If the room overlaps (is inside) with another room in the list
        //+2 and -3 to have a mayor range between the rooms
        if (( (x) < (room_list[i][0] + room_list[i][2] + 3)) && ( (room_list[i][0]-3) < (x + w)) &&
            ( (y) < (room_list[i][1] + room_list[i][3] + 3)) && ( (room_list[i][1]-3) < (y + h))){
            return true;
        }
    }
    /*
    //corridor overlap
    for(int i = 0; i < (int)corridor_list.size(); i++){
        int x1 = corridor_list[i][0].first;
        int y1 = corridor_list[i][0].second;
        int x2 = corridor_list[i][1].first;
        int y2 = corridor_list[i][1].second;

        int w_corridor = 0;
        int h_corridor = 0;
        int x_corridor = 0;
        int y_corridor = 0;

        //vertical corridor
        if(x1 == x2){
            w_corridor = 1;
            x_corridor = x1 + 2;
            h_corridor = max(y1, y2) - min(y1,y2);
            y_corridor = min(y1,y2);
        }
        //horizontal corridor
        else if(y1 == y2){
            h_corridor = 1;
            y_corridor = y1 + 2;
            w_corridor = max(x1, x2) - min(x1,x2);
            x_corridor = min(x1, x2);
        }

        if( ((x) < (x_corridor + w_corridor)) && ( (x_corridor) < (x + w)) &&
             ((y)  < (y_corridor + h_corridor)) && ( (y_corridor) < (y + h) ) ){
                 return true;
        }

    }
    */


    return false;
}

//Function join the rooms with the corridors implementation
void join_rooms(vector<int> room_1, vector<int> room_2){
    //Room 1 information
    int x1 = room_1[0];
    int y1 = room_1[1];
    int w1 = room_1[2];
    int h1 = room_1[3];
    

    //Room 2 information
    int x2 = room_2[0];
    int y2 = room_2[1];
    int w2 = room_2[2];
    int h2 = room_2[3];
    int parent_corridor = room_2[4];

    //North
    if(parent_corridor == 0){
        int common_x;
        if((x1 + w1) < (x2 + w2)){
            common_x = x1 + (w1 / 2);
        }else{
            common_x = x2 + (w2 / 2);
        }
        vector<pair<int, int>> temp_vector{make_pair(common_x, y1 + h1), make_pair(common_x, y2 - 1)};
        //vector<pair<int, int>> temp_vector{make_pair(common_x, y1), make_pair(common_x, y2 + h2 - 1)};
        corridor_list.push_back(temp_vector);
    }

    //West
    else if(parent_corridor == 1){
        int common_y;
        if((y1 + h1) < (y2 + h2)){
            common_y = y1 + (h1 / 2);    
        }else{
            common_y = y2 + (h2 / 2);
        }
        vector<pair<int, int>> temp_vector{make_pair(x1 - 1, common_y), make_pair(x2 + w2, common_y)};
        //vector<pair<int, int>> temp_vector{make_pair(x1 + w1 - 1, common_y), make_pair(x2, common_y)};
        corridor_list.push_back(temp_vector);
    }

    //South
    else if(parent_corridor == 2){
        int common_x;
        if((x1 + w1) < (x2 + w2)){
            common_x = x1 + (w1 / 2);
        }else{
            common_x = x2 + (w2 / 2);
        }

        vector<pair<int, int>> temp_vector{make_pair(common_x, y1 - 1), make_pair(common_x, y2 + h2)};
        //vector<pair<int, int>> temp_vector{make_pair(common_x, y1 + h1 - 1), make_pair(common_x, y2)};
        corridor_list.push_back(temp_vector);
    }

    //East
    else if(parent_corridor == 3){
        int common_y;
        if((y1 + h1) < (y2 + h2)){
            common_y = y1 + (h1 / 2);    
        }else{
            common_y = y2 + (h2 / 2);
        }
        vector<pair<int, int>> temp_vector{make_pair(x1 + w1, common_y), make_pair(x2 - 1, common_y)};
        //vector<pair<int, int>> temp_vector{make_pair(x1, common_y), make_pair(x2 + w2 - 1, common_y)};
        corridor_list.push_back(temp_vector);
    }
   
}

//Function level generation implementation
vector<vector<char>> gen_level(int w, int h, int max_r, int min_r_xy, int max_r_xy){
    //Set seed - I do it in my main loop
    //gen.seed(time(nullptr)); // seed the generator

    //Assignation global variables
    if (w >= MIN_LEVEL_WIDTH && w <= MAX_LEVEL_WIDTH){
        width = w;
    }else{
        width = MIN_LEVEL_WIDTH;
    }

    if (h >= MIN_LEVEL_HEIGHT && h <= MAX_LEVEL_HEIGHT){
        height = h;
    }else{
        height = MIN_LEVEL_HEIGHT;
    }

    if (max_r >= MIN_ROOMS_LIMIT && max_r <= MAX_ROOMS_LIMIT){
        max_rooms = max_r;
    }else{
        max_rooms = MIN_ROOMS_LIMIT;
    }

    if (min_r_xy >= MIN_ROOM_XY_LIMIT && min_r_xy <= MAX_ROOM_XY_LIMIT){
        min_room_xy = min_r_xy;
    }else{
        min_room_xy = MIN_ROOM_XY_LIMIT;
    }

    if (max_r_xy >= MIN_ROOM_XY_LIMIT && max_r_xy <= MAX_ROOM_XY_LIMIT){
        max_room_xy = max_r_xy;
    }else{
        max_room_xy = MAX_ROOM_XY_LIMIT;
    }

    level.clear();
    room_list.clear();
    corridor_list.clear();

    //Empty dungeon
    for (int i = 0; i < height; i++){
        vector<char> temp_init(width, STONE);
        level.push_back(temp_init);
    }

    //Max iterations to generate rooms
    int max_iters = max_rooms * 5;

    //Initial room x,y,w,h,father_corridor
    vector<int> initial_vector{width/2, 5, 5, 5, 0};
    room_list.push_back(initial_vector);

    
    //Generate rooms
    int room_number = 0;
    for (int i = 0; i < max_iters; i++){
        room_number = room_list.size() - 1;
        for(int j = 0; j < 4; j++){
            if(j != room_list[room_number][4]){
                vector<int> tmp_room = gen_room(room_list[room_number][0],room_list[room_number][1],j);
                //If room_overlapping is false insert room
                if (room_overlapping(tmp_room) == false){
                    room_list.push_back(tmp_room);
                    join_rooms(room_list[room_number], tmp_room);
                }
                tmp_room.clear();
            }
            if ((int)room_list.size() >= max_rooms)
                    break;
        }

        if ((int)room_list.size() >= max_rooms){
            break;
        }
    }
    
    
    //fill the map
    //paint rooms
    for (int i = 0; i < (int)room_list.size(); i++){
        for (int j = 0; j < room_list[i][2]; j++){
            for (int k = 0; k < room_list[i][3]; k++){
                //cout << room_list[i][1] + k << " " << room_list[i][0] + j << "\n";
                level[room_list[i][1] + k][room_list[i][0] + j] = FLOOR;
            }
        }
    }

    //print_with_ascii(level);
    
    
    //paint corridors
    for (int i = 0; i < (int)corridor_list.size(); i++){
        pair<int, int> corridor_1 = corridor_list[i][0];
        pair<int, int> corridor_2 = corridor_list[i][1];
        int width_range = abs(corridor_1.first - corridor_2.first) + 1;
        for (int j = 0; j < width_range; j++){
            int height_range = abs(corridor_1.second - corridor_2.second) + 1;
            for (int k = 0; k < height_range; k++){
                level[min(corridor_1.second, corridor_2.second) + k]
                    [min(corridor_1.first, corridor_2.first) + j] =  FLOOR;
            }
        }
    }

    //paint the walls
    for (int i = 1; i < (height - 1); i++){
        for (int j = 1; j < (width - 1); j++){
            if (level[i][j] == FLOOR){
                if (level[i - 1][j - 1] == STONE)
                    level[i - 1][j - 1] = WALL;
                if (level[i - 1][j] == STONE)
                    level[i - 1][j] = WALL;
                if (level[i - 1][j + 1] == STONE)
                    level[i - 1][j + 1] = WALL;
                if (level[i][j - 1] == STONE)
                    level[i][j - 1] = WALL;
                if (level[i][j + 1] == STONE)
                    level[i][j + 1] = WALL;
                if (level[i + 1][j - 1] == STONE)
                    level[i + 1][j - 1] = WALL;
                if (level[i + 1][j] == STONE)
                    level[i + 1][j] = WALL;
                if (level[i + 1][j + 1] == STONE)
                    level[i + 1][j + 1] = WALL;
            }
        }
    }

    
    for(int i = 0; i < (int)corridor_list.size(); i++){
        for(int j = 0; j < (int)corridor_list[i].size(); j++){
            int x = corridor_list[i][j].first;
            int y = corridor_list[i][j].second;
            level[y][x] = DOOR;
        }
    }

    print_with_ascii(level);
    
    /*
    for (int i = 0; i < (int)room_list.size(); i++){
        for (int j = 0; j < room_list[i][2] + 2; j++){
            for (int k = 0; k < room_list[i][3] + 2; k++){
                if (j == 0 || k == 0 || j == (room_list[i][2] + 1) || k == (room_list[i][3] + 1)){
                     if(level[room_list[i][1] + k - 1][room_list[i][0] + j - 1] == FLOOR){
                        if( (level[room_list[i][1] + k -2][room_list[i][0] + j -1] == FLOOR 
                            && level[room_list[i][1] + k][room_list[i][0] + j - 1] == FLOOR) ||
                            (level[room_list[i][1] + k - 1][room_list[i][0] + j - 2] == FLOOR
                            && level[room_list[i][1] + k - 1][room_list[i][0] + j] == FLOOR) ){
                                level[room_list[i][1] + k - 1][room_list[i][0] + j - 1] = DOOR;
                        }
                     }
                }
            }
        }
    }
    */

   /*
   for (int i = 0; i < (int)room_list.size(); i++){
        for (int j = 0; j < room_list[i][2] + 2; j++){
            for (int k = 0; k < room_list[i][3] + 2; k++){
                if (j == 0 || k == 0 || j == (room_list[i][2] + 1) || k == (room_list[i][3] + 1)){
                     if(level[room_list[i][1] + k - 1][room_list[i][0] + j - 1] == FLOOR){
                        if( (level[room_list[i][1] + k -2][room_list[i][0] + j -1] ==  
                             level[room_list[i][1] + k][room_list[i][0] + j - 1]) &&
                            (level[room_list[i][1] + k - 1][room_list[i][0] + j - 2] ==
                             level[room_list[i][1] + k - 1][room_list[i][0] + j]) ){
                                level[room_list[i][1] + k - 1][room_list[i][0] + j - 1] = DOOR;
                        }
                     }
                }
            }
        }
    }
    */

    for (int i = 0; i < (int)room_list.size(); i++){
        for (int j = 0; j < room_list[i][2] + 2; j++){
            for (int k = 0; k < room_list[i][3] + 2; k++){
                if (j == 0 || k == 0 || j == (room_list[i][2] + 1) || k == (room_list[i][3] + 1)){
                     if(level[room_list[i][1] + k - 1][room_list[i][0] + j - 1] == FLOOR){
                        if( !(level[room_list[i][1] + k -2][room_list[i][0] + j -1] == WALL 
                            && level[room_list[i][1] + k][room_list[i][0] + j - 1] == FLOOR) &&
                            !(level[room_list[i][1] + k - 1][room_list[i][0] + j - 2] == WALL
                            && level[room_list[i][1] + k - 1][room_list[i][0] + j] == FLOOR) 
                            
                            && !(level[room_list[i][1] + k -2][room_list[i][0] + j -1] == FLOOR 
                            && level[room_list[i][1] + k][room_list[i][0] + j - 1] == WALL) &&
                             !(level[room_list[i][1] + k - 1][room_list[i][0] + j - 2] == FLOOR
                            && level[room_list[i][1] + k - 1][room_list[i][0] + j] == WALL)){
                                level[room_list[i][1] + k - 1][room_list[i][0] + j - 1] = DOOR;
                        }
                     }
                }
            }
        }
    }

    /*    
    //paint doors
    for (int i = 0; i < (int)room_list.size(); i++){
        for (int j = 0; j < room_list[i][2] + 2; j++){
            for (int k = 0; k < room_list[i][3] + 2; k++){
                if (j == 0 || k == 0 || j == (room_list[i][2] + 1) || k == (room_list[i][3] + 1)){
                     if(level[room_list[i][1] + k - 1][room_list[i][0] + j - 1] == FLOOR){
                        level[room_list[i][1] + k - 1][room_list[i][0] + j - 1] = DOOR;
                     }
                }
            }
        }
    }
    */
    

    print_room_list();
    print_corridors();
    return level;
}

//Function get level implementation
vector<vector<char>> get_level(void){
    return level;
}
//Function get rooms implementation
vector<vector<int>> get_rooms_list(void){
    return room_list;
}
//Function get corridors implementation
vector<vector<pair<int, int>>> get_corridor_list(void){
    return corridor_list;
}
//Printf to console for debug implementation
void print_to_console(vector<vector<char>> level){
    printf("\n");
    for (int i = 0; i < (int)level.size(); i++){
        for (int j = 0; j < (int)level[i].size(); j++){
            printf("%c", level[i][j]);
        }
        printf("\n");
    }
}

//Print to console with ASCII
void print_with_ascii(vector<vector<char>> level){
    printf("\n");
    for(int i = 0; i < (int)level.size(); i++){
        if((i%9)+1 == 1){
            printf(" ");
        }else{
            printf("%d", (i%9)+1);
        }
    }

    printf("\n");
    for (int i = 0; i < (int)level.size(); i++){
        for (int j = 0; j < (int)level[i].size(); j++){
            if(level[i][j] == 'B'){
                int stone = 219;
                printf("%c", stone);
            }else if(level[i][j] == 'P'){
                int wall = 178;
                printf("%c", wall);
            }else if(level[i][j] == 'D'){
                int door = 221;
                printf("%c", door);
            }else{
                printf("%c", level[i][j]);
            }
        }
        printf("%d", i+1 );
        printf("\n");
    }
}

void print_room_list(){
    printf("\n");
    for(int i = 0; i < (int)room_list.size(); i++){
        printf("[");
        for(int j = 0; j < (int)room_list[i].size(); j++){
            printf(" %d ",room_list[i][j]);
        }
        printf("]");
    }
    printf("\n");
}

void print_corridors(){
    printf("\n");
    for(int i = 0; i < (int)corridor_list.size(); i++){
        printf("[");
        for(int j = 0; j < (int)corridor_list[i].size(); j++){
            printf(" %d,%d ",corridor_list[i][j].first, corridor_list[i][j].second);
        }
        printf("]");
    }
    printf("\n");
}

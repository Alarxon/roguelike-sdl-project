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
vector<int> gen_room(void){
    //room variables
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    //rand int between min_room_xy and max_room_xy for the width and height room
    uniform_int_distribution<> wh_rand(min_room_xy, max_room_xy); //inclusive
    w = wh_rand(gen);
    h = wh_rand(gen);
    //rand int between 1 and the level width minus the room width - 1
    uniform_int_distribution<> x_rand(1, (width - w - 1)); //inclusive
    x = x_rand(gen);
    //rand int between 1 and the level height minus the room height - 1
    uniform_int_distribution<> y_rand(1, (height - h - 1)); //inclusive
    y = y_rand(gen);


    //Temporal room vector
    vector<int> temp_vector{x, y, w, h};
    return temp_vector;
}

//Function check room overlaping implementation
bool room_overlapping(vector<int> room){
    int x = room[0];
    int y = room[1];
    int w = room[2];
    int h = room[3];
    for (int i = 0; i < (int)room_list.size(); i++){
        //If the room overlaps (is inside) with another room in the list
        if ((x < (room_list[i][0] + room_list[i][2])) && (room_list[i][0] < (x + w)) &&
            (y < (room_list[i][1] + room_list[i][3])) && (room_list[i][1] < (y + h))){
            return true;
        }
    }
    return false;
}

//Function generate corridors between points implementation
vector<pair<int, int>> corridor_between_points(int x1, int y1, int x2, int y2, string join_type){
    //if the points are the same
    if ((x1 == x2 && y1 == y2) || (x1 == x2 || y1 == y2)){
        vector<pair<int, int>> temp_vector{make_pair(x1, y1), make_pair(x2, y2)};
        return temp_vector;
    }else{
        string join = "";

        //Intersection for bottom join
        vector<int> intersection_botton(6);
        int first_bottom[] = {0, 1};
        int second_bottom[] = {x1, x2, y1, y2};
        sort(second_bottom, second_bottom + 4);
        set_intersection(first_bottom, first_bottom + 2, second_bottom, second_bottom + 4,
                         intersection_botton.begin());

        //Intersection for top join (first)
        vector<int> intersection_top_1(4);
        int first_top_1[] = {width - 2, width - 1};
        int second_top_1[] = {x1, x2};
        sort(second_top_1, second_top_1 + 2);
        set_intersection(first_top_1, first_top_1 + 2, second_top_1, second_top_1 + 2,
                         intersection_top_1.begin());
        //Intersection for top join (second)
        vector<int> intersection_top_2(4);
        int first_top_2[] = {height - 2, height - 1};
        int second_top_2[] = {y1, y2};
        sort(second_top_2, second_top_2 + 2);
        set_intersection(first_top_2, first_top_2 + 2, second_top_2, second_top_2 + 2,
                         intersection_top_2.begin());

        if (join_type == "either" && intersection_botton.size() > 0){
            join = "bottom";
        }
        else if (join_type == "either" && ((intersection_top_1.size() > 0) || (intersection_top_2.size() > 0))){
            join = "top";
        }
        else if (join_type == "either"){
            //Random choice between top and bottom
            int random_choice = rand() % 2;
            if (random_choice == 0){
                join = "top";
            }else{
                join = "bottom";
            }
        }else{
            join = join_type;
        }

        if (join == "top"){
            vector<pair<int, int>> temp_vector{make_pair(x1, y1), make_pair(x1, y2), make_pair(x2, y2)};
            return temp_vector;
        }else if (join == "bottom"){
            vector<pair<int, int>> temp_vector{make_pair(x1, y1), make_pair(x2, y1), make_pair(x2, y2)};
            return temp_vector;
        }
    }

}

//Function join the rooms with the corridors implementation
void join_rooms(vector<int> room_1, vector<int> room_2, string join_type){
    //Sorted rooms by x
    vector<vector<int>> sorted_room{room_1, room_2};
    sort(sorted_room.begin(), sorted_room.end(), [](const vector<int> &a, const vector<int> &b) {
        return a[0] < b[0];
    });

    //Room 1 information
    int x1 = sorted_room[0][0];
    int y1 = sorted_room[0][1];
    int w1 = sorted_room[0][2];
    int h1 = sorted_room[0][3];
    int x1_2 = x1 + w1 - 1;
    int y1_2 = y1 + h1 - 1;

    //Room 2 information
    int x2 = sorted_room[1][0];
    int y2 = sorted_room[1][1];
    int w2 = sorted_room[1][2];
    int h2 = sorted_room[1][3];
    int x2_2 = x2 + w2 - 1;
    int y2_2 = y2 + h2 - 1;

    //overlapping on x
    if (x1 < (x2 + w2) && x2 < (x1 + w1)){
        uniform_int_distribution<> jx1_rand(x2, x1_2); //inclusive  
        int jx1 = jx1_rand(gen);
        int jx2 = jx1;
        vector<int> tmp_y{y1, y2, y1_2, y2_2};
        sort(tmp_y.begin(), tmp_y.end());
        int jy1 = tmp_y[1] + 1;
        int jy2 = tmp_y[2] - 1;

        vector<pair<int, int>> corridors = corridor_between_points(jx1, jy1, jx2, jy2);
        corridor_list.push_back(corridors);
    }

    //overlapping on y
    else if (y1 < (y2 + h2) && y2 < (y1 + h1)){
        int jy1, jy2;
        if (y2 > y1){
            uniform_int_distribution<> jy1_rand(y2, y1_2); //inclusive  
            jy1 = jy1_rand(gen);
            jy2 = jy1;
        }else{
            uniform_int_distribution<> jy1_rand(y1, y2_2); //inclusive  
            jy1 = jy1_rand(gen);
            jy2 = jy1;
        }
        vector<int> tmp_x{x1, x2, x1_2, x2_2};
        sort(tmp_x.begin(), tmp_x.end());
        int jx1 = tmp_x[1] + 1;
        int jx2 = tmp_x[2] - 1;

        vector<pair<int, int>> corridors = corridor_between_points(jx1, jy1, jx2, jy2);
        corridor_list.push_back(corridors);
    }
    //no overlap
    else{
        string join = "";
        if (join_type == "either"){
            int random_choice = rand() % 2;
            if (random_choice == 0){
                join = "top";
            }else{
                join = "bottom";
            }
        }else{
            join = join_type;
        }

        if (join == "top"){
            if (y2 > y1){
                int jx1 = x1_2 + 1;
                uniform_int_distribution<> jy1_rand(y1, y1_2); //inclusive  
                int jy1 = jy1_rand(gen);
                uniform_int_distribution<> jx2_rand(x2, x2_2); //inclusive  
                int jx2 = jx2_rand(gen);
                int jy2 = y2 - 1;

                vector<pair<int, int>> corridors = corridor_between_points(jx1, jy1, jx2, jy2, "bottom");
                corridor_list.push_back(corridors);
            }else{
                uniform_int_distribution<> jx1_rand(x1, x1_2); //inclusive  
                int jx1 = jx1_rand(gen);
                int jy1 = y1 - 1;
                int jx2 = x2 - 1;
                uniform_int_distribution<> jy2_rand(y2, y2_2); //inclusive   
                int jy2 = jy2_rand(gen);

                vector<pair<int, int>> corridors = corridor_between_points(jx1, jy1, jx2, jy2, "top");
                corridor_list.push_back(corridors);
            }
        }else if (join == "bottom"){
            if (y2 > y1){
                uniform_int_distribution<> jx1_rand(x1, x1_2); //inclusive  
                int jx1 = jx1_rand(gen);
                int jy1 = y1_2 + 1;
                int jx2 = x2 - 1;
                uniform_int_distribution<> jy2_rand(y2, y2_2); //inclusive   
                int jy2 = jy2_rand(gen);

                vector<pair<int, int>> corridors = corridor_between_points(jx1, jy1, jx2, jy2, "top");
                corridor_list.push_back(corridors);
            }else{
                int jx1 = x1_2 + 1;
                uniform_int_distribution<> jy1_rand(y1, y1_2); //inclusive  
                int jy1 = jy1_rand(gen);
                uniform_int_distribution<> jx2_rand(x2, x2_2); //inclusive  
                int jx2 = jx2_rand(gen);
                int jy2 = y2_2 + 1;

                vector<pair<int, int>> corridors = corridor_between_points(jx1, jy1, jx2, jy2, "bottom");
                corridor_list.push_back(corridors);
            }
        }
    }
}

//Function level generation implementation
vector<vector<char>> gen_level(int w, int h, int max_r, int min_r_xy, int max_r_xy, bool r_overlap, int random_con, int random_spur){
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

    rooms_overlap = r_overlap;
    random_connections = random_con;
    random_spurs = random_spur;

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

    //Generate rooms
    for (int i = 0; i < max_iters; i++){
        vector<int> tmp_room = gen_room();
        //If overlap is true or is the room list is empty
        if (rooms_overlap || room_list.empty()){
            room_list.push_back(tmp_room);
        }else{
            //If overlap is false
            tmp_room.clear();
            vector<int> tmp_room = gen_room();
            //If room_overlapping  is false insert room
            if (room_overlapping(tmp_room) == false){
                room_list.push_back(tmp_room);
            }
        }

        tmp_room.clear();

        if ((int)room_list.size() >= max_rooms){
            break;
        }
    }

    //connect the rooms
    for (int i = 0; i < (int)(room_list.size() - 1); i++){
        join_rooms(room_list[i], room_list[i + 1]);
    }

    //do the random joins
    for (int i = 0; i < random_connections; i++){
        uniform_int_distribution<> random_rand(0, (room_list.size() - 1)); //inclusive  
        int random1 = random_rand(gen);
        int random2 = random_rand(gen);
        vector<int> room_1 = room_list[random1];
        vector<int> room_2 = room_list[random2];
        join_rooms(room_1, room_2);
    }

    //do the spurs
    for (int i = 0; i < random_spurs; i++){
        uniform_int_distribution<> w_rand(2, (width - 2)); //inclusive  
        int random1 = w_rand(gen);
        uniform_int_distribution<> h_rand(2, (height - 2)); //inclusive  
        int random2 = h_rand(gen);
        vector<int> room_1{random1, random2, 1, 1};

        uniform_int_distribution<> room_rand(0, (room_list.size() - 1)); //inclusive  
        int random3 = room_rand(gen);
        vector<int> room_2 = room_list[random3];
        join_rooms(room_1, room_2);
    }

    //fill the map
    //paint rooms
    for (int i = 0; i < (int)room_list.size(); i++){
        for (int j = 0; j < room_list[i][2]; j++){
            for (int k = 0; k < room_list[i][3]; k++){
                level[room_list[i][1] + k][room_list[i][0] + j] = FLOOR;
            }
        }
    }

    //paint corridors
    for (int i = 0; i < (int)corridor_list.size(); i++){
        pair<int, int> corridor_1 = corridor_list[i][0];
        pair<int, int> corridor_2 = corridor_list[i][1];
        int width_range = abs(corridor_1.first - corridor_2.first) + 1;
        for (int j = 0; j < width_range; j++){
            int height_range = abs(corridor_1.second - corridor_2.second) + 1;
            for (int k = 0; k < height_range; k++){
                level[min(corridor_1.second, corridor_2.second) + k]
                    [min(corridor_1.first, corridor_2.first) + j] = FLOOR;
            }
        }

        if (corridor_list[i].size() == 3){
            pair<int, int> corridor_3 = corridor_list[i][2];
            width_range = abs(corridor_2.first - corridor_3.first) + 1;
            for (int j = 0; j < width_range; j++){
                int height_range = abs(corridor_2.second - corridor_3.second) + 1;
                for (int k = 0; k < height_range; k++){
                    level[min(corridor_2.second, corridor_3.second) + k]
                         [min(corridor_2.first, corridor_3.first) + j] = FLOOR;
                }
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
    for (int i = 0; i < (int)level.size(); i++){
        for (int j = 0; j < (int)level[i].size(); j++){
            if(level[i][j] == 'B'){
                int stone = 219;
                printf("%c", stone);
            }else if(level[i][j] == 'P'){
                int wall = 178;
                printf("%c", wall);
            }else{
                printf("%c", level[i][j]);
            }
        }
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

#include "generator.hpp"

#include <iostream>
#include <algorithm>

using namespace std;


Generator::Generator(){
    width = 0;
    height = 0;
    max_rooms = 0;
    min_room_xy = 0;
    max_room_xy = 0;
}

void Generator::set_seed(long long int seed){
    gen.seed(seed);
}

vector<int> Generator::gen_room(int x, int y, int direction){
    //room variables
    int final_x = 0;
    int final_y = 0;
    int father_corridor = NORTH;
    int w = 0;
    int h = 0;

    //rand int between min_room_xy and max_room_xy for the width and height room
    uniform_int_distribution<> wh_rand(min_room_xy, max_room_xy); //inclusive
    w = wh_rand(gen);
    h = wh_rand(gen);

    //North
    if(direction == NORTH){
        //rand int between 1 and the room y minus the new height - 1
        uniform_int_distribution<> y_rand(1, (y - h - 1)); //inclusive
        final_y = y_rand(gen);
        final_x = x;
        //South
        father_corridor = SOUTH;
    }
    //West
    else if(direction == WEST){
        //rand int between the room x plus the new width + 1 and the level width - 1
        uniform_int_distribution<> x_rand( (x + w + 1), (width - 1) ); //inclusive
        final_x = x_rand(gen);
        final_y = y;
        //East
        father_corridor = EAST;
    }
    //South
    else if(direction == SOUTH){
        //rand int between the room y plus the new height + 1 and height - 1
        uniform_int_distribution<> y_rand( (y + h + 1) , (height - 1) ); //inclusive
        final_y = y_rand(gen);
        final_x = x;
        //North
        father_corridor = NORTH;
    }
    //East
    else if(direction == EAST){
        //rand int between the room x plus the new width + 1 and the level width - 1
        uniform_int_distribution<> x_rand( 1, (x - w - 1) ); //inclusive
        final_x = x_rand(gen);
        final_y = y;
        //West
        father_corridor = WEST;
    }

    //Temporal room vector
    int activate_flag = 0;
    vector<int> temp_vector{final_x, final_y, w, h, father_corridor,activate_flag};

    return temp_vector;
}

bool Generator::room_overlapping(vector<int> room){
    int x = room[0];
    int y = room[1];
    int w = room[2];
    int h = room[3];

    //Out of bounds
    if((x + w) >= (width - 1) || x <= 1){
        return true;
    }
    if((y + h) >= (height - 1) || y <= 1){
        return true;
    }

    //room overlap
    for (int i = 0; i < (int)room_list.size(); i++){
        //If the room overlaps (is inside) with another room in the list
        //+3 and -3 to have a mayor range between the rooms
        int room_offset = 3;
        if (( (x) < (room_list[i][0] + room_list[i][2] + room_offset)) && ( (room_list[i][0]-room_offset) < (x + w)) &&
            ( (y) < (room_list[i][1] + room_list[i][3] + room_offset)) && ( (room_list[i][1]-room_offset) < (y + h))){
            return true;
        }
    }

    return false;
}

void Generator::join_rooms(vector<int> room_1, vector<int> room_2){
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
    if(parent_corridor == NORTH){
        int common_x;
        if((x1 + w1) < (x2 + w2)){
            common_x = x1 + (w1 / 2);
        }else{
            common_x = x2 + (w2 / 2);
        }
        vector<pair<int, int>> temp_vector{make_pair(common_x, y1 + h1), make_pair(common_x, y2 - 1)};
        corridor_list.push_back(temp_vector);
    }

    //West
    else if(parent_corridor == WEST){
        int common_y;
        if((y1 + h1) < (y2 + h2)){
            common_y = y1 + (h1 / 2);    
        }else{
            common_y = y2 + (h2 / 2);
        }
        vector<pair<int, int>> temp_vector{make_pair(x1 - 1, common_y), make_pair(x2 + w2, common_y)};
        corridor_list.push_back(temp_vector);
    }

    //South
    else if(parent_corridor == SOUTH){
        int common_x;
        if((x1 + w1) < (x2 + w2)){
            common_x = x1 + (w1 / 2);
        }else{
            common_x = x2 + (w2 / 2);
        }

        vector<pair<int, int>> temp_vector{make_pair(common_x, y1 - 1), make_pair(common_x, y2 + h2)};
        corridor_list.push_back(temp_vector);
    }

    //East
    else if(parent_corridor == EAST){
        int common_y;
        if((y1 + h1) < (y2 + h2)){
            common_y = y1 + (h1 / 2);    
        }else{
            common_y = y2 + (h2 / 2);
        }
        vector<pair<int, int>> temp_vector{make_pair(x1 + w1, common_y), make_pair(x2 - 1, common_y)};
        corridor_list.push_back(temp_vector);
    }
}

vector<vector<char>> Generator::gen_level(int w, int h, int max_r, int min_r_xy, int max_r_xy){
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
    doors_list.clear();

    //Empty dungeon
    for (int i = 0; i < height; i++){
        vector<char> temp_init(width, STONE);
        level.push_back(temp_init);
    }

    //Max iterations to generate rooms
    int max_iters = max_rooms * 5;

    //Initial room x,y,w,h,father_corridor, state flag
    vector<int> initial_vector{width/2, 5, 5, 5, 0, 0};
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

    //Paint corridors doors (both sides)
    for(int i = 0; i < (int)corridor_list.size(); i++){
        for(int j = 0; j < (int)corridor_list[i].size(); j++){
            int x = corridor_list[i][j].first;
            int y = corridor_list[i][j].second;
            level[y][x] = DOOR;
        }
    }
    
    //Paint "extra" doors in all rooms (its trash but it works)
    for (int i = 0; i < (int)room_list.size(); i++){
        for (int j = 0; j < room_list[i][2] + 2; j++){
            for (int k = 0; k < room_list[i][3] + 2; k++){
                //The corners of the room
                if (j == 0 || k == 0 || j == (room_list[i][2] + 1) || k == (room_list[i][3] + 1)){
                    //If is a floor tile
                     if(level[room_list[i][1] + k - 1][room_list[i][0] + j - 1] == FLOOR){
                        /*
                        If any of the conditions are true then don't add a door
                        If the floor tile is between a wall AND a floor tile (in any directions)
                        then don't add the door
                            A - Actual floor tile
                            W - Wall tile
                            F - Another floor tile
                            
                            WAF --- NO DOOR
                            FAW --- NO DOOR

                            W
                            A --- NO DOOR
                            F

                            F
                            A --- NO DOoR
                            W
                        */
                        if( !(level[room_list[i][1] + k -2][room_list[i][0] + j -1] == WALL 
                            && level[room_list[i][1] + k][room_list[i][0] + j - 1] == FLOOR) &&
                            !(level[room_list[i][1] + k - 1][room_list[i][0] + j - 2] == WALL
                            && level[room_list[i][1] + k - 1][room_list[i][0] + j] == FLOOR) 
                            
                            && !(level[room_list[i][1] + k -2][room_list[i][0] + j -1] == FLOOR 
                            && level[room_list[i][1] + k][room_list[i][0] + j - 1] == WALL) &&
                             !(level[room_list[i][1] + k - 1][room_list[i][0] + j - 2] == FLOOR
                            && level[room_list[i][1] + k - 1][room_list[i][0] + j] == WALL)){
                                int x_door = room_list[i][1] + k - 1;
                                int y_door = room_list[i][0] + j - 1;
                                level[x_door][y_door] = DOOR;
                                doors_list.push_back(make_pair(make_pair(x_door, y_door), i));
                        }
                     }
                }
            }
        }
    }

    /*
    //Final corridors
    for(int i = 0; i < (int)corridor_list.size(); i++){
        int x1 = corridor_list[i][0].first;
        int y1 = corridor_list[i][0].second;
        int x2 = corridor_list[i][1].first;
        int y2 = corridor_list[i][1].second;
        if(y1 == y2){
            int x_initial = min(x1, x2);
            int x_final = max(x2,x2);
            int w_final = x_final - x_initial;
            vector<int> temp_vector{x_initial, y1, w_final, 1, 0};
            final_corridors_list.push_back(temp_vector);
        }else if(x1 == x2){
            int y_initial = min(y1,y2);
            int y_final = max(y1,y2);
            int h_final = y_final - y_initial;
            vector<int> temp_vector{x1, y_initial, 1, h_final, 0};
            final_corridors_list.push_back(temp_vector);
        }
    }
    */

    print_room_list();
    print_corridors();
    return level;
}

vector<vector<char>> Generator::get_level(void){
    return level;
}

vector<vector<int>> Generator::get_rooms_list(void){
    return room_list;
}

vector<vector<pair<int, int>>> Generator::get_corridor_list(void){
    return corridor_list;
}

vector<pair<pair<int,int> , int> > Generator::get_doors_list(void){
    return doors_list;
}

void Generator::print_to_console(vector<vector<char>> level){
    printf("\n");
    for (int i = 0; i < (int)level.size(); i++){
        for (int j = 0; j < (int)level[i].size(); j++){
            printf("%c", level[i][j]);
        }
        printf("\n");
    }
}

void Generator::print_with_ascii(vector<vector<char>> level){
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

void Generator::print_room_list(){
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

void Generator::print_corridors(){
    printf("\n");
    for(int i = 0; i < (int)corridor_list.size(); i++){
        printf("[");
        for(int j = 0; j < (int)corridor_list[i].size(); j++){
            printf(" %d,%d ",corridor_list[i][j].first, corridor_list[i][j].second);
        }
        printf("]");
    }
}


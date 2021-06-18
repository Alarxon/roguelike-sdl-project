#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include <vector>
#include <utility> 
#include <string>
#include <random>

using namespace std; 

//Max and min values
#define MAX_LEVEL_WIDTH 1000
#define MAX_LEVEL_HEIGHT 1000
#define MIN_LEVEL_WIDTH 32
#define MIN_LEVEL_HEIGHT 32
#define MAX_ROOMS_LIMIT 100
#define MIN_ROOMS_LIMIT 15
#define MIN_ROOM_XY_LIMIT 5
#define MAX_ROOM_XY_LIMIT 50
#define DEFAULT_RANDOM_CONNECTION 2
#define DEFAULT_RANDOM_SPURS 3

//Tiles
enum TILES{STONE = 'B', FLOOR = ' ', WALL = 'P', DOOR = 'D'};

//Cardinal points
enum CARDINAL{NORTH, WEST, SOUTH, EAST};

class Generator{
public:
    //Constructor
    Generator();
    //Function level generation
    vector<vector<char>> gen_level(int w = MIN_LEVEL_WIDTH, int h = MIN_LEVEL_HEIGHT, 
                    int max_r = MIN_ROOMS_LIMIT, int min_r_xy = MIN_ROOM_XY_LIMIT, 
                    int max_r_xy = MAX_ROOM_XY_LIMIT);
    //Set seed
    void set_seed(long long int seed);
    //Function get level
    vector<vector<char>> get_level(void);
    //Function get rooms
    vector<vector<int>> get_rooms_list(void);
    //Function get corridors
    vector<vector<pair<int,int>> > get_corridor_list(void);
    //Function get doors
    vector<pair<pair<int,int> , int> > get_doors_list(void);
    //Printf to console for debug
    void print_to_console(vector<vector<char>> level);
    //Print to console with ASCII
    void print_with_ascii(vector<vector<char>> level);
    void print_room_list();
    void print_corridors();
private:
    //Vector level (generation), vector room_list (x,y,w,h,father_corridor,state_flag) 
    //and vector corridor_list (x,y)
    vector<vector<char>> level;
    //Room list
    vector<vector<int>> room_list;
    //Corridors list to paint
    vector<vector<pair<int,int>> > corridor_list;
    //x,y position's door and room number
    vector<pair<pair<int,int> , int> > doors_list;
    //general values
    int width;
    int height;
    int max_rooms;
    int min_room_xy;
    int max_room_xy;
    //random generator
    mt19937 gen;
    //Function generate room
    vector<int> gen_room(int x, int y, int direction);
    //Function check room overlaping or out of bounds
    bool room_overlapping(vector<int> room);
    //Function join the rooms with the corridors
    void join_rooms(vector<int> room_1, vector<int> room_2);
};

#endif

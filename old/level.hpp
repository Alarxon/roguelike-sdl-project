#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <utility> 
#include <string>
#include <random>

using namespace std; 

//Max and min values
#define MAX_LEVEL_WIDTH 1000
#define MAX_LEVEL_HEIGHT 1000
#define MIN_LEVEL_WIDTH 64
#define MIN_LEVEL_HEIGHT 64
#define MAX_ROOMS_LIMIT 100
#define MIN_ROOMS_LIMIT 15
#define MIN_ROOM_XY_LIMIT 5
#define MAX_ROOM_XY_LIMIT 50
#define DEFAULT_RANDOM_CONNECTION 2
#define DEFAULT_RANDOM_SPURS 3

//general values
extern int width;
extern int height;
extern int max_rooms;
extern int min_room_xy;
extern int max_room_xy;

//random generator
extern mt19937 gen;

//Tiles
enum TILES{STONE = 'B', FLOOR = ' ', WALL = 'P', DOOR = 'D'};

//Vector level (generation), vector room_list (x,y,w,h) and vector corridor_list (x,y)
extern vector<vector<char>> level;
extern vector<vector<int>> room_list;
extern vector<vector<pair<int,int>> > corridor_list;

//Function generate room
vector<int> gen_room(int x, int y, int direction);
//Function if a point is between other two points
bool point_between_points(pair<int,int> a, pair<int,int> b, pair<int,int> c);
//Function check room overlaping or out of bounds
bool room_overlapping(vector<int> room);
//Function join the rooms with the corridors
void join_rooms(vector<int> room_1, vector<int> room_2);

//Function level generation
vector<vector<char>> gen_level(int w = MIN_LEVEL_WIDTH, int h = MIN_LEVEL_HEIGHT, 
                    int max_r = MIN_ROOMS_LIMIT, int min_r_xy = MIN_ROOM_XY_LIMIT, 
                    int max_r_xy = MAX_ROOM_XY_LIMIT);

//Function get level
vector<vector<char>> get_level(void);
//Function get rooms
vector<vector<int>> get_rooms_list(void);
//Function get corridors
vector<vector<pair<int,int>> > get_corridor_list(void);

//Printf to console for debug
void print_to_console(vector<vector<char>> level);

//Print to console with ASCII
void print_with_ascii(vector<vector<char>> level);


void print_room_list();
void print_corridors();

#endif
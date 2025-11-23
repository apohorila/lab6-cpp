#include "player.h"
#include <random>
#include <iostream>

using namespace std;

Player play_tic_tac_toe(char player_move){
    std::random_device r;
    mt19937 gen(r());

    bool game_is_active = true;

    while(game_is_active){
            uniform_int_distribution<>distrib(0,8);
            int chosen_move = distrib(gen);
            co_yield chosen_move;
            int response_from_main = co_await 0;
             if (response_from_main == 2){
                game_is_active=false;
            } 
 
    }
    co_return -1;
}
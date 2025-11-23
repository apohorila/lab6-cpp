//g++ compiler
#include "Player.h" 
#include <iostream>
#include <vector>
#include <utility>
#include <random>

using namespace std;

pair<int, int> to_coords(int move_id) {
    return {move_id / 3, move_id % 3};
}

void print_board(const vector<vector<char>>& board) {
    cout << "  1   2   3" << endl;
    cout << "-------------" << endl;
    for (int i = 0; i < 3; ++i) {
        cout << i + 1 << "| ";
        for (int j = 0; j < 3; ++j) {
            cout << board[i][j] << " | ";
        }
        cout << endl << "-------------" << endl;
    }
}

bool check_win(const vector<vector<char>>& board, char player) {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return true; 
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return true;
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return true;
    return false;
}

int main() {

    vector<vector<char>> board(3, vector<char>(3, ' '));
    Player player_x = play_tic_tac_toe('X');
    Player player_o = play_tic_tac_toe('O');

    Player* current_player = &player_x;
    char current_symbol = 'X';
    int move_count = 0;
    bool game_on = true;
    int winner_code = 0;

    cout<<"Game tic-tac-toe started!" <<endl;
    while (game_on && move_count < 9) {

        Player* recent_move_player = current_player;

        if(current_player->move_next()){
            
            int move_id=current_player->current_value_safety().first;
            auto [r,c] = to_coords(move_id);
           
            bool is_valid = (r >= 0 && r < 3 && c >= 0 && c < 3 && board[r][c] == ' ');
            int response_code = 0;

            if(is_valid){
                
                cout << "\n--- Move " << move_count + 1 << " ---" << endl;
                cout << "Current field:" << endl;
                print_board(board);
                cout << "Player " << current_symbol << " suggested move " << " (" << r + 1 << ", " << c + 1 << ")" << endl;

                board[r][c]=current_symbol;
                response_code = 1;
                move_count++;

                if(check_win(board,current_symbol)){
                    winner_code = (current_symbol == 'X' ? 1 : 2);
                    game_on = false; 
                }

                current_player = (current_symbol == 'X') ? &player_o : &player_x;
                current_symbol = (current_symbol == 'X') ? 'O' : 'X';
                } else {
                response_code = 0; 
                }
                if (!game_on && response_code == 1) {
                 player_x.set_response(2); 
                 player_o.set_response(2);
                } else {
                recent_move_player->set_response(response_code);
                }

        } else {
            cout << "Critical error: player's coroutine" << current_symbol << " stopped too soon." << endl;
            game_on = false; 
        }
    }

    cout << "\n===================================" << endl;
    print_board(board);

    if (winner_code == 1) {
        cout << "Winner is player X!" << endl;
    } else if (winner_code == 2) {
        cout << "Winner is player O!" << endl;
    } else { 
        cout << "It's a tie!" << endl;
    }
    cout << "===================================" << endl;

    return 0;

}
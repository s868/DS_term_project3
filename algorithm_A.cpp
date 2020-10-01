#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions
 * 1. ~ 4. are listed in next block)
 *
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 *
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/

class ALERT {
public:
	int x, y;
	ALERT() : x(-1), y(-1) {}
};

ALERT alert[35];
int alt_cnt;
int map[5][6];
int use;	//0 代表不要用map(map全是0)

bool game_point(Board board, Player player, int tmp[]);
void game_sim2(Board board, Player player, int tmp[]);
int game_sim3_check(int r, int c, Board board, Player player);
bool explode_defend(Board board, Player player, int tmp[]);
bool add_orb_defend(Board board, Player player, int tmp[]);
bool corner(Board board, Player player, int tmp[]);
bool explode_corner_replaceEnemy(Board board, Player player, int tmp[]);
bool get_enemy_corner(Board board, Player player, int tmp[]);
bool side(Board board, Player player, int tmp[]);
bool explode_corner(Board board, Player player, int tmp[]);
bool remaining(Board board, Player player, int tmp[]);


void algorithm_A(Board board, Player player, int index[]) {
	srand(time(NULL));

	int row, col;
	int color = player.get_color(), e;
	int tmp[2];
	int i, j;
	int v;

	if (color == 'r') e = 'b';
	else if (color == 'b') e = 'r';

	alert[0].x = -1; alert[0].y = -1;
	alt_cnt = 0;

	while (1) {
		while (1) {
			use = 1;
			v = 0;
			if (game_point(board, player, tmp)) {
				row = tmp[0];
				col = tmp[1];
				if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w')
					break;
			}

			game_sim2(board, player, tmp); //若下了這步，對方可能再一步就會贏的話，map[i][j]會設成false

			if (alt_cnt > 0) {
				for (i = 0; i < alt_cnt; i++) {
					map[alert[i].x][alert[i].y] = 0;
				}
			}

			for (i = 0; i < 5; i++) {
				for (j = 0; j < 6; j++) {
					if (map[i][j]) {
						v = 1;
						break;
					}
				}
			}

			if (!v)  use = 0;

			if (!use && alt_cnt > 0) {
				row = alert[0].x;
				col = alert[0].y;
				break;
			}

			if (explode_defend(board, player, tmp)) {
				row = tmp[0];
				col = tmp[1];
				if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w')
					break;
			}
			if (add_orb_defend(board, player, tmp)) {
				row = tmp[0];
				col = tmp[1];
				if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w')
					break;
			}
			if (explode_corner_replaceEnemy(board, player, tmp)) {
				row = tmp[0];
				col = tmp[1];
				if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w')
					break;
			}
			if (explode_corner(board, player, tmp)) {
				row = tmp[0];
				col = tmp[1];
				if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w')
					break;
			}
			if (get_enemy_corner(board, player, tmp)) {
				row = tmp[0];
				col = tmp[1];
				if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w')
					break;
			}
			if (corner(board, player, tmp)) {
				row = tmp[0];
				col = tmp[1];
				if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w')
					break;
			}
			if (side(board, player, tmp)) {
				row = tmp[0];
				col = tmp[1];
				if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w')
					break;
			}
			if (remaining(board, player, tmp)) {
				row = tmp[0];
				col = tmp[1];
				if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w')
					break;
			}
			
			int go = 0;
			if (use) {
				for (i = 0; i < 5; i++) {
					for (j = 0; j < 6; j++) {
						if (!map[i][j]) {
							continue;
						}
						else {
							if (board.get_cell_color(i, j) == color || board.get_cell_color(i, j) == 'w') {
								row = i;
								col = j;
								go = 1;
								break;
							}
						}
					}
					if (go) break;
				}
				if (go) break;
			}
			else {
				row = rand() % 5;
				col = rand() % 6;
				if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') {
					break;
				}
			}
		}

		if (use) {
			if (game_sim3_check(row, col, board, player)) {
				break;
			}
			else {
				alert[alt_cnt].x = row;
				alert[alt_cnt].y = col;
				alt_cnt++;
			}
		}
		else {
			break;
		}
	}
	index[0] = row;
	index[1] = col;
}


bool game_point(Board board, Player player, int tmp[]) {
	int orb_num;
	char symbol;

	Board tmp_board = board;
	Player *tmp_player;
	char color = player.get_color();
	int i, j;

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 6; j++) {
			board = tmp_board;
			tmp_player = new Player(color);
			if (board.place_orb(i, j, &(*tmp_player))) {
				if (board.win_the_game(*tmp_player)) {
					tmp[0] = i;
					tmp[1] = j;
					return true;
				}
			}
		}
	}
	return false;
}

void game_sim2(Board board, Player player, int tmp[]) {
	bool stop = 0;
	char e, color;
	if (player.get_color() == 'r')	e = 'b';
	else if (player.get_color() == 'b')	e = 'r';
	Player *enemy, *tmp_player;
	Board tmp_board, tmp_board2;
	int i, j, m, n;
	color = player.get_color();


	for (i = 0; i < 5; i++) {
		for (j = 0; j < 6; j++) {
			map[i][j] = 1;
		}
	}

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 6; j++) {
			if (board.get_cell_color(i, j) == e)
				map[i][j] = 0;
		}
	}

	tmp_board = board;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 6; j++) {
			board = tmp_board;
			tmp_player = new Player(color);
			if (board.place_orb(i, j, &(*tmp_player))) {
				if (!board.win_the_game(*tmp_player)) {
					tmp_board2 = board;
					stop = 0;
					for (m = 0; m < 5; m++) {
						for (n = 0; n < 6; n++) {
							board = tmp_board2;
							enemy = new Player(e);
							if (board.place_orb(m, n, &(*enemy))) {
								if (board.win_the_game(*enemy)) {
									map[i][j] = 0;
									stop = 1;
									break;
								}
							}
						}
						if (stop) break;
					}
				}
			}
		}
	}
	return;
}

int game_sim3_check(int r, int c, Board board, Player player) {
	bool stop1 = 0;
	char e, color;
	Player *enemy, *tmp_player;
	Board tmp_board, tmp_board2;
	int i, j, m, n, x, y;
	int b[2];
	int a = 0;

	color = player.get_color();
	if (color == 'r')	e = 'b';
	else if (color == 'b')	e = 'r';

	tmp_player = new Player(color);
	if (board.place_orb(r, c, &(*tmp_player))) {
		if (!board.win_the_game(*tmp_player)) {
			tmp_board2 = board;
			for (m = 0; m < 5; m++) {
				for (n = 0; n < 6; n++) {
					board = tmp_board2;
					enemy = new Player(e);
					if (board.place_orb(m, n, &(*enemy))) {
						if (!board.win_the_game(*enemy)) {
							game_sim2(board, *tmp_player, b);
							a = 0;
							for (x = 0; x < 5; x++) {
								for (y = 0; y < 6; y++) {
									if (map[x][y]) {
										a = 1;
										break;
									}
								}
								if (a) break;
							}
							if (x == 5 && y == 6) {	//代表模擬的結果，必輸(整個map都是0) 
							//代表下了(i, j)後，對方下了某個(m, n)後，我不管下哪裡，對方都有機會再一步就贏
								return 0;
							}
						}
						else {	//map應該有設成0，所以照理來說不會進到這裡(除非map都是0)
							return 0;
						}
					}
				}
			}
		}
		else {	//game point
			return 1;
		}
	}
	else{	//放置違規
		return 0;
	}
	return 1;
}

bool explode_defend(Board board, Player player, int tmp[]) {
	int color = player.get_color();

	if (board.get_cell_color(1, 1) == color && board.get_orbs_num(1, 1) == 3) {
		if (board.get_orbs_num(1, 0) == 2 && board.get_orbs_num(0, 1) == 2) {
			if (board.get_cell_color(1, 0) != color || board.get_cell_color(0, 1) != color) {
				tmp[0] = 1;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(3, 1) == color && board.get_orbs_num(3, 1) == 3) {
		if (board.get_orbs_num(3, 0) == 2 && board.get_orbs_num(4, 1) == 2) {
			if (board.get_cell_color(3, 0) != color || board.get_cell_color(4, 1) != color) {
				tmp[0] = 3;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(1, 4) == color && board.get_orbs_num(1, 4) == 3) {
		if (board.get_orbs_num(0, 4) == 2 && board.get_orbs_num(1, 5) == 2) {
			if (board.get_cell_color(0, 4) != color || board.get_cell_color(1, 5) != color) {
				tmp[0] = 1;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(3, 4) == color && board.get_orbs_num(3, 4) == 3) {
		if (board.get_orbs_num(3, 5) == 2 && board.get_orbs_num(4, 4) == 2) {
			if (board.get_cell_color(3, 5) != color || board.get_cell_color(4, 4) != color) {
				tmp[0] = 3;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	return false;
}

bool add_orb_defend(Board board, Player player, int tmp[]) {
	int color = player.get_color();

	if (board.get_cell_color(0, 0) != color && board.get_cell_color(0, 0) != 'w') {
		if (board.get_orbs_num(0, 1) == 1 && board.get_orbs_num(1, 0) == 1) {
			if (board.get_cell_color(1, 1) == color && board.get_orbs_num(1, 1) == 2) {
				tmp[0] = 1;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(0, 5) != color && board.get_cell_color(0, 5) != 'w') {
		if (board.get_orbs_num(0, 4) == 1 && board.get_orbs_num(1, 5) == 1) {
			if (board.get_cell_color(1, 4) == color && board.get_orbs_num(1, 4) == 2) {
				tmp[0] = 1;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(4, 5) != color && board.get_cell_color(4, 5) != 'w') {
		if (board.get_orbs_num(4, 4) == 1 && board.get_orbs_num(3, 5) == 1) {
			if (board.get_cell_color(3, 4) == color && board.get_orbs_num(3, 4) == 2) {
				tmp[0] = 3;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(4, 0) != color && board.get_cell_color(4, 0) != 'w') {
		if (board.get_orbs_num(3, 0) == 1 && board.get_orbs_num(4, 1) == 1) {
			if (board.get_cell_color(3, 1) == color && board.get_orbs_num(3, 1) == 2) {
				tmp[0] = 3;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	return false;
}

bool corner(Board board, Player player, int tmp[]) {
	int color = player.get_color();

	//角落附近都是友方
	if (board.get_orbs_num(0, 0) == 0 && board.get_cell_color(0, 1) == color && board.get_cell_color(1, 0) == color) {
		tmp[0] = 0;
		tmp[1] = 0;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}
	if (board.get_orbs_num(0, 5) == 0 && board.get_cell_color(0, 4) == color && board.get_cell_color(1, 5) == color) {
		tmp[0] = 0;
		tmp[1] = 5;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}
	if (board.get_orbs_num(4, 0) == 0 && board.get_cell_color(3, 0) == color && board.get_cell_color(4, 1) == color) {
		tmp[0] = 4;
		tmp[1] = 0;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}
	if (board.get_orbs_num(4, 5) == 0 && board.get_cell_color(3, 5) == color && board.get_cell_color(4, 4) == color) {
		tmp[0] = 4;
		tmp[1] = 5;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}

	//角落附近有一邊是空的，一邊隨便(但不是空的)
	if (board.get_orbs_num(0, 0) == 0 &&
		((board.get_cell_color(0, 1) != 'w' && board.get_cell_color(1, 0) == 'w') || (board.get_cell_color(0, 1) == 'w' && board.get_cell_color(1, 0) != 'w'))) {
		tmp[0] = 0;
		tmp[1] = 0;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}
	if (board.get_orbs_num(0, 5) == 0 &&
		((board.get_cell_color(0, 4) != 'w' && board.get_cell_color(1, 5) == 'w') || (board.get_cell_color(0, 4) == 'w' && board.get_cell_color(1, 5) != 'w'))) {
		tmp[0] = 0;
		tmp[1] = 5;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}
	if (board.get_orbs_num(4, 0) == 0 &&
		((board.get_cell_color(3, 0) != 'w' && board.get_cell_color(4, 1) == 'w') || (board.get_cell_color(3, 0) == 'w' && board.get_cell_color(4, 1) != 'w'))) {
		tmp[0] = 4;
		tmp[1] = 0;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}
	if (board.get_orbs_num(4, 5) == 0 &&
		((board.get_cell_color(3, 5) != 'w' && board.get_cell_color(4, 4) == 'w') || (board.get_cell_color(3, 5) == 'w' && board.get_cell_color(4, 4) != 'w'))) {
		tmp[0] = 4;
		tmp[1] = 5;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}

	//角落附近有敵方的棋 (兩邊都有敵方兩顆棋的話不放)
	if (board.get_orbs_num(0, 0) == 0 && ((board.get_cell_color(0, 1) != color && board.get_cell_color(0, 1) != 'w') || (board.get_cell_color(1, 0) != color && board.get_cell_color(1, 0) != 'w'))) {
		if (!(board.get_orbs_num(0, 1) == 2 && board.get_orbs_num(1, 0) == 2)) {
			tmp[0] = 0;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_orbs_num(0, 5) == 0 && ((board.get_cell_color(0, 4) != color && board.get_cell_color(0, 4) != 'w') || (board.get_cell_color(1, 5) != color && board.get_cell_color(1, 5) != 'w'))) {
		if (!(board.get_orbs_num(0, 4) == 2 && board.get_orbs_num(1, 5) == 2)) {
			tmp[0] = 0;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_orbs_num(4, 0) == 0 && ((board.get_cell_color(3, 0) != color && board.get_cell_color(3, 0) != 'w') || (board.get_cell_color(4, 1) != color && board.get_cell_color(4, 1) != 'w'))) {
		if (!(board.get_orbs_num(3, 0) == 2 && board.get_orbs_num(4, 1) == 2)) {
			tmp[0] = 4;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_orbs_num(4, 5) == 0 && ((board.get_cell_color(3, 5) != color && board.get_cell_color(3, 5) != 'w') || (board.get_cell_color(4, 4) != color && board.get_cell_color(4, 4) != 'w'))) {
		if (!(board.get_orbs_num(3, 5) == 2 && board.get_orbs_num(4, 4) == 2)) {
			tmp[0] = 4;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	int ok = 0;
	//角落附近全空
	if (board.get_orbs_num(0, 0) == 0 && board.get_orbs_num(0, 1) == 0 && board.get_orbs_num(1, 0) == 0) {
		tmp[0] = 0;
		tmp[1] = 0;
		if (board.get_cell_color(0, 5) == color || board.get_cell_color(4, 0) == color) //優先選可連成一條的角落
		{
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		else
			ok = 1;
	}
	if (board.get_orbs_num(0, 5) == 0 && board.get_orbs_num(0, 4) == 0 && board.get_orbs_num(1, 5) == 0) {
		tmp[0] = 0;
		tmp[1] = 5;
		if (board.get_cell_color(0, 0) == color || board.get_cell_color(4, 5) == color)
		{
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		else
			ok = 2;
	}
	if (board.get_orbs_num(4, 0) == 0 && board.get_orbs_num(3, 0) == 0 && board.get_orbs_num(4, 1) == 0) {
		tmp[0] = 4;
		tmp[1] = 0;
		if (board.get_cell_color(0, 0) == color || board.get_cell_color(4, 5) == color)
		{
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		else
			ok = 3;
	}
	if (board.get_orbs_num(4, 5) == 0 && board.get_orbs_num(4, 4) == 0 && board.get_orbs_num(3, 5) == 0) {
		tmp[0] = 4;
		tmp[1] = 5;
		if (board.get_cell_color(0, 5) == color || board.get_cell_color(4, 0) == color)
		{
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		else ok = 4;
	}

	if (ok == 1) { tmp[0] = 0; tmp[1] = 0; if (use == map[tmp[0]][tmp[1]])  return true; }
	if (ok == 2) { tmp[0] = 0; tmp[1] = 5; if (use == map[tmp[0]][tmp[1]])  return true; }
	if (ok == 3) { tmp[0] = 4; tmp[1] = 0; if (use == map[tmp[0]][tmp[1]])  return true; }
	if (ok == 4) { tmp[0] = 4; tmp[1] = 5; if (use == map[tmp[0]][tmp[1]])  return true; }

	return false;
}

bool explode_corner_replaceEnemy(Board board, Player player, int tmp[]) {
	int color = player.get_color();

	//角落一邊有敵方兩顆，另一邊一顆(隨便)
	if (board.get_cell_color(0, 0) == color) {
		if (board.get_cell_color(0, 1) != color && board.get_cell_color(0, 1) != 'w' && board.get_orbs_num(0, 1) == 2 && board.get_orbs_num(1, 0) == 1) {
			tmp[0] = 0;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(1, 0) != color && board.get_cell_color(1, 0) != 'w' && board.get_orbs_num(1, 0) == 2 && board.get_orbs_num(0, 1) == 1) {
			tmp[0] = 0;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_cell_color(4, 0) == color) {
		if (board.get_cell_color(3, 0) != color && board.get_cell_color(3, 0) != 'w' && board.get_orbs_num(3, 0) == 2 && board.get_orbs_num(4, 1) == 1) {
			tmp[0] = 4;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(4, 1) != color && board.get_cell_color(4, 1) != 'w' && board.get_orbs_num(4, 1) == 2 && board.get_orbs_num(3, 0) == 1) {
			tmp[0] = 4;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_cell_color(0, 5) == color) {
		if (board.get_cell_color(0, 4) != color && board.get_cell_color(0, 4) != 'w' && board.get_orbs_num(0, 4) == 2 && board.get_orbs_num(1, 5) == 1) {
			tmp[0] = 0;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(1, 5) != color && board.get_cell_color(1, 5) != 'w' && board.get_orbs_num(1, 5) == 2 && board.get_orbs_num(0, 4) == 1) {
			tmp[0] = 0;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_cell_color(4, 5) == color) {
		if (board.get_cell_color(3, 5) != color && board.get_cell_color(3, 5) != 'w' && board.get_orbs_num(3, 5) == 2 && board.get_orbs_num(4, 4) == 1) {
			tmp[0] = 4;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(4, 4) != color && board.get_cell_color(4, 4) != 'w' && board.get_orbs_num(4, 4) == 2 && board.get_orbs_num(3, 5) == 1) {
			tmp[0] = 4;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	//角落一邊有敵方，另一邊有一顆(隨便)
	if (board.get_cell_color(0, 0) == color) {
		if (board.get_cell_color(0, 1) != color && board.get_cell_color(0, 1) != 'w' && board.get_orbs_num(0, 1) == 1 && board.get_orbs_num(1, 0) == 1) {
			tmp[0] = 0;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(1, 0) != color && board.get_cell_color(1, 0) != 'w' && board.get_orbs_num(1, 0) == 1 && board.get_orbs_num(0, 1) == 1) {
			tmp[0] = 0;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_cell_color(4, 0) == color) {
		if (board.get_cell_color(3, 0) != color && board.get_cell_color(3, 0) != 'w' && board.get_orbs_num(3, 0) == 1 && board.get_orbs_num(4, 1) == 1) {
			tmp[0] = 4;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(4, 1) != color && board.get_cell_color(4, 1) != 'w' && board.get_orbs_num(4, 1) == 1 && board.get_orbs_num(3, 0) == 1) {
			tmp[0] = 4;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_cell_color(0, 5) == color) {
		if (board.get_cell_color(0, 4) != color && board.get_cell_color(0, 4) != 'w' && board.get_orbs_num(0, 4) == 1 && board.get_orbs_num(1, 5) == 1) {
			tmp[0] = 0;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(1, 5) != color && board.get_cell_color(1, 5) != 'w' && board.get_orbs_num(1, 5) == 1 && board.get_orbs_num(0, 4) == 1) {
			tmp[0] = 0;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_cell_color(4, 5) == color) {
		if (board.get_cell_color(3, 5) != color && board.get_cell_color(3, 5) != 'w' && board.get_orbs_num(3, 5) == 1 && board.get_orbs_num(4, 4) == 1) {
			tmp[0] = 4;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(4, 4) != color && board.get_cell_color(4, 4) != 'w' && board.get_orbs_num(4, 4) == 1 && board.get_orbs_num(3, 5) == 1) {
			tmp[0] = 4;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	return false;
}

bool get_enemy_corner(Board board, Player player, int tmp[]) {
	int color = player.get_color();

	//先爆enemy的角落兩邊都是兩顆(其中一邊是友方就好)
	if (board.get_cell_color(0, 0) != 'w' && board.get_cell_color(0, 0) != color) {
		if (board.get_orbs_num(0, 1) == 2 && board.get_orbs_num(1, 0) == 2) {
			//先爆可以爆到空的或敵人的
			if (board.get_cell_color(0, 1) == color && board.get_cell_color(0, 2) != color) {
				tmp[0] = 0;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
			if (board.get_cell_color(1, 0) == color && board.get_cell_color(2, 0) != color) {
				tmp[0] = 1;
				tmp[1] = 0;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}

			if (board.get_cell_color(0, 1) == color) {
				tmp[0] = 0;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
			if (board.get_cell_color(1, 0) == color) {
				tmp[0] = 1;
				tmp[1] = 0;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(4, 0) != 'w' && board.get_cell_color(4, 0) != color) {
		if (board.get_orbs_num(4, 1) == 2 && board.get_orbs_num(3, 0) == 2) {
			//先爆可以爆到空的或敵人的
			if (board.get_cell_color(4, 1) == color && board.get_cell_color(4, 2) != color) {
				tmp[0] = 4;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
			if (board.get_cell_color(3, 0) == color && board.get_cell_color(2, 0) != color) {
				tmp[0] = 3;
				tmp[1] = 0;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}

			if (board.get_cell_color(4, 1) == color) {
				tmp[0] = 4;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
			if (board.get_cell_color(3, 0) == color) {
				tmp[0] = 3;
				tmp[1] = 0;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(4, 5) != 'w' && board.get_cell_color(4, 5) != color) {
		if (board.get_orbs_num(4, 4) == 2 && board.get_orbs_num(3, 5) == 2) {
			//先爆可以爆到空的或敵人的
			if (board.get_cell_color(4, 4) == color && board.get_cell_color(4, 3) != color) {
				tmp[0] = 4;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
			if (board.get_cell_color(3, 5) == color && board.get_cell_color(2, 5) != color) {
				tmp[0] = 3;
				tmp[1] = 5;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}

			if (board.get_cell_color(4, 4) == color) {
				tmp[0] = 4;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
			if (board.get_cell_color(3, 5) == color) {
				tmp[0] = 3;
				tmp[1] = 5;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(0, 5) != 'w' && board.get_cell_color(0, 5) != color) {
		if (board.get_orbs_num(0, 4) == 2 && board.get_orbs_num(1, 5) == 2) {
			//先爆可以爆到空的或敵人的
			if (board.get_cell_color(0, 4) == color && board.get_cell_color(0, 3) != color) {
				tmp[0] = 0;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
			if (board.get_cell_color(1, 5) == color && board.get_cell_color(2, 5) != color) {
				tmp[0] = 1;
				tmp[1] = 5;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}

			if (board.get_cell_color(0, 4) == color) {
				tmp[0] = 0;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
			if (board.get_cell_color(1, 5) == color) {
				tmp[0] = 1;
				tmp[1] = 5;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}


	//enemy的角落只有一邊是友方兩顆
	if (board.get_cell_color(0, 0) != 'w' && board.get_cell_color(0, 0) != color) {
		//先爆可以爆到空的或敵人的
		if (board.get_cell_color(0, 1) == color && board.get_cell_color(0, 2) != color) {
			if (board.get_orbs_num(0, 1) == 2) {
				tmp[0] = 0;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
		if (board.get_cell_color(1, 0) == color && board.get_cell_color(2, 0) != color) {
			if (board.get_orbs_num(1, 0) == 2) {
				tmp[0] = 1;
				tmp[1] = 0;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}

		if (board.get_cell_color(0, 1) == color) {
			if (board.get_orbs_num(0, 1) == 2) {
				tmp[0] = 0;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
		if (board.get_cell_color(1, 0) == color) {
			if (board.get_orbs_num(1, 0) == 2) {
				tmp[0] = 1;
				tmp[1] = 0;
				if (use == map[tmp[0]][tmp[1]])  return true;

			}
		}
	}

	if (board.get_cell_color(0, 5) != 'w' && board.get_cell_color(0, 5) != color) {
		if (board.get_cell_color(0, 4) == color && board.get_cell_color(0, 3) != color) {
			if (board.get_orbs_num(0, 4) == 2) {
				tmp[0] = 0;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
		if (board.get_cell_color(1, 5) == color && board.get_cell_color(2, 5) == color) {
			if (board.get_orbs_num(1, 5) == 2) {
				tmp[0] = 1;
				tmp[1] = 5;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
		if (board.get_cell_color(0, 4) == color) {
			if (board.get_orbs_num(0, 4) == 2) {
				tmp[0] = 0;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
		if (board.get_cell_color(1, 5) == color) {
			if (board.get_orbs_num(1, 5) == 2) {
				tmp[0] = 1;
				tmp[1] = 5;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(4, 0) != 'w' && board.get_cell_color(4, 0) != color) {
		if (board.get_orbs_num(3, 0) == 2) {
			if (board.get_cell_color(3, 0) == color && board.get_cell_color(2, 0) != color) {
				tmp[0] = 3;
				tmp[1] = 0;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
		if (board.get_orbs_num(4, 1) == 2) {
			if (board.get_cell_color(4, 1) == color && board.get_cell_color(4, 2) != color) {
				tmp[0] = 4;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}

		if (board.get_orbs_num(3, 0) == 2) {
			if (board.get_cell_color(3, 0) == color) {
				tmp[0] = 3;
				tmp[1] = 0;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
		if (board.get_orbs_num(4, 1) == 2) {
			if (board.get_cell_color(4, 1) == color) {
				tmp[0] = 4;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(4, 5) != 'w' && board.get_cell_color(4, 5) != color) {
		if (board.get_orbs_num(4, 4) == 2) {
			if (board.get_cell_color(4, 4) == color && board.get_cell_color(4, 3) != color) {
				tmp[0] = 4;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
		if (board.get_orbs_num(3, 5) == 2) {
			if (board.get_cell_color(3, 5) == color && board.get_cell_color(2, 5) != color) {
				tmp[0] = 3;
				tmp[1] = 5;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}

		if (board.get_orbs_num(4, 4) == 2) {
			if (board.get_cell_color(4, 4) == color) {
				tmp[0] = 4;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
		if (board.get_orbs_num(3, 5) == 2) {
			if (board.get_cell_color(3, 5) == color) {
				tmp[0] = 3;
				tmp[1] = 5;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	
	//若角落對角為三顆棋，爆掉它
	if (board.get_cell_color(1, 1) == color && board.get_orbs_num(1, 1) == 3) {
		if (board.get_orbs_num(1, 0) == 2 || board.get_orbs_num(0, 1) == 2) {
			if (board.get_cell_color(1, 0) != color || board.get_cell_color(0, 1) != color) {
				tmp[0] = 1;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(3, 1) == color && board.get_orbs_num(3, 1) == 3) {
		if (board.get_orbs_num(3, 0) == 2 || board.get_orbs_num(4, 1) == 2) {
			if (board.get_cell_color(3, 0) != color || board.get_cell_color(4, 1) != color) {
				tmp[0] = 3;
				tmp[1] = 1;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(1, 4) == color && board.get_orbs_num(1, 4) == 3) {
		if (board.get_orbs_num(0, 4) == 2 || board.get_orbs_num(1, 5) == 2) {
			if (board.get_cell_color(0, 4) != color || board.get_cell_color(1, 5) != color) {
				tmp[0] = 1;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	if (board.get_cell_color(3, 4) == color && board.get_orbs_num(3, 4) == 3) {
		if (board.get_orbs_num(3, 5) == 2 || board.get_orbs_num(4, 4) == 2) {
			if (board.get_cell_color(3, 5) != color || board.get_cell_color(4, 4) != color) {
				tmp[0] = 3;
				tmp[1] = 4;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}


	return false;
}


bool side(Board board, Player player, int tmp[]) {
	int color = player.get_color();

	//填角落附近的格子，先填有連線的
	if (board.get_cell_color(0, 0) == color && board.get_cell_color(4, 0) == color) {
		if (board.get_cell_color(1, 0) == 'w') {
			tmp[0] = 1;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(3, 0) == 'w') {
			tmp[0] = 3;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}

		if (board.get_cell_color(4, 1) == 'w') {
			tmp[0] = 4;
			tmp[1] = 1;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(0, 1) == 'w') {
			tmp[0] = 0;
			tmp[1] = 1;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	if (board.get_cell_color(4, 0) == color && board.get_cell_color(4, 5) == color) {
		if (board.get_cell_color(4, 1) == 'w') {
			tmp[0] = 4;
			tmp[1] = 1;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(4, 4) == 'w') {
			tmp[0] = 4;
			tmp[1] = 4;

			if (use == map[tmp[0]][tmp[1]])  return true;
		}

		if (board.get_cell_color(3, 0) == 'w') {
			tmp[0] = 3;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(3, 5) == 'w') {
			tmp[0] = 3;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	if (board.get_cell_color(0, 5) == color && board.get_cell_color(4, 5) == color) {
		if (board.get_cell_color(1, 5) == 'w') {
			tmp[0] = 1;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(3, 5) == 'w') {
			tmp[0] = 3;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}

		if (board.get_cell_color(4, 4) == 'w') {
			tmp[0] = 4;
			tmp[1] = 4;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(0, 4) == 'w') {
			tmp[0] = 0;
			tmp[1] = 4;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	if (board.get_cell_color(0, 0) == color && board.get_cell_color(0, 5) == color) {
		if (board.get_cell_color(0, 1) == 'w') {
			tmp[0] = 0;
			tmp[1] = 1;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(0, 4) == 'w') {
			tmp[0] = 0;
			tmp[1] = 4;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}

		if (board.get_cell_color(1, 0) == 'w') {
			tmp[0] = 1;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(1, 5) == 'w') {
			tmp[0] = 1;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	//若沒有連線的
	if (board.get_cell_color(0, 0) == color) {
		if (board.get_cell_color(1, 0) == 'w') {
			tmp[0] = 1;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}

		if (board.get_cell_color(0, 1) == 'w') {
			tmp[0] = 0;
			tmp[1] = 1;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	if (board.get_cell_color(4, 0) == color) {
		if (board.get_cell_color(4, 1) == 'w') {
			tmp[0] = 4;
			tmp[1] = 1;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}

		if (board.get_cell_color(3, 0) == 'w') {
			tmp[0] = 3;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	if (board.get_cell_color(0, 5) == color) {
		if (board.get_cell_color(1, 5) == 'w') {
			tmp[0] = 1;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(0, 4) == 'w') {
			tmp[0] = 0;
			tmp[1] = 4;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	if (board.get_cell_color(4, 5) == color) {
		if (board.get_cell_color(4, 4) == 'w') {
			tmp[0] = 4;
			tmp[1] = 4;

			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(3, 5) == 'w') {
			tmp[0] = 3;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	return false;
}


bool explode_corner(Board board, Player player, int tmp[]) {
	int color = player.get_color();

	if (board.get_cell_color(0, 0) == color) {
		if (board.get_orbs_num(0, 1) >= 1 || board.get_orbs_num(1, 0) >= 1) {
			tmp[0] = 0;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	if (board.get_cell_color(0, 5) == color) {
		if (board.get_orbs_num(0, 4) >= 1 || board.get_orbs_num(1, 5) >= 1) {
			tmp[0] = 0;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	if (board.get_cell_color(4, 0) == color) {
		if (board.get_orbs_num(3, 0) >= 1 || board.get_orbs_num(4, 1) >= 1) {
			tmp[0] = 4;
			tmp[1] = 0;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	if (board.get_cell_color(4, 5) == color) {
		if (board.get_orbs_num(3, 5) >= 1 || board.get_orbs_num(4, 4) >= 1) {
			tmp[0] = 4;
			tmp[1] = 5;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	return false;
}


bool remaining(Board board, Player player, int tmp[]) {
	int color = player.get_color();

	//先看自己角落的對角可不可以放 (目前是設最多就放到2個)
	if (board.get_cell_color(0, 0) == color) {
		if ((board.get_cell_color(1, 1) == color && board.get_orbs_num(1, 1) < 2) || board.get_orbs_num(1, 1) == 0) {
			tmp[0] = 1;
			tmp[1] = 1;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_cell_color(0, 5) == color) {
		if ((board.get_cell_color(1, 4) == color && board.get_orbs_num(1, 4) < 2) || board.get_orbs_num(1, 4) == 0) {
			tmp[0] = 1;
			tmp[1] = 4;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_cell_color(4, 0) == color) {
		if ((board.get_cell_color(3, 1) == color && board.get_orbs_num(3, 1) < 2) || board.get_orbs_num(3, 1) == 0) {
			tmp[0] = 3;
			tmp[1] = 1;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_cell_color(4, 5) == color) {
		if ((board.get_cell_color(3, 4) == color && board.get_orbs_num(3, 4) < 2) || board.get_orbs_num(3, 4) == 0) {
			tmp[0] = 3;
			tmp[1] = 4;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	//再去放敵人角落的對角
	if ((board.get_cell_color(1, 1) == color && board.get_orbs_num(1, 1) < 2) || board.get_orbs_num(1, 1) == 0) {
		tmp[0] = 1;
		tmp[1] = 1;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}
	if ((board.get_cell_color(1, 4) == color && board.get_orbs_num(1, 4) < 2) || board.get_orbs_num(1, 4) == 0) {
		tmp[0] = 1;
		tmp[1] = 4;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}
	if ((board.get_cell_color(3, 1) == color && board.get_orbs_num(3, 1) < 2) || board.get_orbs_num(3, 1) == 0) {
		tmp[0] = 3;
		tmp[1] = 1;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}
	if ((board.get_cell_color(3, 4) == color && board.get_orbs_num(3, 4) < 2) || board.get_orbs_num(3, 4) == 0) {
		tmp[0] = 3;
		tmp[1] = 4;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}

	//放邊邊
	if (board.get_cell_color(0, 0) == color) {
		if (board.get_cell_color(1, 0) == color) {
			if ((board.get_cell_color(2, 0) == color && board.get_orbs_num(2, 0) < 2) || board.get_cell_color(2, 0) == 'w') {
				tmp[0] = 2;
				tmp[1] = 0;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}

		}
		if (board.get_cell_color(0, 1) == color) {
			if ((board.get_cell_color(0, 2) == color && board.get_orbs_num(0, 2) < 2) || board.get_cell_color(0, 2) == 'w') {
				tmp[0] = 0;
				tmp[1] = 2;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}
	if (board.get_cell_color(4, 0) == color) {
		if (board.get_cell_color(3, 0) == color) {
			if ((board.get_cell_color(2, 0) == color && board.get_orbs_num(2, 0) < 2) || board.get_cell_color(2, 0) == 'w') {
				tmp[0] = 2;
				tmp[1] = 0;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}

		}
		if (board.get_cell_color(4, 1) == color) {
			if ((board.get_cell_color(4, 2) == color && board.get_orbs_num(4, 2) < 2) || board.get_cell_color(4, 2) == 'w') {
				tmp[0] = 4;
				tmp[1] = 2;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}
	if (board.get_cell_color(0, 5) == color) {
		if (board.get_cell_color(0, 4) == color) {
			if ((board.get_cell_color(0, 3) == color && board.get_orbs_num(0, 3) < 2) || board.get_cell_color(0, 3) == 'w') {
				tmp[0] = 0;
				tmp[1] = 3;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}

		}
		if (board.get_cell_color(1, 5) == color) {
			if ((board.get_cell_color(2, 5) == color && board.get_orbs_num(2, 5) < 2) || board.get_cell_color(2, 5) == 'w') {
				tmp[0] = 2;
				tmp[1] = 5;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}
	if (board.get_cell_color(4, 5) == color) {
		if (board.get_cell_color(4, 4) == color) {
			if ((board.get_cell_color(4, 3) == color && board.get_orbs_num(4, 3) < 2) || board.get_cell_color(4, 3) == 'w') {
				tmp[0] = 4;
				tmp[1] = 3;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}

		}
		if (board.get_cell_color(3, 5) == color) {
			if ((board.get_cell_color(2, 5) == color && board.get_orbs_num(2, 5) < 2) || board.get_cell_color(2, 5) == 'w') {
				tmp[0] = 2;
				tmp[1] = 5;
				if (use == map[tmp[0]][tmp[1]])  return true;
			}
		}
	}

	//中間連線
	if (board.get_cell_color(1, 1) == color && board.get_cell_color(3, 1) == color) {
		if (board.get_cell_color(2, 1) == 'w' || (board.get_cell_color(2, 1) == color && board.get_orbs_num(2, 1) < 3)) {
			tmp[0] = 2;
			tmp[1] = 1;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_cell_color(1, 4) == color && board.get_cell_color(3, 4) == color) {
		if (board.get_cell_color(2, 4) == 'w' || (board.get_cell_color(2, 4) == color && board.get_orbs_num(2, 4) < 3)) {
			tmp[0] = 2;
			tmp[1] = 4;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_cell_color(1, 1) == color && board.get_cell_color(1, 4) == color) {
		if (board.get_cell_color(1, 2) == 'w' || (board.get_cell_color(1, 2) == color && board.get_orbs_num(1, 2) < 3)) {
			tmp[0] = 1;
			tmp[1] = 2;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(1, 3) == 'w' || (board.get_cell_color(1, 3) == color && board.get_orbs_num(1, 3) < 3)) {
			tmp[0] = 1;
			tmp[1] = 3;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}
	if (board.get_cell_color(3, 1) == color && board.get_cell_color(3, 4) == color) {
		if (board.get_cell_color(3, 2) == 'w' || (board.get_cell_color(3, 2) == color && board.get_orbs_num(3, 2) < 3)) {
			tmp[0] = 3;
			tmp[1] = 2;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
		if (board.get_cell_color(3, 3) == 'w' || (board.get_cell_color(3, 3) == color && board.get_orbs_num(3, 3) < 3)) {
			tmp[0] = 3;
			tmp[1] = 3;
			if (use == map[tmp[0]][tmp[1]])  return true;
		}
	}

	//剩中間兩格
	if (board.get_cell_color(2, 2) == color && board.get_orbs_num(2, 2) < 3) {
		tmp[0] = 2;
		tmp[1] = 2;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}
	if (board.get_cell_color(2, 3) == color && board.get_orbs_num(2, 3) < 3) {
		tmp[0] = 2;
		tmp[1] = 3;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}

	if (board.get_cell_color(2, 2) == 'w') {
		tmp[0] = 2;
		tmp[1] = 2;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}
	if (board.get_cell_color(2, 3) == 'w') {
		tmp[0] = 2;
		tmp[1] = 3;
		if (use == map[tmp[0]][tmp[1]])  return true;
	}

	return false;
}


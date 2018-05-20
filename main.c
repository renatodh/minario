/*sudo apt-get install ncurses-dev*/
/*gcc <file-name>.c -lncurses -o <executable-name>*/
#include "player.h" // Also includes display, board and util recursively.

#define GAME_INTRODUCTION_STATE 0
#define MENU_STATE 1
#define START_GAME_STATE 2
#define PLAY_GAME_STATE 3
#define WIN_STATE 4
#define LOSE_STATE 5

void ncursesInit();
void ncursesEnd();
int getRandomIntegerInRange(int min,int max);
bool checkLoseCondition();
bool checkWinCondition();

int main() {
	ncursesInit();
	clock_t timeSinceLastGameBoardDecrease;
	int gameState = GAME_INTRODUCTION_STATE;

	while(!userEscAction){
		updateNextUserAction();

		switch(gameState) {
			case GAME_INTRODUCTION_STATE:
				showGameIntroduction();
				gameState = MENU_STATE;
			break;

			case MENU_STATE:
				if (userEnterAction) {
					clear();
					gameState = START_GAME_STATE;
				}
			break;

			case START_GAME_STATE:
				createPlayers();
				settingGameBoard();
				delay(60);
				gameState = PLAY_GAME_STATE;
			break;

			case PLAY_GAME_STATE:
				updatePlayers();
				playersCollision();
				drawAlivePlayersNumber();
				drawPlayers();
				decreaseGameBoardByInterval(&timeSinceLastGameBoardDecrease);
				if (checkWinCondition())
					gameState = WIN_STATE;
				if (checkLoseCondition())
					gameState = LOSE_STATE;

			break;

			case WIN_STATE:
				showVictoryScreen(OFFSET_HEIGHT, BOARD_HEIGHT, OFFSET_WIDTH);
				if (userEnterAction) {
					clear();
					gameState = GAME_INTRODUCTION_STATE;
				}
			break;

			case LOSE_STATE:
				showFailureScreen(OFFSET_HEIGHT, BOARD_HEIGHT, OFFSET_WIDTH);
				if (userEnterAction) {
					clear();
					gameState = GAME_INTRODUCTION_STATE;
				}
			break;
		}
		delay(1);
	}

	ncursesEnd();
	return 0;
}

void ncursesInit() {
	initscr();
	curs_set(0);
	cbreak();
	noecho();
	keypad(stdscr,true);
	timeout(10);
}

void ncursesEnd() {
	endwin();
}

int getRandomIntegerInRange(int min,int max){
	return  min + getRandomInteger(max-min);
}

bool checkLoseCondition(){
	return !players[0].isAlive;
}

bool checkWinCondition(){
	for (int i = 1; i < PLAYERS_NUMBER; i++){
		if (players[i].isAlive) {
			return false;
		}
	}
	return !checkLoseCondition();
}
#include <stdio.h>
#include <Windows.h>
#include <list>
#include <stack>

#define WALL '#'
#define SNAKE '$'
#define FRUIT '@'
#define BLANK ' '
#define STAGESZ 25

struct BlockInfo {
	unsigned int Positions[2];
	unsigned int PrevPositions[2];
	int Vectors[2];
	char BlockType;
};

struct BlockToDrawInfo {
	unsigned int X;
	unsigned int Y;
	char BlockType;
};

struct BlockToEraseInfo {
	unsigned int X;
	unsigned int Y;
};

class ObjectPhysics {
private:
	BlockInfo Block;
public:
	ObjectPhysics(char blockType ,unsigned int x, unsigned int y) {
		this->Block.Positions[0] = x;
		this->Block.Positions[1] = y;
		this->Block.PrevPositions[0] = STAGESZ + 2;
		this->Block.PrevPositions[1] = STAGESZ + 2;
		this->Block.Vectors[0] = 0;
		this->Block.Vectors[1] = 0;
		this->Block.BlockType = blockType;
	}

	ObjectPhysics(BlockInfo info) {
		this->Block.Positions[0] = info.PrevPositions[0];
		this->Block.Positions[1] = info.PrevPositions[1];
		this->Block.PrevPositions[0] = (info.PrevPositions[0] - info.Vectors[0]) > 0 ? info.PrevPositions[0] - info.Vectors[0] : 50;
		this->Block.PrevPositions[1] = (info.PrevPositions[1] - info.Vectors[1]) > 0 ? info.PrevPositions[1] - info.Vectors[1] : 50;
		this->Block.Vectors[0] = info.Vectors[0];
		this->Block.Vectors[1] = info.Vectors[1];
		this->Block.BlockType = info.BlockType;
	}
	
	void setBlockPos(unsigned int x, unsigned int y) {

		this->Block.PrevPositions[0] = this->Block.Positions[0];
		this->Block.PrevPositions[1] = this->Block.Positions[1];

		this->Block.Positions[0] = x;
		this->Block.Positions[1] = y;

		this->Block.Vectors[0] = 0;
		this->Block.Vectors[1] = 0;
	}

	void blockMoveRight() {
		this->Block.PrevPositions[0] = this->Block.Positions[0];
		this->Block.PrevPositions[1] = this->Block.Positions[1];

		this->Block.Positions[0]++;
		
		this->Block.Vectors[0] = 1;
		this->Block.Vectors[1] = 0;

		this->blockWallBoundaryCheck();
	}

	void blockMoveLeft() {
		this->Block.PrevPositions[0] = this->Block.Positions[0];
		this->Block.PrevPositions[1] = this->Block.Positions[1];

		this->Block.Positions[0]--;

		this->Block.Vectors[0] = -1;
		this->Block.Vectors[1] = 0;

		this->blockWallBoundaryCheck();
	}

	void blockMoveDown() {
		this->Block.PrevPositions[0] = this->Block.Positions[0];
		this->Block.PrevPositions[1] = this->Block.Positions[1];

		this->Block.Positions[1]++;

		this->Block.Vectors[0] = 0;
		this->Block.Vectors[1] = 1;

		this->blockWallBoundaryCheck();
	}

	void blockMoveUp() {
		this->Block.PrevPositions[0] = this->Block.Positions[0];
		this->Block.PrevPositions[1] = this->Block.Positions[1];

		this->Block.Positions[1]--;

		this->Block.Vectors[0] = 0;
		this->Block.Vectors[1] = -1;

		this->blockWallBoundaryCheck();
	}

	void blockWallBoundaryCheck() {
		if (this->Block.Positions[0] > STAGESZ)
			this->Block.Positions[0] = 1;
		else if (this->Block.Positions[0] < 1)
			this->Block.Positions[0] = STAGESZ;

		if (this->Block.Positions[1] > STAGESZ)
			this->Block.Positions[1] = 1;
		else if (this->Block.Positions[1] < 1)
			this->Block.Positions[1] = STAGESZ;
	}

	bool whetherBlockCollide(BlockInfo block) {

		int expectedPos[2] = {this->Block.Positions[0] + this->Block.Vectors[0],
			this->Block.Positions[1] + this->Block.Vectors[1] };

		if (expectedPos[0] > STAGESZ)
			expectedPos[0] = 1;
		else if(expectedPos[0] < 1)
			expectedPos[0] = STAGESZ;

		if (expectedPos[1] > STAGESZ)
			expectedPos[1] = 1;
		else if (expectedPos[1] < 1)
			expectedPos[1] = STAGESZ;

		if (expectedPos[0] == block.Positions[0])
			if (expectedPos[1] == block.Positions[1])
				return true;
		return false;

	}

	BlockInfo getBlockInfo() {
		return this->Block;
	}

};

//class snake should be redesigned
class Snake {
private:
	std::list<ObjectPhysics> snake;
public:
	Snake() {
		this->snake.push_back(ObjectPhysics(SNAKE, 10, 10));
		this->snake.begin()->blockMoveUp();
		this->snakeGrow();
		this->snakeGrow();
	}

	void snakeGrow() {
		BlockInfo buffer = this->snake.back().getBlockInfo();

		this->snake.push_back(ObjectPhysics(SNAKE, buffer.PrevPositions[0], buffer.PrevPositions[1]));

		if (buffer.Vectors[0] == 1) {
			this->snake.back().blockMoveLeft();
			this->snake.back().blockMoveRight();
		}
		else if (buffer.Vectors[0] == -1) {
			this->snake.back().blockMoveRight();
			this->snake.back().blockMoveLeft();
		}
		else {
			if (buffer.Vectors[1] == 1) {
				this->snake.back().blockMoveUp();
				this->snake.back().blockMoveDown();
			}
			else if (buffer.Vectors[1] == -1) {
				this->snake.back().blockMoveDown();
				this->snake.back().blockMoveUp();
			}
		}
	}

	void snakeGoToUp() {
		BlockInfo blockInfoBuffer;
		
		this->snake.begin()->blockMoveUp();
		blockInfoBuffer = this->snake.begin()->getBlockInfo();
		std::list<ObjectPhysics>::iterator iter = this->snake.begin();
		iter++;
		for (;iter != this->snake.end(); iter++) {
			iter->setBlockPos(blockInfoBuffer.PrevPositions[0], blockInfoBuffer.PrevPositions[1]);
			blockInfoBuffer = iter->getBlockInfo();
		}
	}

	void snakeGoToDown() {
		BlockInfo blockInfoBuffer;

		this->snake.begin()->blockMoveDown();
		blockInfoBuffer = this->snake.begin()->getBlockInfo();
		std::list<ObjectPhysics>::iterator iter = this->snake.begin();
		iter++;
		for (; iter != this->snake.end(); iter++) {
			iter->setBlockPos(blockInfoBuffer.PrevPositions[0], blockInfoBuffer.PrevPositions[1]);
			blockInfoBuffer = iter->getBlockInfo();
		}
	}

	void snakeGoToRight() {
		BlockInfo blockInfoBuffer;

		this->snake.begin()->blockMoveRight();
		blockInfoBuffer = this->snake.begin()->getBlockInfo();
		std::list<ObjectPhysics>::iterator iter = this->snake.begin();
		iter++;
		for (; iter != this->snake.end(); iter++) {
			iter->setBlockPos(blockInfoBuffer.PrevPositions[0], blockInfoBuffer.PrevPositions[1]);
			blockInfoBuffer = iter->getBlockInfo();
		}
	}

	void snakeGoToLeft() {
		BlockInfo blockInfoBuffer;

		this->snake.begin()->blockMoveLeft();
		blockInfoBuffer = this->snake.begin()->getBlockInfo();
		std::list<ObjectPhysics>::iterator iter = this->snake.begin();
		iter++;
		for (; iter != this->snake.end(); iter++) {
			iter->setBlockPos(blockInfoBuffer.PrevPositions[0], blockInfoBuffer.PrevPositions[1]);
			blockInfoBuffer = iter->getBlockInfo();
		}
	}

	bool wheterSnakeEatFruit(BlockInfo fruit) {
		if (this->snake.begin()->whetherBlockCollide(fruit)) {
			return true;
		}

		return false;
	}

	bool whetherSnakeGameOver() {
		ObjectPhysics head = *(this->snake.begin());

		std::list<ObjectPhysics>::iterator iter = this->snake.begin();
		iter++;

		for (; iter != this->snake.end(); iter++) {
			if (head.whetherBlockCollide(iter->getBlockInfo())) {
				return true;
			}
		}

		return false;
	}

	std::stack<BlockInfo> getSnakeInfo() {
		std::stack<BlockInfo> buffer;
		
		for (std::list<ObjectPhysics>::iterator iter = this->snake.begin(); iter != this->snake.end(); iter++) {
			buffer.push((*iter).getBlockInfo());
		}

		return buffer;
	}

	BlockInfo getSnakeHead() {
		return this->snake.front().getBlockInfo();
	}
};

class Fruit : public ObjectPhysics{
private : 
	unsigned int Fx;
	unsigned int Fy;
public :
	Fruit(unsigned int x = 20, unsigned int y = 20) : ObjectPhysics(FRUIT, x, y) {
		this->Fx = 0;
		this->Fy = 0;
	}

	void getRandomPos() {
		this->Fx = rand()%(1+STAGESZ);
		this->Fy = rand()%(1+STAGESZ);

		if (this->Fx == 0)
			Fx++;

		if (this->Fy == 0)
			Fy++;
	}

	void moveRandomly() {
		this->getRandomPos();
		this->setBlockPos(this->Fx, this->Fy);
	}
};

class Renderer{
private:
	std::stack<BlockToDrawInfo> toDraw;
	std::stack<BlockToEraseInfo> toErase;
public:
	Renderer() {

		this->CursorView();

		for (unsigned int i = 0; i < 2 + STAGESZ; i++) {
			for (unsigned int j = 0; j < 2 + STAGESZ; j++) {
				if (i == 0) {
					toDraw.push({i, j, WALL});
				}
				else if (i == 1+STAGESZ) {
					toDraw.push({ i, j, WALL });
				}
				else {
					if (j == 0) {
						toDraw.push({ i, j, WALL });
					}
					else if (j == 1 + STAGESZ) {
						toDraw.push({ i, j, WALL });
					}
				}
			}
		}
	}

	void CursorView()
	{
		CONSOLE_CURSOR_INFO cursorInfo = { 0, };
		cursorInfo.dwSize = 1; 
		cursorInfo.bVisible = FALSE; 
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	}

	void gotoXY(unsigned int x, unsigned int y) {
		COORD pos = { x, y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	}

	void printBlock(BlockToDrawInfo info) {
		gotoXY(info.X, info.Y);
		printf("%c", info.BlockType);
	}

	void eraseBlock(BlockToEraseInfo info) {
		gotoXY(info.X, info.Y);
		printf("%c", BLANK);
	}

	void updateStacks(BlockInfo& info) {
		this->toDraw.push({info.Positions[0], info.Positions[1], info.BlockType});
		this->toErase.push({info.PrevPositions[0], info.PrevPositions[1]});
	}

	void updateFruitStack(BlockInfo& info) {
		this->toDraw.push({ info.Positions[0], info.Positions[1], info.BlockType });
		this->toErase.push({ info.PrevPositions[0], info.PrevPositions[1] });
	}

	void updateSnakeStack(std::stack<BlockInfo> snake) {
		while (!snake.empty()) {
			this->updateStacks(snake.top());
			snake.pop();
		}
	}

	void updateScreen(std::stack<BlockInfo> snake, BlockInfo fruit){
		this->updateSnakeStack(snake);
		this->updateFruitStack(fruit);

		while (!this->toErase.empty()) {
			this->eraseBlock(this->toErase.top());
			this->toErase.pop();
		}

		while (!this->toDraw.empty()) {
			this->printBlock(this->toDraw.top());
			this->toDraw.pop();
		}

		while (!this->toDraw.empty()) {
			this->toDraw.pop();
		}

		while (!this->toErase.empty()) {
			this->toErase.pop();
		}
	}
};

class Game {
private:
	Snake snake;
	Fruit fruit;
	Renderer ren;
	int harder;
	
public:
	Game() {
		this->snake = Snake();
		this->fruit = Fruit();
		this->ren = Renderer();
		

		while (!this->snake.whetherSnakeGameOver()) {
			this->snakeControll();
			this->showIt();
			Sleep(200 + this->harder < 50 ? 50 : (200 + this->harder));
		}

		this->ren.gotoXY(0, STAGESZ + 2);
		printf("Game Over!!");
	}

	void snakeControll() {
		if (GetAsyncKeyState(VK_UP))
			this->snake.snakeGoToUp();
		else if (GetAsyncKeyState(VK_DOWN))
			this->snake.snakeGoToDown();
		else if (GetAsyncKeyState(VK_RIGHT))
			this->snake.snakeGoToRight();
		else if (GetAsyncKeyState(VK_LEFT))
			this->snake.snakeGoToLeft();
		else {
			BlockInfo snkHead = this->snake.getSnakeHead();
			if (snkHead.Vectors[0] == 1)
				this->snake.snakeGoToRight();
			if (snkHead.Vectors[0] == -1)
				this->snake.snakeGoToLeft();
			if (snkHead.Vectors[1] == 1)
				this->snake.snakeGoToDown();
			if (snkHead.Vectors[1] == -1)
				this->snake.snakeGoToUp();
		}

		if (this->snake.wheterSnakeEatFruit(this->fruit.getBlockInfo())) {
			this->fruit.moveRandomly();
			this->snake.snakeGrow();
			this->harder--;
		}
	}

	void showIt() {
		ren.updateScreen(this->snake.getSnakeInfo(), this->fruit.getBlockInfo());
	}

};

int main() {
	Game();
	return 0;
}
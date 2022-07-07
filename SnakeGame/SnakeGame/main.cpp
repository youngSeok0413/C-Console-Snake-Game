#include <stdio.h>
#include <Windows.h>
#include <stack>

#define WALL '#'
#define SNAKE '$'
#define FRUIT '@'
#define BLANK ' '
#define STAGESZ 50

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
		this->Block.BlockType = blockType;
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

	bool whetherBlockCollide(ObjectPhysics& other) {

		BlockInfo buffer = other.getBlockInfo();

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

		if (expectedPos[0] == buffer.Positions[0])
			if (expectedPos[1] == buffer.Positions[1])
				return true;
		return false;

	}

	BlockInfo getBlockInfo() {
		return this->Block;
	}

};

class Renderer{
private:
	std::stack<BlockToDrawInfo> toDraw;
	std::stack<BlockToEraseInfo> toErase;
public:
	Renderer() {
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

	void updateScreen() {

		while (!this->toErase.empty()) {
			this->eraseBlock(this->toErase.top());
			this->toDraw.pop();
		}

		while (!this->toDraw.empty()) {
			this->printBlock(this->toDraw.top());
			this->toDraw.pop();
		}
	}
};

int main() {
	Renderer ren;
	ren.updateScreen();
	
	return 0;
}
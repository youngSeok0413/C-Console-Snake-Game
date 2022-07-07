#include <stdio.h>
#include <Windows.h>

#define BLOCK '#'
#define SNAKE '$'
#define FRUIT '@'
#define STAGESZ 50

struct BlockInfo {
	unsigned int Positions[2];
	unsigned int PrevPositions[2];
	int Vectors[2];
	char BlockType;
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

	void blockMoveDown() {
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
public:
	void gotoXY(unsigned int x, unsigned int y) {
		COORD pos = { x, y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	}
};

int main() {

	
	return 0;
}
#ifndef Block_H
#define Block_H

#include "sdlUtil.h"

class Block {
	
private:
	static int block_number;
	int id;
	static int group_counter;
public:
	int group;
	int block_type;
	bool block_active = false;

	Block(int type){
		this->block_type = type;
		this->id = ++block_number;
	};

	Block(int type, bool active) {

		this->block_type = type;
		this->id = ++block_number;

		block_active = active;
		group = group_counter;

		if (++block_number > 4) {
			block_number = 0;
			group_counter++;
		}
	}

	int getId(){
		return this->id;
	};
};

int Block::block_number = 1;
int Block::group_counter = 0;
#endif
/*
 * game.h
 *
 *  Created on: May 25, 2015
 *      Author: TalhaSayed
 */

#ifndef SRC_GAME_H_
#define SRC_GAME_H_

namespace miniphysx {

class game {
public:
	game();
	virtual ~game();

	void start();
	void render();
	void update();


private:
	int windowHeight;
	int windowWidth;


};

} /* namespace miniphysx */

#endif /* SRC_GAME_H_ */

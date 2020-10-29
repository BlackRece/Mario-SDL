#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#define POW_FILE		"POW_Scores.txt"
#define RUN_FILE		"RUN_Scores.txt"

#define NUM_OF_SCORES	10

#include <iostream>

#include "Constants.h"

struct ScoreData {
	std::string name;		//player's name
	std::string bros;		//Mario Brother's name
	int score = -1;			//player's score
};

class HighScores {
protected:
	ScoreData mScores[10];

private:
	static HighScores*	mInstance;
	std::string			mPath;

	HighScores();

public:
	static HighScores* Instance();

	~HighScores();

	void			AddScore();
	int				GetLowScore();
	void			GetName();
	int				GetTotal() { return int(NUM_OF_SCORES); }
	std::string		GetScores(int index);
	bool			LoadScores(std::string score_path = POW_FILE);
	bool			SaveScores(std::string score_path = POW_FILE);
	void			SortScores();

};

#endif // HIGHSCORES_H
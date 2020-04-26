#include "HighScores.h"
#include <fstream>

HighScores* HighScores::mInstance = nullptr;

HighScores::HighScores() {
	for (int i = 0; i < NUM_OF_SCORES; i++) {
		mScores[i].name.clear();
		mScores[i].bros.clear();
		mScores[i].score = -1;
	}
}

HighScores::~HighScores() {
	mInstance = nullptr;
}

HighScores* HighScores::Instance() {
	if (!mInstance)
		mInstance = new HighScores;

	return mInstance;
}

void HighScores::AddScore() {}

void HighScores::LoadScores(std::string score_path) {
	std::ifstream fScores(score_path.c_str());

	if (fScores.good()) {
		for (int i = 0; i < NUM_OF_SCORES; i++) {
			while (!fScores.eof()) {
				fScores >>
					mScores[i].name >>
					mScores[i].bros >>
					mScores[i].score;
			}
		}
	} else {
		std::cerr << "ERROR: Can't open high score file " << score_path.c_str() << std::endl;
	}

	fScores.close();
}

void HighScores::GetName() {}

void HighScores::SaveScores(std::string score_path) {
	std::ofstream fScores(score_path.c_str(), std::ios::out | std::ios::trunc);

	if (fScores.good()) {
		for (int i = 0; i < NUM_OF_SCORES; i++) {
			if (mScores[i].score >= 0) {
				fScores << 
					mScores[i].name <<
					mScores[i].bros <<
					mScores[i].score;
			}
		}
	} else {
		std::cerr << "ERROR: Can't open high score file " << score_path.c_str() << std::endl;
	}

	fScores.close();
}

void HighScores::SortScores() {
	ScoreData tmp[NUM_OF_SCORES];// = { "","",-1 };
	int scoreCounter = 0;
	bool stop = false;
	while( stop == false) {
		for (int i = 0; i < NUM_OF_SCORES; i++) {
			if (scoreCounter < 1) {

				if (mScores[i].score >= 0) {
					if (mScores[i].score > tmp[scoreCounter].score) {
						tmp[scoreCounter] = mScores[i];
						break; //should i break since scores after the highest should be lower?
					}
				} else {
					//the rest of the scores are either 
					//not set or not valid, so ignore
					break;
				}
			} else {
				if (
					mScores[i].score > tmp[scoreCounter].score &&
					mScores[i].score < tmp[scoreCounter - 1].score
				) {
					tmp[scoreCounter] = mScores[i];
					if (scoreCounter < NUM_OF_SCORES - 1) {
						tmp[scoreCounter + 1] = tmp[scoreCounter];
						tmp[scoreCounter] = mScores[i];
					}
					//break; //should i break since scores after the highest should be lower?
				}
			}
		}

		scoreCounter++;
	}
}
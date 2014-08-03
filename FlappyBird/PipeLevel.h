#pragma once

#include "SceneObject.h"
#include "Pipe.h"
#include "Bridge.h"
#include <vector>
#include <deque>

class PipeLevel: public SceneObject
{
private:
	const static float firstPipeDistance;
	const static float pipeDistance;
	const static int   maxActivePipes;

	Bridge &			bridge;
	std::vector<float>	pipeSizes;
	std::deque<Pipe>	activePipes;
	int					nextPendingPipe;
	size_t				nextCollitionPipe;
	size_t				nextCoinPipe;
	bool				pointAwarded;
public:
	PipeLevel(Bridge& bridge);

	void render(Scene &parent);
	void stop();
	void reset();
	bool testCollition(Flappy& flappy);
	bool wasPointAwarded();
private:
	void addActivePipes();
};
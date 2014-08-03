#include "PipeLevel.h"
#include <pugixml.hpp>

using namespace pugi;
const float PipeLevel::firstPipeDistance = 180;
const float PipeLevel::pipeDistance = 60;
const int PipeLevel::maxActivePipes = 20;

PipeLevel::PipeLevel(Bridge& bridge):
	bridge(bridge)
{
	xml_document doc;
	xml_parse_result result = doc.load_file("level.xml");
	if(!result){
		throw exception("Couldn't load file ");
	}
	xpath_query queryHeights("//height");
	xpath_node_set heights = queryHeights.evaluate_node_set(doc);
	for(xpath_node_set::const_iterator it = heights.begin(); it != heights.end(); ++it){
		

		pipeSizes.push_back(it->node().attribute("value").as_float());
	}
	reset();
}

void PipeLevel::stop()
{
	for(std::deque<Pipe>::iterator it = activePipes.begin(); it != activePipes.end(); ++it)
	{
		it->stop();
	}
}

void PipeLevel::reset()
{
	nextPendingPipe = 0;
	activePipes.clear();
	addActivePipes();
	nextCollitionPipe = nextCoinPipe = activePipes.size() -1;
	pointAwarded = false;
}

bool PipeLevel::testCollition(Flappy &flappy)
{
	return activePipes.at(nextCollitionPipe).collidesWith(flappy);
}

bool PipeLevel::wasPointAwarded()
{
	bool res = pointAwarded;
	pointAwarded = false;
	return res;
}

void PipeLevel::addActivePipes()
{
	while(activePipes.size() >= maxActivePipes)
	{
		activePipes.back().clean();
		activePipes.pop_back();
	}
	while(activePipes.size() < maxActivePipes)
	{
		float pipePosition;
		if(activePipes.empty())
		{
			pipePosition = firstPipeDistance;
		} else {
			pipePosition = activePipes.front().getAbsolutePosition() + pipeDistance;
		}
		activePipes.push_front(Pipe(bridge, pipePosition, pipeSizes.at(nextPendingPipe)));
		nextPendingPipe = (nextPendingPipe + 1) % pipeSizes.size();
		++nextCollitionPipe;
		++nextCoinPipe;
	}
}

void PipeLevel::render(Scene &parent)
{
	for(std::deque<Pipe>::iterator it = activePipes.begin(); it != activePipes.end(); ++it)
	{
		it->render(parent);
	}
	
	if(!activePipes.empty() && activePipes.back().getAbsolutePosition() < -firstPipeDistance*3)
	{
		activePipes.back().clean();
		activePipes.pop_back();
		addActivePipes();
	}


	if(activePipes.at(nextCollitionPipe).pastPipe())
		nextCollitionPipe--;
	if(activePipes.at(nextCoinPipe).pastMiddle()){
		pointAwarded = true;
		nextCoinPipe--;
	}
}
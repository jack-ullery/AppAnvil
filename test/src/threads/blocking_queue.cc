#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../../../src/threads/blocking_queue.h"

using ::testing::AtLeast; 

class BLOCKING_QUEUE : public ::testing::Test
{
protected:
  	
	BLOCKING_QUEUE();

	virtual void SetUp(){
		queue->clear();
	}
	
	std::deque<int> internal_queue;
	std::shared_ptr<std::mutex> mtx;
    std::shared_ptr<BlockingQueue<int>> queue;
};

BLOCKING_QUEUE::BLOCKING_QUEUE()
:	mtx{new std::mutex()},
	queue{new BlockingQueue<int>(internal_queue, mtx)}
{	}

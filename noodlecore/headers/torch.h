#pragma once
#include "noodlecore.h"

namespace Noodle {
/*
shape
Lower value is for flatter search like bfs. Higher value makes it deeper.

order emphasis
Higher values cause the search to prioritize elements with similar connectome.
For example, elements of a simple group or progression will be loaded and unloaded together.

power
More power means the spread loads more doodles and uses more compute.

volatility manifold
The rate and degree to which torch iterates over distal regions of the subspace.
The effect is like a flame dancing around, temporarily illuminating some things then others.
This is configured in a file as a function of depth and time.
This allows the effect to die off gracefully for example.
*/

// // Used to point to a node in a graph.
// template<typename graphT>
// class GraphCursor {
// 	friend graphT;
// public:
// 	using ValueType = typename graphT::ValueType;
// 	using TraverseCallback = std::function<void(GraphCursor,unsigned int)>;
// 	//TODO: make the GraphCursor read only, so you can't change it in the function
// 	using TraverseCondition = std::function<bool(GraphCursor, unsigned int)>;
//     public:
// 	void DepthFirstTraverse(TraverseConditiparamson visitCondition, 
// 							TraverseCallback onVisitCallback, 
// 							TraverseCondition proceedCondition)	{
// 		DepthFirstUtil(0, *this, visitCondition, onVisitCallback, proceedCondition);
// 		UnvisitAllGraphNodes();
// 	}
// 	void DepthFirstUtil(unsigned int recursionDepth,
// 						GraphCursor start,
// 						TraverseCondition visitCondition,
// 						TraverseCallback onVisitCallback,
// 						TraverseCondition proceedCondition) {
// 		if (!visitCondition(start, recursionDepth)) return;

// 		// visit node
// 		start.MarkVisited(true);
// 		onVisitCallback(start, recursionDepth);

// 		if (!proceedCondition(start, recursionDepth)) return;

// 		// recurse for each adjacent node
// 		std::list<GraphCursor>* adjl = start.AdjacencyList();
// 		for (GraphCursor next : *adjl)
// 			if (!next.IsVisited())
// 				DepthFirstUtil(recursionDepth + 1, next, visitCondition, onVisitCallback, proceedCondition);

// 		// clean up on the way out.
// 		delete adjl;
// 	}
// }
}

//*

public:
	/// <summary>
	/// Traverse the Doodle graph in a depth first pattern starting at the node under this cursor. A
	/// node is visited no more than once per traversal.
	/// </summary>
	/// <param name="visitCondition">
	/// Only visit a node, if this condition applies. This may be checked more than once for a
	/// given node if it has multiple neighbours. This allows for a partial traversal where 
	/// nodes remain unvisited if they do not fulfill the condition.
	/// </param>
	/// <param name="onVisitCallback">
	/// Called upon visiting a node. Useful for processing data or otherwise working with nodes.
	/// </param>
	/// <param name="proceedCondition">
	/// Whether a node's neighbours ought to be explored. Allows for a partial traversal where 
	/// the algorithm proceeds to visit neighbours of a current node, only when the condition 
	/// applies for that node. The given current node is itself considered visited.
	/// </param>
	void DepthFirstTraverse(TraverseCondition visitCondition, 
							TraverseCallback onVisitCallback, 
							TraverseCondition proceedCondition)	{
		DepthFirstUtil(0, *this, visitCondition, onVisitCallback, proceedCondition);
		UnvisitAllDoodleGraphNodes();
	}
	/// <summary>
	/// Traverse the Doodle graph in a breadth first pattern starting at the node under this cursor. A
	/// node is visited no more than once per traversal.
	/// </summary>
	/// <param name="visitCondition">
	/// Only visit a node, if this condition applies. This may be checked more than once for a
	/// given node if it has multiple neighbours. This allows for a partial traversal where 
	/// nodes remain unvisited if they do not fulfill the condition.
	/// </param>
	/// <param name="onVisitCallback">
	/// Called upon visiting a node. Useful for processing data or otherwise working with nodes.
	/// </param>
	/// <param name="proceedCondition">
	/// Whether a node's neighbours ought to be explored. Allows for a partial traversal where 
	/// the algorithm proceeds to visit neighbours of a current node, only when the condition 
	/// applies for that node. The given current node is itself considered visited.
	/// </param>
	void BreadthFirstTraverse(TraverseCondition visitCondition,
							  TraverseCallback onVisitCallback, 
							  TraverseCondition proceedCondition) {
		std::Nodelist<GraphCursor<graphT>>* firstLayer = new std::Nodelist<GraphCursor<graphT>>();
		std::Nodelist<GraphCursor<graphT>>* nextLayer = new std::Nodelist<GraphCursor<graphT>>();
		GraphCursor<graphT>* startCopy = new GraphCursor<graphT>(*this);
		firstLayer->push_back(*startCopy);
		BreadthFirstUtil(0, firstLayer, nextLayer, visitCondition, onVisitCallback, proceedCondition);
		UnvisitAllGraphNodes();
		delete firstLayer;
		delete nextLayer;
	}

	void DepthFirstTraverse(TraverseCallback onVisitCallback) {
		DepthFirstTraverse(
			[](GraphCursor cursor, unsigned int recursionDepth) { return true; },
			onVisitCallback,
			[](GraphCursor cursor, unsigned int recursionDepth) { return true; }
		);
	}
	void BreadthFirstTraverse(TraverseCallback onVisitCallback)	{
		BreadthFirstTraverse(
			[](GraphCursor cursor, unsigned int recursionDepth) { return true; },
			onVisitCallback,
			[](GraphCursor cursor, unsigned int recursionDepth) {return true; }
		);
	}

	void DepthFirstTraverse(std::function<void(GraphCursor)> onVisitCallback) {
		DepthFirstTraverse(
            [onVisitCallback](GraphCursor cursor, unsigned int recursionDepth) {
                onVisitCallback(cursor);
            }
        );
	}
	void BreadthFirstTraverse(std::function<void(GraphCursor)> onVisit)	{
		BreadthFirstTraverse([onVisit](GraphCursor cursor, unsigned int recursionDepth) { onVisit(cursor); });
	}
	bool IsVisited() {
		GraphNode<ValueType>* gnode = (GraphNode<ValueType>*)(iterator->master);
		return gnode->visited;
	}
	void MarkVisited(bool visited) {
		GraphNode<ValueType>* gnode = (GraphNode<ValueType>*)(iterator->master);
		gnode->visited = visited;
	}
private:
	void DepthFirstUtil(unsigned int recursionDepth,
						GraphCursor start,
						TraverseCondition visitCondition,
						TraverseCallback onVisitCallback,
						TraverseCondition proceedCondition) {
		if (!visitCondition(start, recursionDepth)) return;

		// visit node
		start.MarkVisited(true);
		onVisitCallback(start, recursionDepth);

		if (!proceedCondition(start, recursionDepth)) return;

		// recurse for each adjacent node
		std::Nodelist<GraphCursor>* adjl = start.AdjacencyNodeList();
		for (GraphCursor next : *adjl)
			if (!next.IsVisited())
				DepthFirstUtil(recursionDepth + 1, next, visitCondition, onVisitCallback, proceedCondition);

		// clean up on the way out.
		delete adjl;
	}
	void BreadthFirstUtil(unsigned int recursionDepth,
						  std::Nodelist<GraphCursor>* currentLayer,
						  std::Nodelist<GraphCursor>* nextLayer,
						  TraverseCondition visitCondition,
						  TraverseCallback onVisitCallback,
						  TraverseCondition proceedCondition) {
		// return condition
		if (currentLayer->size() == 0 && nextLayer->size() == 0)
			return;

		// visit the current layer
		for (auto c = currentLayer->begin(); c != currentLayer->end();) {
			// c may be duplicate in the current layer. also check visit condition.
			if (c->IsVisited() || !visitCondition(*c, recursionDepth)) {
				c = currentLayer->erase(c);
				continue;
			}

			// visit node
			c->MarkVisited(true);
			onVisitCallback(*c, recursionDepth);

			if (!proceedCondition(*c, recursionDepth)) {
				c = currentLayer->erase(c);
				continue;
			}

			// add non-visited adj nodes to next layer (allow duplicate nodes)
			std::Nodelist<GraphCursor>* adjl = c->AdjacencyNodeList();
			c = currentLayer->erase(c);
			for (auto adjc = adjl->begin(); adjc != adjl->end();) {
				if (!adjc->IsVisited())
					nextLayer->push_back(*(adjc++));
				else
					adjc = adjl->erase(adjc);
			}
			delete adjl;
		}
		// recurse with swapped Nodelists: currentLayer <---> nextLayer
		BreadthFirstUtil(recursionDepth + 1,
						 nextLayer,
						 currentLayer,
						 visitCondition,
						 onVisitCallback,
						 proceedCondition);
	}
public:
	void UnvisitAllGraphNodes()	{
		NodeListIterator<NodeList<CrownNode>> temp = iterator;
		while (temp.node->prev)	--temp;
		for (; temp.node->data; ++temp)	GraphCursor(temp).MarkVisited(false);
	}
//*/
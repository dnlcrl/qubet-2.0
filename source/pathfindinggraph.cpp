#include "pathfindinggraph.h"

PathfindingGraph::PathfindingGraph(Level *_level, int xPos):
    level(_level)
{
    findCorridors();

    head = new Node(0, xPos, 0, 0);
    head->setCostSoFar(0);
    head->setBeforeNode(NULL);
    nodes.insert( head->getId(),head);

    createGraph(head);
    resetStates();
    findPath(true);
}

PathfindingGraph::~PathfindingGraph()
{
    level       = NULL;
    head        = NULL;
    goal        = NULL;
    tempNode    = NULL;
    corridors.~QVector();
    nodes.~QMap();
    connections.~QVector();
    pathStack.~QStack();

}

Node *PathfindingGraph::getNode(unsigned int _id)
{
    return nodes[_id];
}

void PathfindingGraph::findCorridors()
{
    QVector<QVector<QVector<bool> > >   obstacleCells = level->getObstacleCells();
    Corridor *corridor = NULL;

    for (int i = 0; i < level->getWidth() / 3; i++)
    {
        for (int k = 0; k < level->getLength() / 3; k++)
        {
            if (obstacleCells[i][0][k] == false && corridor == NULL)
            {
                corridor = new Corridor();
                corridor->setXPosition(i);
                corridor->setZBegin(k);
            }
            else if (obstacleCells[i][0][k] == true && corridor)
            {
                corridor->setZEnd(k-1);
                corridors.append(corridor);
                corridor = NULL;
            }
        }
        if (corridor)
        {
            corridor->setZEnd(level->getLength() / 3 - 1);
            corridors.append(corridor);
            corridor = NULL;
        }
    }
}

void PathfindingGraph::createGraph(Node *_node)
{
    if (_node->getZPosition() >= (int)(level->getLength() / 3) -1 || _node->getState() & NODE_STATE_CHECKED)
        return;

    for (int i = 0; i < corridors.count(); i++)
    {
        if (corridors.at(i)->getXPosition()  == _node->getXPosition() &&
                corridors.at(i)->getZBegin()<= _node->getZPosition() &&
                (corridors.at(i)->getZEnd()  > _node->getZPosition() + 3 ||
                 corridors.at(i)->getZEnd() == (int)(level->getLength() / 3) -1))
        {
            getNodeID(_node->getXPosition(), 0, _node->getZPosition() + 1);
            _node->setState(NODE_STATE_FRONT);
            appendConnectionIfNotExist(1.0f, _node, tempNode);
            createGraph(tempNode);
        }
        if (_node->getXPosition() != 0 &&
                corridors.at(i)->getXPosition()  == _node->getXPosition() -1 &&
                corridors.at(i)->getZBegin() < _node->getZPosition() &&
                corridors.at(i)->getZEnd()   > _node->getZPosition() + 5)
        {
            getNodeID(_node->getXPosition() -1, 0, _node->getZPosition() + 3);
            _node->setState(NODE_STATE_LEFT);
            appendConnectionIfNotExist(3.2f, _node, tempNode);
            createGraph(tempNode);
        }
        if (_node->getXPosition() != (int)(level->getWidth() / 3) -1 &&
                corridors.at(i)->getXPosition()  == _node->getXPosition() +1 &&
                corridors.at(i)->getZBegin()< _node->getZPosition() &&
                corridors.at(i)->getZEnd()  > _node->getZPosition() + 5)
        {
            getNodeID(_node->getXPosition() +1, 0, _node->getZPosition() + 3);
            _node->setState(NODE_STATE_RIGHT);
            appendConnectionIfNotExist(3.2f, _node, tempNode);
            createGraph(tempNode);
        }
    }
}

void PathfindingGraph::getNodeID(int _xPosition, int _yPosition, int _zPosition)
{
    for (QMap<unsigned int,Node*>::iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        if (dynamic_cast<Node*>(i.value())->getXPosition() == _xPosition )
                if (dynamic_cast<Node*>(i.value())->getYPosition() == _yPosition)
                if (dynamic_cast<Node*>(i.value())->getZPosition() == _zPosition)
        {
            tempNode = dynamic_cast<Node*>(i.value());
            return;
        }
    }
    tempNode = new Node(nodes.count(), _xPosition, _yPosition, _zPosition);
    nodes.insert(tempNode->getId(), tempNode);
}

void PathfindingGraph::appendConnectionIfNotExist(float _cost, Node *_fromNode, Node*_toNode)
{
    for (int i= 0; i < connections.count(); i++)
        if (connections.at(i)->getFromNode() == _fromNode && connections.at(i)->getFromNode() == _toNode)
            return;
    connections.append(new Connection(_cost, _fromNode, _toNode));
}

void PathfindingGraph::findPath(bool aStarAlgorithm)
{
    double_t minimumNextNodeDistance;

    gettimeofday(&timerStart, NULL);
    QFile file("stats");

    Node *iterator = nodes[0];
    Node *iterator2 = nodes[0];

    bool goalFound = false;
    bool nodeFound = false;
    goal = NULL;

    if (!aStarAlgorithm)
    {
        gettimeofday(&timerStart, NULL);
        for (QMap<unsigned int,Node*>::iterator j = nodes.begin(); j != nodes.end(); j++)
        {
            for (int i = 0; i < connections.count(); i++)
            {
                if (connections.at(i)->getFromNode() == dynamic_cast<Node*>(j.value()))
                {
                    if (connections.at(i)->getFromNode()->getId() == 0)
                    {
                        connections.at(i)->getToNode()->setBeforeNode(connections.at(i)->getFromNode());
                        connections.at(i)->getToNode()->setCostSoFar(connections.at(i)->getCost());
                    }
                    else if (connections.at(i)->getFromNode()->getCostSoFar() + connections.at(i)->getCost() < connections.at(i)->getToNode()->getCostSoFar())
                    {
                        connections.at(i)->getToNode()->setCostSoFar(connections.at(i)->getFromNode()->getCostSoFar() + connections.at(i)->getCost());
                        connections.at(i)->getToNode()->setBeforeNode(connections.at(i)->getFromNode());
                    }
                    if (connections.at(i)->getToNode()->getZPosition() > ((int)(level->getLength() / 3.0f) -3) && connections.at(i)->getToNode()->getXPosition() == head->getXPosition())
                    {
                        goal = connections.at(i)->getToNode();
                    }
                }
            }
        }
        if (!goal)
        {
            qDebug()<<"Warning! algorithm bug!!! goal not found!";
        }
        createPathStack();
        gettimeofday(&timerStop, NULL);
        elapsedTime += (timerStop.tv_usec - timerStart.tv_usec);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
            QTextStream in(&file);
            in<<QString::number(elapsedTime)<<"\t";
            file.close();
        }
    }
    else
    {
        while (!goalFound)
        {
            nodeFound = false;
            minimumNextNodeDistance = INFINITY;
            for (int i = 0; i < connections.count() && goalFound == false; i++)
            {
                if (connections.at(i)->getFromNode() == iterator && !(connections.at(i)->getToNode()->getState() & NODE_STATE_CHECKED))
                {
                    if(connections.at(i)->getToNode()->getDistance(head->getXPosition(), ((int)(level->getLength() / 3.0f) -1)) +
                            connections.at(i)->getCost() < minimumNextNodeDistance)
                    {
                        minimumNextNodeDistance = connections.at(i)->getToNode()->getDistance(head->getXPosition(), ((int)(level->getLength() / 3.0f) -1)) + connections.at(i)->getCost();
                        connections.at(i)->getToNode()->setBeforeNode(iterator, connections.at(i)->getCost());
                        iterator2 = connections.at(i)->getToNode();
                        nodeFound = true;
                    }
                    if(QString::number(connections.at(i)->getToNode()->getDistance(head->getXPosition(), ((int)(level->getLength() / 3.0f) -1))) == "0" ||
                            iterator2->getZPosition() >= (level->getLength() / 3.0f )-2 )
                    {
                        goal = connections.at(i)->getToNode();
                        goalFound = true;
                        createPathStack();
                    }

                }

            }
            iterator->setState(NODE_STATE_CHECKED);
            if(nodeFound)
            {
                iterator = iterator2;
            }
            else
            {
                iterator = iterator->getBeforeNode();
            }

        }
        gettimeofday(&timerStop, NULL);
        elapsedTime += (timerStop.tv_usec - timerStart.tv_usec);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
            QTextStream in(&file);
            in<<QString::number(elapsedTime)<<"\n";
            file.close();
        }
    }
}

void PathfindingGraph::createPathStack()
{
    tempNode = goal;
    while(tempNode != head)
    {
        pathStack.push(tempNode);
        tempNode = tempNode->getBeforeNode();
    }
}

Node *PathfindingGraph::pop()
{
    return pathStack.pop();
}

bool PathfindingGraph::isTheGoal(Node *node)
{
    return node->getId() == goal->getId();
}

void PathfindingGraph::resetStates()
{
    for(QMap<unsigned int,Node*>::iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        if (i.value() != NULL)
            dynamic_cast<Node*>(i.value())->unSetState(NODE_STATE_CHECKED);
    }
}

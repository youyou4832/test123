#include "stdafx.h"
#include "AstarAlgorithm.h"


HRESULT AstarAlgorithm::init()
{
	m_index = 1;
	m_startNode.id = m_startNode.x + (m_startNode.y*MAP_SIZE);
	m_startNode.F_Score = 0;
	m_startNode.G_Score = 0;
	m_startNode.H_Score = 0;
	m_startNode.parent = NULL;
	setDirectNull(&m_startNode);
	
	pushCloseList(m_startNode);
	copyNode(m_startNode);
	m_iter = m_closeList.begin();
	m_parent = &(*m_iter);
	m_destNode.id = m_destNode.x + (m_destNode.y*MAP_SIZE);
	
	return S_OK;
}

void AstarAlgorithm::release()
{
	m_closeList.clear();
	m_openMap.clear();
	m_storeWay.clear();
}

void AstarAlgorithm::render(HDC hdc)
{

}

void AstarAlgorithm::update()
{
	while (m_currNode.id != m_destNode.id) {
		calChildNode();
		m_currNode = CompareOpenList();
	}
	calculateWay();
}

void AstarAlgorithm::update2(int move)
{
	while (m_currNode.id != m_destNode.id) {
		calChildNode();
		m_currNode = CompareOpenList();
		if (m_closeList.size() > move) {
			break;
		}
	}
	calculateWay();
}

void AstarAlgorithm::calChildNode()//탐색할 노드 구하기
{
	Node leftChild;
	Node topChild;
	Node rightChild;
	Node bottomChild;
	leftChild.id = m_currNode.id - 1;
	topChild.id = m_currNode.id - MAP_SIZE;
	rightChild.id = m_currNode.id + 1;
	bottomChild.id = m_currNode.id + MAP_SIZE;
	
	if (m_currNode.x == 0||m_basicTile[leftChild.id].terrainInfo == TERRAIN::o_fence) {
		m_currNode.direct[0] = NULL;//왼쪽
	}
	else {
		if (!checkCLoseList(leftChild.id)) {
			leftChild.x = m_currNode.x - 1;
			leftChild.y = m_currNode.y;
			leftChild.parent = m_parent;
			leftChild.H_Score = calH_Score(leftChild);
			leftChild.G_Score = calG_Score(leftChild);
			leftChild.F_Score = calF_Score(leftChild);
			setDirectNull(&leftChild);
			if (!checkOpenMap(leftChild)) {
				pushOpenMap(leftChild);
			}
		}
	}

	if (m_currNode.y == 0||m_basicTile[topChild.id].terrainInfo == TERRAIN::o_fence) {//위쪽
		m_currNode.direct[1] = NULL;
	}
	else {
		
		if (!checkCLoseList(topChild.id)) {
			topChild.x = m_currNode.x;
			topChild.y = m_currNode.y-1;
			topChild.parent = m_parent;
			topChild.H_Score = calH_Score(topChild);
			topChild.G_Score = calG_Score(topChild);
			topChild.F_Score = calF_Score(topChild);
			setDirectNull(&topChild);
			if (!checkOpenMap(topChild)) {
				pushOpenMap(topChild);
			}
		}
	}

	if (m_currNode.x == MAP_SIZE || m_basicTile[rightChild.id].terrainInfo == TERRAIN::o_fence) {//오른쪽
		m_currNode.direct[2] = NULL;
	}
	else {
		if (!checkCLoseList(rightChild.id)) {
			rightChild.x = m_currNode.x + 1;
			rightChild.y = m_currNode.y;
			rightChild.parent = m_parent;
			rightChild.H_Score = calH_Score(rightChild);
			rightChild.G_Score = calG_Score(rightChild);
			rightChild.F_Score = calF_Score(rightChild);
			setDirectNull(&rightChild);
			if (!checkOpenMap(rightChild)) {
				pushOpenMap(rightChild);
			}
		}
	}

	if (m_currNode.y == MAP_SIZE || m_basicTile[bottomChild.id].terrainInfo == TERRAIN::o_fence) {//아래
		m_currNode.direct[3] = NULL;
	}
	else {
		if (!checkCLoseList(bottomChild.id)) {
			bottomChild.x = m_currNode.x;
			bottomChild.y = m_currNode.y + 1;
			bottomChild.parent = m_parent;
			bottomChild.H_Score = calH_Score(bottomChild);
			bottomChild.G_Score = calG_Score(bottomChild);
			bottomChild.F_Score = calF_Score(bottomChild);
			setDirectNull(&bottomChild);
			if (!checkOpenMap(bottomChild)) {
				pushOpenMap(bottomChild);
			}
		}
	}
}

Node AstarAlgorithm::CompareOpenList()
{
	////////////////////////////////맵으로 바꾼 후 코드/////////////
	int parent, child;
	Node root, temp;

	root = m_openMap.find(1)->second;
	temp = m_openMap.find(m_openMap.size())->second;
	m_openMap.erase(m_openMap.size());
	m_index--;
	parent = 1;
	child = 2;
	while (child <= m_openMap.size()) {
		if ((child < m_openMap.size()) && ((m_openMap.find(child)->second.F_Score) > m_openMap.find(child + 1)->second.G_Score)) {
			child++;
		}
		if (temp.F_Score <= m_openMap.find(child)->second.F_Score) {
			break;
		}
		m_openMap.find(parent)->second = m_openMap.find(child)->second;
		parent = child;
		child *= 2;
	}
	m_openMap.find(parent)->second = temp;
	pushCloseList(root);
	m_iter = m_closeList.end();
	m_iter--;
	m_parent = &(*m_iter);
	return root;



	///////////////////////////////////힙소트 하기 전 코드////////////
	/*m_iter = m_openList.begin();
	Node temp = (*m_iter);
	m_iter++;
	for (; m_iter != m_openList.end(); m_iter++) {
		if (temp.F_Score > (*m_iter).F_Score) {
			temp = (*m_iter);
		}
	}
	pushCloseList(temp);
	eraseOpenList(temp);
	m_iter = m_closeList.end();
	m_iter--;
	m_parent = &(*m_iter);*/



	///////////////////////////////////////////부모노드 설정 수정 전 코드///////////////////////
	/*for (m_iter = m_closeList.begin(); m_iter != m_closeList.end(); m_iter++) {
		if (temp.id == m_iter->id) {
			m_parent = &(*m_iter);
		}
	}*/
	//m_currNode.parent = m_parent;


	//return temp;
}

void AstarAlgorithm::calculateWay()
{
	Node temp;
	m_iter = m_closeList.end();
	m_iter--;
	m_storeWay.push_back((*m_iter).id);
	if ((m_iter->parent) != NULL) {
		temp = *(m_iter->parent);
	}
	else {
		return;
	}
	while (temp.id != m_startNode.id) {
		m_storeWay.push_back(temp.id);
		temp = *(temp.parent);
	}
	//int a = 1;
}

int AstarAlgorithm::calG_Score(Node node)
{
	if (node.parent == NULL) {
		return SCORE_VAL;
	}
	else {
		return node.parent->G_Score + 1*SCORE_VAL;
	}
}

float AstarAlgorithm::calF_Score(Node node)
{
	return (node.G_Score + node.H_Score);
}

float AstarAlgorithm::calH_Score(Node node)
{
	/*int calH;
	calH = sqrt((m_destNode.x - node.x)*(m_destNode.x - node.x)) + sqrt((m_destNode.y - node.y)*(m_destNode.y - node.y));
	return calH * SCORE_VAL;*/
	//float a = MY_UTIL::getDistance(node.x, node.y, m_destNode.x, m_destNode.y);
	int x;
	int y;
	if (m_destNode.x > node.x) {
		x = m_destNode.x - node.x;
	}
	else {
		x = node.x - m_destNode.x;
	}
	if (m_destNode.y > node.y) {
		y = m_destNode.y - node.y;
	}
	else {
		y = node.y - m_destNode.y;
	}
	return (x+y);
}

void AstarAlgorithm::setDirectNull(Node* node)
{
	for (int i = 0; i < 4; i++) {
		node->direct[i] = NULL;
	}
}

//void AstarAlgorithm::eraseOpenList(Node node)
//{
//	for (m_iter = m_openList.begin(); m_iter != m_openList.end(); m_iter++) {
//		if ((*m_iter).id == node.id) {
//			m_openList.erase(m_iter);
//			break;
//		}
//	}
//}

void AstarAlgorithm::pushOpenMap(Node node)
{
	int i;
	m_openMap.insert(make_pair(m_index, node));
	i = m_openMap.size();
	
	while ((i != 1) && (node.F_Score < m_openMap.find(i / 2)->second.F_Score)) {
		m_openMap.find(i)->second = m_openMap.find(i / 2)->second;
		i /= 2;
	}
	
	m_openMap.find(i)->second = node;
	m_index++;
}

void AstarAlgorithm::searchPostion(Node node, int index)
{
	int i = index;
	while ((i != 1) && (node.F_Score < m_openMap.find(i / 2)->second.F_Score)) {
		m_openMap.find(i)->second = m_openMap.find(i / 2)->second;
		//m_openMap.find(i)->second.F_Score = m_openMap.find(i / 2)->second.F_Score;
		i /= 2;
	}
	//m_openMap.find(i)->second.F_Score = node.F_Score;
	m_openMap.find(i)->second = node;
}

//void AstarAlgorithm::openListHeapSort(Node node)
//{
//	int i;
//	float parent;
//	i = m_openList.size()+1;
//	
//	while ((i != 1) && calIter(i / 2 - 1) &&(node.F_Score < (*m_iter).F_Score)) {
//		calIter(i/2-1);
//		parent = (*m_iter).F_Score;
//		calIter(i-1);
//		(*m_iter).F_Score = parent;
//	}
//	calIter(i-1);
//	(*m_iter).F_Score = node.F_Score;
//}

//bool AstarAlgorithm::calIter(int num)
//{
//	m_iter = m_openList.begin();
//	for (int i = 0; i < num; i++) {
//		m_iter++;
//	}
//	return true;
//}

//void AstarAlgorithm::checkOpenList(Node node)
//{
//	for (m_iter = m_openList.begin(); m_iter != m_openList.end(); m_iter++) {
//		if ((*m_iter).id == node.id) {
//			if ((*m_iter).F_Score > node.F_Score) {
//				m_openList.erase(m_iter);
//				//openListHeapSort(node);
//				m_openList.push_back(node);
//				return;
//			}
//		}
//	}
//	m_openList.push_back(node);
//	//openListHeapSort(node);
//}

bool AstarAlgorithm::checkOpenMap(Node node)
{
	for (m_mapIter = m_openMap.begin(); m_mapIter != m_openMap.end(); m_mapIter++) {
		if ((*m_mapIter).second.id == node.id) {
			if ((*m_mapIter).second.F_Score > node.F_Score) {
				(*m_mapIter).second = node;
				searchPostion((*m_mapIter).second, (*m_mapIter).first);
			}
			return true;
		}
	}
	return false;
}

bool AstarAlgorithm::checkCLoseList(int id)
{
	for (m_iter = m_closeList.begin(); m_iter != m_closeList.end(); m_iter++) {
		if ((*m_iter).id == id) {
			return true;
		}
	}
	return false;
}

AstarAlgorithm::AstarAlgorithm()
{
}


AstarAlgorithm::~AstarAlgorithm()
{
}

#pragma once

#define SCORE_VAL 10

struct Node {
	int x;
	int y;
	int id;//��� id , Ÿ���� index��ȣ
	float F_Score; // G+H
	int G_Score; // ���۳����� �Ÿ� (�θ����� G Score + �θ������ �Ÿ�)
	float H_Score; // ������������ �Ÿ�
	Node* parent; // �������(�θ���)
	Node* direct[4]; // Ž���� 4������ ����
};

class AstarAlgorithm
{
private:
	map<int, Node> m_openMap;
	map<int, Node>::iterator m_mapIter;
	//list<Node> m_openList;
	list<Node> m_closeList;
	list<Node>::iterator m_iter;
	vector<int> m_storeWay;
	int m_index;

	tagBasicTile* m_basicTile;
	Node m_startNode;
	Node m_currNode;
	Node m_destNode;
	Node* m_parent;

public:
	HRESULT init();
	void release();
	void render(HDC hdc);
	void update();
	void update2(int move);

	void calChildNode();
	Node CompareOpenList();
	void calculateWay();

	int calG_Score(Node node);
	float calF_Score(Node node);
	float calH_Score(Node node);
	void setDirectNull(Node* direct);

	//void eraseOpenList(Node node);
	//inline void pushOpenList(Node node) { m_openList.push_back(node); }
	void pushOpenMap(Node node);
	void searchPostion(Node node, int index);
	inline void pushCloseList(Node node) { m_closeList.push_back(node); }
	inline void setTileInfo(tagBasicTile* tile) { m_basicTile = tile; }
	inline void setStartNode(int startX, int startY) { m_startNode.x = startX, m_startNode.y = startY; }
	inline void setDestNode(int destX, int destY) { m_destNode.x = destX, m_destNode.y = destY; }
	inline void copyNode(Node node) { m_currNode = node; }

	//void openListHeapSort(Node node);
	//bool calIter(int num);

	//void checkOpenList(Node node);
	bool checkOpenMap(Node node);
	bool checkCLoseList(int id);

	inline vector<int> getStoreWay() { return m_storeWay; }
	


	AstarAlgorithm();
	~AstarAlgorithm();
};


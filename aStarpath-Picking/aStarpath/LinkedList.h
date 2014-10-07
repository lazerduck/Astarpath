///--------------Overview--------------///
/*
This creates a linked list of any type.
items can be removed at any point using
RemoveAt, elements can be added at the
end using Add or at a specific point
using AddAt. elemets can be returned using
Return or []

This has no leaks (from what I've tested)
but can still cause a stack overflow
if it is too large.

[in progress]

Made by Adam Bowes

*/

template<class T>
class LinkedList
{
private:
	//node class
	class Node
	{
	public:
		T Value;
		Node* NextNode;
		Node(T value, Node* nextnode){ Value = value;	NextNode = nextnode;}
	};
	//pointer to the first node
	Node* first;
	void AddNode(T Value, Node& CurrNode);
	T ReturnTravel(Node& CurrNode, int Position);
	void DeleteTravel(Node& CurrNode);
	void RemoveTravel(Node& PrevNode, Node& CurrNode, int Position);
	void AddTravel(Node& CurrNode, T Value, int Position);
	int LengthTravel(Node& CurrNode);
	bool ContainsTravel(T& test, Node& CurrNode);
	T ContainsTravel(T& test, Node& CurrNode, bool Ret);
	int ContainsAtTravel(T& test, Node& CurrNode, int Ret);
public:
	LinkedList();
	void Add(T Value);
	T Return(int Position);
	void RemoveAt(int Position);
	void AddAt(int Position, T Value);
	int Length();
	bool Contains(T& test);
	int ContainsAt(T& test);
	T Contains(T& test, bool ret);
	~LinkedList();
	T operator[] (int);
};
template<class T>
LinkedList<T>::LinkedList()
{
	//empty linked list
	first = 0;
}
template<class T>
void LinkedList<T>::Add(T Value)
{
	//if first is unassigned
	if(first == 0)
	{
		first = new Node(Value,0);
	}
	else
	{
		//add a node to the end
		AddNode(Value, *first);
	}
}
template<class T>
void LinkedList<T>::AddNode(T Value, Node& CurrNode)
{
	if(CurrNode.NextNode == 0)
	{
		CurrNode.NextNode = new Node(Value,0);
	}
	else
	{
		AddNode(Value, *CurrNode.NextNode);
	}
}
template<class T>
T LinkedList<T>::Return(int Position)
{
	return ReturnTravel(*first, Position);
}
template<class T>
T LinkedList<T>::ReturnTravel(Node& CurrNode, int Position)
{
	if(Position == 0)
	{
		 return CurrNode.Value;
	}
	else if(CurrNode.NextNode == 0)
	{
		try
		{
			throw 20;
		}
		catch(int e)
		{
			cout<<"error code "<<e<<" in return travel"<<endl;
		}
		return CurrNode.Value;
	}
	else
	{
		return ReturnTravel(*CurrNode.NextNode, Position -1);
	}
}
template<class T>
void LinkedList<T>::RemoveAt(int Position)
{
	if(Position == 0)
	{
		if(first->NextNode == 0)
		{
			Node* Storage = first;
			first=0;
			delete Storage;
		}
		else
		{
		Node* Storage = first;
		first=Storage->NextNode;
		delete Storage;
		}
	}
	else
	{
	RemoveTravel(*first,*first->NextNode,Position - 1);
	}
}
template<class T>
void LinkedList<T>::RemoveTravel(Node& PrevNode, Node& CurrNode, int Position)
{
	if(Position <= 0)
	{
		PrevNode.NextNode = CurrNode.NextNode;
		delete &CurrNode;
	}
	else
	{
		RemoveTravel(CurrNode, *CurrNode.NextNode, Position -1);
	}
}
template<class T>
void LinkedList<T>::AddAt(int Position, T Value)
{
	AddTravel(*first, Value, Position-1);
}
template<class T>
void LinkedList<T>::AddTravel(Node& CurrNode, T Value, int Position)
{
	if(Position > 0 && CurrNode.NextNode != 0)
	{
		AddTravel(*CurrNode.NextNode, Value, Position-1);
	}
	else
	{
		Node* NewNode = new Node(Value,CurrNode.NextNode);
		CurrNode.NextNode = NewNode;

	}
}
template<class T>
int LinkedList<T>::Length()
{
	if(first == 0)
	{
		return 0;
	}
	return LengthTravel(*first);
}
template<class T>
int LinkedList<T>::LengthTravel(Node& CurrNode)
{
	if(CurrNode.NextNode == 0)
	{
		return 1;
	}
	else
	{
		return 1 + LengthTravel(*CurrNode.NextNode);
	}
}
template<class T>
bool LinkedList<T>::Contains(T& test)
{
	if(first == 0)
	{
		return false;
	}
	return ContainsTravel(test, *first);
}
template<class T>
bool LinkedList<T>::ContainsTravel(T& test, Node& CurrNode)
{
	if(CurrNode.Value == test)
	{
		return true;
	}
	if(CurrNode.NextNode == 0)
	{
		return false;
	}
	else
	{
		return ContainsTravel(test, *CurrNode.NextNode);
	}
}
template<class T>
int LinkedList<T>::ContainsAt(T& test)
{
	if(first == 0)
	{
		return -1;
	}
	return ContainsAtTravel(test, *first,0);
}
template<class T>
int LinkedList<T>::ContainsAtTravel(T& test, Node& CurrNode,int tet)
{
	if(CurrNode.Value == test)
	{
		return 0;
	}
	if(CurrNode.NextNode == 0)
	{
		return -1;
	}
	else
	{
		return 1+ ContainsAtTravel(test, *CurrNode.NextNode,0);
	}
}
template<class T>
T LinkedList<T>::Contains(T& test,bool ret)
{
	return ContainsTravel(test, *first,true);
}
template<class T>
T LinkedList<T>::ContainsTravel(T& test, Node& CurrNode, bool ret)
{
	if(CurrNode.Value == test)
	{
		return CurrNode.Value;
	}
	if(CurrNode.NextNode == 0)
	{
		return CurrNode.Value;
	}
	else
	{
		return ContainsTravel(test, *CurrNode.NextNode,true);
	}
}
template<class T>
LinkedList<T>::~LinkedList()
{
	if(first->NextNode != 0)
	{
		DeleteTravel(*first);
	}
	delete first;
}
template<class T>
void LinkedList<T>::DeleteTravel(Node& CurrNode)
{
	if(CurrNode.NextNode != 0)
	{
		if(CurrNode.NextNode->NextNode == 0)
		{
		}
		Node* delNode = CurrNode.NextNode;
		DeleteTravel(*delNode);
		delete CurrNode.NextNode;
		CurrNode.NextNode = 0;
	}
}
template<class T>
T LinkedList<T>::operator[] (int pos)
{
	return Return(pos);
}
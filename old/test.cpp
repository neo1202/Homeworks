template<typename ItemType>
class Node{
	private:
        ItemType item; 
		Node<ItemType>* next;
		Node<ItemType>* back;
	public:
        Node();
        Node(const ItemType& anItem);
        Node(const ItemType& anItem, Node<ItemType>* nextNodePtr);
        void setItem(const ItemType& anItem);
        void setNext(Node<ItemType>* nextNodePtr);
		void setBack(Node<ItemType>* backNodePtr);
        ItemType getItem() const ;
        Node<ItemType>* getNext() const ;
		Node<ItemType>* getBack() const ;
 };

template<class ItemType> Node<ItemType>::Node() : next(nullptr) ,back(nullptr) {
} // default constructor
template<class ItemType>
Node<ItemType>::Node(const ItemType& anItem) : item(anItem), next(nullptr),back(nullptr){
} //initial Item constructor
template<class ItemType>
Node<ItemType>::Node(const ItemType& anItem, Node<ItemType>* nextNodePtr)
:item(anItem), next(nextNodePtr) }

template<class ItemType>
void Node<ItemType>::setItem(const ItemType& anItem) {
item = anItem;
} 
template<class ItemType>
void Node<ItemType>::setNext(Node<ItemType>* nextNodePtr) {
next = nextNodePtr;
} 

template<class ItemType>
ItemType Node<ItemType>::getItem() const {
    return item;
} // end getItem
template<class ItemType>
Node<ItemType>* Node<ItemType>::getNext() const {
    return next;
} // end getNext

template<typename ItemType>
class LinkedBag : public BagInterface<ItemType>{
	private:
		Node<ItemType>* headPtr;
		int itemCount;
     public:
		LinkedBag();
		LinkedBag(const LinkedBag<ItemType>& aBag);
		int getCurrentSize() const;
		bool isEmpty() const;
};
template<class ItemType>
LinkedBag<ItemType>::LinkedBag() : headPtr(nullptr),itemCount(0) {
}
template<class ItemType>
bool LinkedBag<ItemType>::add(const ItemType& newEntry) {
Node<ItemType>* newNodePtr = new Node<ItemType>(); newNodePtr->setItem(newEntry);
newNodePtr->setNext(headPtr); 
newNodePtr->setBack(nullptr);
    return true;
}
#include<string>
//第一題
bool checkBalanceBraces(string aString){
	bool isBalance = true
	aStack = new stack
	int i = 0
	while( isBalance and i< strlen){
		ch = aString[i]
		if(ch == '(' or ch=='[' or ch =='{'){
			aStack.push(ch)
		}
		else if(ch ==')'){
			if(!isEmpty and aStack.peek() is '('){
				aStack.pop()
			}
			else{
				isBalance = false
			}
		}
		else if(ch ==']'){
			if(!isEmpty and aStack.peek() is '['){
				aStack.pop()
			}
			else{
				isBalance = false
			}
		}
		else if(ch =='}'){
			if(!isEmpty and aStack.peek() is '{'){
				aStack.pop()
			}
			else{
				isBalance = false
			}
		}
		i++
	}
	if(isBalance and aStack.isEmpty()){
		return true; //valid
	}
	else{
		return false; //not valid
	}
}




#include <iostream>
using namespace std;
//=========================================================================================================================
//======================================================泛型================================================================
//=========================================================================================================================
template <typename T>
void addNode(T* curr, T* newNode) {
	//如果调用addNode的那个对象（不一定是头节点）所在节点的下一个是nullptr，直接把新节点接在后面
	if (curr->getNext() == nullptr) {
		curr->setNext(newNode);
		return;
	}
	//否则遍历已有链表到尾部
	T* temp = curr;
	while (temp->getNext() != nullptr) {
		temp = temp->getNext();
	}
	//尾插新节点
	temp->setNext(newNode);
	return;
}
template <typename T>
void deleteNode(T* curr, T* head) {
	T* temp = head;
	while (temp != nullptr && temp->getNext() != nullptr) {
		if (temp->getNext() == curr) {
			temp->setNext(curr->getNext());
			delete curr;
			curr = nullptr;
			//虽然说delete this（为了实现泛型，传入curr）有一定风险，
			//但是只要注意以后不要对当前节点进行任何操作即可（也就是调用成员函数）
			return;
		}
		temp = temp->getNext();
	}
	return;
}
template <typename T>
void deleteAll(T* head) {
	T* toDelete;
	//遍历链表删除所有节点（除了头节点）
	while (head->getNext() != nullptr) {
		toDelete = head->getNext();
		head->setNext(toDelete->getNext());
		delete toDelete;
	}
}
template <typename T>
void swapNode(T* a, T* b, T* head) {
	T* temp = head;
	//此循环用于找到a节点的上一个节点
	while (temp->getNext() != a) {
		temp = temp->getNext();
	}
	//让a的上一个节点（temp）指向b
	temp->setNext(b);
	//让a的下一个指向b的下一个
	a->setNext(b->getNext());
	//让b的下一个指向a
	b->setNext(a);
	return;
}
#include <iostream>
using namespace std;
//=========================================================================
//===============================����=======================================
//=========================================================================
template <typename T>
void addNode(T* curr, T* newNode) {
	//�������addNode���Ǹ����󣨲�һ����ͷ�ڵ㣩���ڽڵ����һ����nullptr��ֱ�Ӱ��½ڵ���ں���
	if (curr->getNext() == nullptr) {
		curr->setNext(newNode);
		return;
	}
	//���������������β��
	T* temp = curr;
	while (temp->getNext() != nullptr) {
		temp = temp->getNext();
	}
	//β���½ڵ�
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
			//��Ȼ˵delete this��Ϊ��ʵ�ַ��ͣ�����curr����һ�����գ�����ֻҪע���Ժ�Ҫ�Ե�ǰ�ڵ�����κβ������ɣ�Ҳ���ǵ��ó�Ա������
			return;
		}
		temp = temp->getNext();
	}
	return;
}
template <typename T>
void deleteAll(T* head) {
	T* toDelete;
	while (head->getNext() != nullptr) {
		toDelete = head->getNext();
		head->setNext(toDelete->getNext());
		delete toDelete;
	}
}
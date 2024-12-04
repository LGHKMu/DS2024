#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
using namespace std;
typedef int Rank;; //��
#define DEFAULT_CAPACITY  3 //Ĭ�ϵĳ�ʼ������ʵ��Ӧ���п�����Ϊ����


template <typename T> class Vector { //����ģ����
protected:
	Rank _size; Rank _capacity;  T* _elem; //��ģ��������������
	void copyFrom(T const* A, Rank lo, Rank hi); //������������A[lo, hi)
	void expand(); //�ռ䲻��ʱ����
	void shrink(); //װ�����ӹ�Сʱѹ��
	bool bubble(Rank lo, Rank hi); //ɨ�轻��
	void bubbleSort(Rank lo, Rank hi); //���������㷨
	Rank maxItem(Rank lo, Rank hi); //ѡȡ���Ԫ��
	void selectionSort(Rank lo, Rank hi); //ѡ�������㷨
	void merge(Rank lo, Rank mi, Rank hi); //�鲢�㷨
	void mergeSort(Rank lo, Rank hi); //�鲢�����㷨
	void heapSort(Rank lo, Rank hi); //�������Ժ�����ȫ�ѽ��⣩
	Rank partition(Rank lo, Rank hi); //��㹹���㷨
	void quickSort(Rank lo, Rank hi); //���������㷨
	void shellSort(Rank lo, Rank hi); //ϣ�������㷨
public:
	// ���췽��
	Vector(Rank c = DEFAULT_CAPACITY) //����Ϊc�Ŀ�����
	{
		_elem = new T[_capacity = c]; _size = 0;
	}
	Vector(Rank c, Rank s, T v) //����Ϊc����ģΪs������Ԫ�س�ʼΪv��s<=c
	{
		_elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //�������帴��
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //����
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //�������帴��
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //����
	// ��������
	~Vector() { delete[] _elem; } //�ͷ��ڲ��ռ�
	// ֻ�����ʽӿ�
	Rank size() const { return _size; } //��ģ
	bool empty() const { return !_size; } //�п�
	Rank find(T const& e) const { return find(e, 0, _size); } //���������������
	Rank find(T const& e, Rank lo, Rank hi) const; //���������������
	Rank select(Rank k) { return quickSelect(_elem, _size, k); } //�������������ҵ���k���Ԫ��
	Rank search(T const& e) const //���������������
	{
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	Rank search(T* A, T const& e, Rank lo, Rank hi) const; //���������������
	// ��д���ʽӿ�
	T& operator[] (Rank r); //�����±������������������������ʽ���ø�Ԫ��
	const T& operator[] (Rank r) const; //����������ֵ�����ذ汾
	Vector<T>& operator= (Vector<T> const&); //���ظ�ֵ���������Ա�ֱ�ӿ�¡����
	T remove(Rank r); //ɾ����Ϊr��Ԫ��
	int remove(Rank lo, Rank hi); //ɾ����������[lo, hi)֮�ڵ�Ԫ��
	Rank insert(Rank r, T const& e); //����Ԫ��
	Rank insert(T const& e) { return insert(_size, e); } //Ĭ����ΪĩԪ�ز���
	void sort(Rank lo, Rank hi); //��[lo, hi)����
	void sort() { sort(0, _size); } //��������
	void unsort(Rank lo, Rank hi); //��[lo, hi)����
	void unsort() { unsort(0, _size); } //��������
	int deduplicate(); //����ȥ��
	int uniquify(); //����ȥ��
	void print();
	void reverse();
	// ����
	void traverse(void (*) (T&)); //������ʹ�ú���ָ�룬ֻ����ֲ����޸ģ�
	template <typename VST>
	void traverse(VST&);// ������ʹ�ú������󣬿�ȫ�����޸ģ�
}; //Vector
//���ƵĹ��췽��
template <typename T> void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
	_elem = new T[_capacity = 2 * (hi - lo)]; _size = 0;
	while (lo < hi)
		_elem[_size++] = A[lo++];
}
//���ظ�ֵ���������Ա�ֱ�ӿ�¡����
template <typename T> Vector<T>& Vector<T>:: operator= (Vector<T> const& V) {
	if (_elem) delete[] _elem;
	copyFrom(V._elem, 0, V.size());
	return *this;
}
//����
template <typename T> void Vector<T>::expand() {
	if (_size < _capacity) return;
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
	T* oldElem = _elem; _elem = new T[_capacity <<= 1];
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i];
	delete[] oldElem;
}
// ����
template <typename T> void Vector<T>::shrink() {
	if (_capacity < DEFAULT_CAPACITY << 1)
		return;
	if (_size << 2 > _capacity)
		return;
	T* oldElem = _elem;
	_elem = new T[_capacity >>= 1];
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i];
	delete[] oldElem;
}
// �����±������������������������ʽ���ø�Ԫ��
template <typename T> T& Vector<T>::operator[] (Rank r) {
	return _elem[r];
}
// ��������
template <typename T> void permute(Vector<T>& V) {
	for (int i = V.size(); i > 0; i--)
		swap(V[i - 1], V[rand() % i]);
}
//��[lo, hi)����
template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) {
	T* V = _elem + lo;
	for (Rank i = hi - lo; i > 0; i--)
		swap(V[i - 1], V[rand() % i]);
}
//���������������
template <typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const {
	while ((lo < hi--) && (e != _elem[hi]));
	return hi;
}
//����Ԫ��
template <typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
	expand();
	for (int i = _size; i > r; i--)_elem[i] = _elem[i - 1];
	_elem[r] = e; _size++;
	return r;
}
// ����ɾ��
template <typename T> int Vector<T>::remove(Rank lo, Rank hi) {
	if (lo == hi)
		return 0;
	while (hi < _size)
		_elem[lo++] = _elem[hi++];
	_size = lo;
	shrink();
	return hi - lo;
}
// ��Ԫ������ɾ��
template <typename T> T Vector<T>::remove(Rank r) {
	T e = _elem[r];
	remove(r, r + 1);
	return e;
}
//����ȥ��
template <typename T> int Vector<T>::deduplicate() {
	int oldSize = _size;
	Rank i = 1;
	while (i < _size)
	{
		find(_elem[i], 0, i) < 0 ? i++ : remove(i);
	}
	return oldSize - _size;
}
// ����
template <typename T> void Vector<T>::traverse(void(*vist)(T&)) {
	for (int i = 0; i < _size; i++) visit(_elem[i]);
}
template <typename T> template <typename VST>
void Vector<T>::traverse(VST& visit) {
	for (int i = 0; i < _size; i++)visit(_elem[i]);
}
//���ڱ���ʵ��increase()����
template <typename T> struct Increase {
	void operator()(T& e)
	{
		e++;
	}
};
template <typename T> void increase(Vector<T>& V) {
	V.traverse(Increase<T>());
}
//Ψһ���������Ч�棩
template <typename T> int Vector<T>::uniquify() {
	Rank i = 0, j = 0;
	while (++j < _size)
	{
		if (_elem[i] != _elem[j]) _elem[++i] = _elem[j];
	}
	_size = ++i;
	shrink();
	return j - i;
}
//ɨ�轻��
template <typename T> bool Vector<T>::bubble(Rank lo, Rank hi) {
	bool sorted = true;
	while (++lo < hi)
		if (_elem[lo - 1] > _elem[lo]) {
			sorted = false;
			swap(_elem[lo - 1], _elem[lo]);
		}
	return sorted;
}
//����
#include "Fib.h"
#include <iostream>
template <typename T> static Rank search(T* A, T const& e, Rank lo, Rank hi) {
	Fib fib(hi - lo);
	while (lo < hi)
	{
		while (hi - lo < fib.get())
			fib.prev();
		Rank mi = lo + fib.get() - 1;
		if (e < A[mi])
			hi = mi;
		else if (e > A[mi])
			lo = mi + 1;
		else
			return mi;
	}
	return -1;
}
//���������㷨
template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
	while (!bubble(lo, hi--));
}
//�鲢�����㷨
template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi) {
	if (hi - lo < 2) return;
	int mi = (lo + hi) / 2;
	mergeSort(lo, hi); mergeSort(mi, hi);
	merge(lo, mi, hi);
}
//�鲢�㷨
template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) {
	T* A = _elem + lo;
	int lb = mi - lo;
	T* B = new T[lb];
	for (Rank i = 0; i < lb; B[i] = A[i++]);
	int lc = hi - mi;
	T* C = _elem + mi;
	for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)
	{
		if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
		if ((k < lc) && (!(j < lb) || (C[k] < C[j]))) A[i++] = C[k++];
	}
}
template <typename T> void Vector<T>::sort(Rank lo, Rank hi) {
	switch (rand() % 2) {
	case 1: bubbleSort(lo, hi); break; //��������
		//case 2: selectionSort(lo, hi); break; //ѡ������ϰ�⣩
	default: mergeSort(lo, hi); break; //�鲢����
		//case 4: heapSort(lo, hi); break; //�����򣨵�12�£�
		//case 5: quickSort(lo, hi); break; //�������򣨵�14�£�
		//default: shellSort(lo, hi); break; //ϣ�����򣨵�14�£�
	}
}
template <typename T> void Vector<T>::reverse() {
	size_t left = 0;
	size_t right = _size - 1;
	while (left < right) {
		std::swap(_elem[left], _elem[right]);
		++left;
		--right;
	}
}
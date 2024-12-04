
#include<iostream>
typedef int Rank;

template <typename T> struct ListNode;
template <typename T> using ListNodePosi = ListNode<T>*; //列表节点位置
template <typename T> struct ListNode { //列表节点模板类（以双向链表形式实现）
// 成员
   T data; ListNodePosi<T> pred, succ; //数值、前驱、后继
// 构造方法
   ListNode() {} //针对head和tail的构造
   ListNode ( T const& e, ListNodePosi<T> p = NULL, ListNodePosi<T> s = NULL )
      : data( e ), pred( p ), succ( s ) {} //默认构造器（类T须定义复制方法）
// 操作接口
   ListNodePosi<T> insertPred( T const& e ); //紧靠当前节点之前插入新节点
   ListNodePosi<T> insertSucc( T const& e ); //紧随当前节点之后插入新节点
};
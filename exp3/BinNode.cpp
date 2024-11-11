#include<iostream>
#include<algorithm>
//int* ptr = nullptr; // 使用 nullptr 代替 NULL
typedef int Rank;//秩
#if defined( DSA_REDBLACK ) //在红黑树中
#define stature( p ) ( ( p ) ? ( p )->height : 0 ) //外部节点（黑）高度为0，以上递推
#else //其余BST中
#define stature( p ) ( (int)( ( p ) ? ( p )->height : -1 ) ) //外部节点高度为-1，以上递推
#endif


typedef enum { RB_RED, RB_BLACK} RBColor; //节点颜色

template <typename T> struct BinNode;
template <typename T> using BinNodePosi = BinNode<T>*; //节点位置

template <typename T> struct BinNode { //二叉树节点模板类
// 成员（为简化描述起见统一开放，读者可根据需要进一步封装）
   T data; //数值
   BinNodePosi<T> parent, lc, rc; //父节点及左、右孩子
   Rank height; //高度（通用）
   Rank npl; //Null Path Length（左式堆，也可直接用height代替）
   RBColor color; //颜色（红黑树）
// 构造方法
   BinNode() : parent( NULL ), lc( NULL ), rc( NULL ), height( 0 ), npl( 1 ), color( RB_RED ) {}
   BinNode( T e, BinNodePosi<T> p = NULL, BinNodePosi<T> lc = NULL,
            BinNodePosi<T> rc = NULL, int h = 0, int l = 1, RBColor c = RB_RED )
      : data( e ), parent( p ), lc( lc ), rc( rc ), height( h ), npl( l ), color( c )
      { if (lc) lc->parent = this; if (rc) rc->parent = this; }
// 操作接口8\
   Rank size(); //统计当前节点后代总数，亦即以其为根的子树的规模
   Rank updateHeight(); //更新当前节点高度
   void updateHeightAbove(); //更新当前节点及其祖先的高度
   BinNodePosi<T> insertLc( T const& ); //插入左孩子
   BinNodePosi<T> insertRc( T const& ); //插入右孩子
   void attachLc( BinNodePosi<T> ); //接入左子树
   void attachRc( BinNodePosi<T> ); //接入右子树
   BinNodePosi<T> succ(); //取当前节点的直接后继
   template <typename VST> void travLevel( VST& ); //子树层次遍历
   template <typename VST> void travPre( VST& ); //子树先序遍历
   template <typename VST> void travIn( VST& ); //子树中序遍历
   template <typename VST> void travPost( VST& ); //子树后序遍历
// 比较器、判等器（各列其一，其余自行补充）
   bool operator< ( BinNode const& bn ) { return data < bn.data; } //小于
   bool operator== ( BinNode const& bn ) { return data == bn.data; } //等于
};


template <typename T> BinNodePosi<T> BinNode<T>::insertLc( T const& e )
   { return lc = new BinNode<T>( e, this ); } //将e作为当前节点的左孩子插入二叉树

template <typename T> BinNodePosi<T> BinNode<T>::insertRc( T const& e )
   { return rc = new BinNode<T>( e, this ); } //将e作为当前节点的右孩子插入二叉树


template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travIn ( VST& visit ) { //二叉树中序遍历算法统一入口
   switch ( rand() % 5 ) { //此处暂随机选择以做测试，共五种选择
      case 1 : travIn_I1( this, visit ); break; //迭代版#1
      case 2 : travIn_I2( this, visit ); break; //迭代版#2
      case 3 : travIn_I3( this, visit ); break; //迭代版#3
      case 4 : travIn_I4( this, visit ); break; //迭代版#4
      default : travIn_R( this, visit ); break; //递归版
    }
}

// BinNode状态与性质的判断

// 与BinNode具有特定关系的节点及指针

#define IsRoot(x) ( ! ( (x).parent ) )
#define IsLChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->lc ) )
#define IsRChild(x) ( ! IsRoot(x) && ( & (x) == (x).parent->rc ) )
#define HasParent(x) ( ! IsRoot(x) )
#define HasLChild(x) ( (x)->lc )
#define HasRChild(x) ( (x)->rc )
#define HasChild(x) ( HasLChild(x) || HasRChild(x) ) //至少拥有一个孩子
#define HasBothChild(x) ( HasLChild(x) && HasRChild(x) ) //同时拥有两个孩子
#define IsLeaf(x) ( (!(x)->lc) && (!(x)->rc) )
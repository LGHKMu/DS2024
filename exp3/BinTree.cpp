#include <iostream> 
#include <cstring> 
#include <algorithm>
#include "vector"
#include "BinNode.cpp"

template <typename T> class BinTree { //二叉树模板类
protected:
   Rank _size; BinNodePosi<T> _root; //规模、根节点
public:
   BinTree() : _size( 0 ), _root( NULL ) {} //构造方法
   ~BinTree() { if ( 0 < _size ) remove( _root ); } //析构方法
   BinTree( BinTree<T> const & ); //复制方法
   Rank size() const { return _size; } //规模
   bool empty() const { return !_root; } //判空
   BinNodePosi<T> root() const { return _root; } //树根
   BinNodePosi<T> insert( T const& ); //插入根节点
   BinNodePosi<T> insert( T const&, BinNodePosi<T> ); //插入左孩子
   BinNodePosi<T> insert( BinNodePosi<T>, T const& ); //插入右孩子
   BinNodePosi<T> attach( BinTree<T>*&, BinNodePosi<T> ); //接入左子树
   BinNodePosi<T> attach( BinNodePosi<T>, BinTree<T>*& ); //接入右子树
   Rank remove ( BinNodePosi<T> ); //子树删除
   BinTree<T>* secede ( BinNodePosi<T> ); //子树分离
   template <typename VST> //操作器
   void travLevel( VST& visit ) { if ( _root ) _root->travLevel( visit ); } //层次遍历
   template <typename VST> //操作器
   void travPre( VST& visit ) { if ( _root ) _root->travPre( visit ); } //先序遍历
   template <typename VST> //操作器
   void travIn( VST& visit ) { if ( _root ) _root->travIn( visit ); } //中序遍历
   template <typename VST> //操作器
   void travPost( VST& visit ) { if ( _root ) _root->travPost( visit ); } //后序遍历
   bool operator<( BinTree<T> const& t ) //比较器（其余自行补充）
      { return _root && t._root && ( *_root < *(t._root) ); }
   bool operator==( BinTree<T> const& t ) //判等器
      { return _root && t._root && ( _root == t._root ); }
}; //BinTree



template <typename T> Rank BinNode<T>::updateHeight() //更新当前节点高度
   { return height = 1 + std::max( stature( lc ), stature( rc ) ); } //具体规则，因树而异

template <typename T> void BinNode<T>::updateHeightAbove() //更新当前节点及其祖先的高度
   { for ( BinNodePosi<T> x = this; x; x = x->parent ) x->updateHeight(); } //可优化


template <typename T> BinNodePosi<T> BinTree<T>::insert( T const& e )
   { _size = 1; return _root = new BinNode<T>( e ); } //将e当作根节点插入空的二叉树

template <typename T> BinNodePosi<T> BinTree<T>::insert( T const& e, BinNodePosi<T> x )
   { _size++; x->insertLc( e ); x->updateHeightAbove(); return x->lc; } // e插入为x的左孩子

template <typename T> BinNodePosi<T> BinTree<T>::insert( BinNodePosi<T> x, T const& e )
   { _size++; x->insertRc( e ); x->updateHeightAbove(); return x->rc; } // e插入为x的右孩子


template <typename T> //将S当作节点x的左子树接入二叉树，S本身置空
BinNodePosi<T> BinTree<T>::attach( BinTree<T>*& S, BinNodePosi<T> x ) { // x->lc == NULL
   if ( x->lc = S->_root ) x->lc->parent = x; //接入
   _size += S->_size; x->updateHeightAbove(); //更新全树规模与x所有祖先的高度
   S->_root = NULL; S->_size = 0; return x; //释放原树，返回接入位置
}
/*template <typename T> //二叉树子树接入算法：将S弼作节点x癿右子树接入，S本身置空
BinNodePosi(T) BinTree<T>::attachAsRC ( BinNodePosi(T) x, BinTree<T>* &S ) { //x->rc == NULL
   if ( x->rc = S->_root ) x->rc->parent = x; //接入
   _size += S->_size; updateHeightAbove ( x ); //更新全树觃模不x所有祖先癿高度
   S->_root = NULL; S->_size = 0; release ( S ); S = NULL; return x; //释放原树，迒回接入位置
}*/

template <typename T> //将S当作节点x的右子树接入二叉树，S本身置空
BinNodePosi<T> BinTree<T>::attach( BinNodePosi<T> x, BinTree<T>*& S ) { // x->rc == NULL
   if ( x->rc = S->_root ) x->rc->parent = x; //接入
   _size += S->_size; x->updateHeightAbove(); //更新全树规模与x所有祖先的高度
   S->_root = NULL; S->_size = 0; return x; //释放原树，返回接入位置
}



template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
Rank BinTree<T>::remove( BinNodePosi<T> x ) { // assert: x为二叉树中的合法位置
   FromParentTo( x ) = NULL; //切断来自父节点的指针
   x->parent->updateHeightAbove(); //更新祖先高度
   Rank n = removeAt( x ); _size -= n; return n; //删除子树x，更新规模，返回删除节点总数
}
template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
static Rank removeAt( BinNodePosi<T> x ) { // assert: x为二叉树中的合法位置
   if ( !x ) return 0; //递归基：空树
   Rank n = 1 + removeAt( x->lc ) + removeAt( x->rc ); //递归释放左、右子树
   delete x; return n; //释放被摘除节点，并返回删除节点总数
}



template <typename T> //二叉树子树分离算法：将子树x从当前树中摘除，将其封装为一棵独立子树返回
BinTree<T>* BinTree<T>::secede( BinNodePosi<T> x ) { // assert: x为二叉树中的合法位置
   FromParentTo( x ) = NULL; //切断来自父节点的指针
   x->parent->updateHeightAbove(); //更新原树中所有祖先的高度
   BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = NULL; //新树以x为根
   S->_size = x->size(); _size -= S->_size; return S; //更新规模，返回分离出来的子树
}


template <typename T, typename VST> //元素类型、操作器
void travPre_R( BinNodePosi<T> x, VST& visit ) { //二叉树先序遍历算法（递归版）
   if ( !x ) return;
   visit( x->data );
   travPre_R( x->lc, visit );
   travPre_R( x->rc, visit );
}

template <typename T, typename VST> //元素类型、操作器
void travPost_R( BinNodePosi<T> x, VST& visit ) { //二叉树后序遍历算法（递归版）
   if ( !x ) return;
   travPost_R( x->lc, visit );
   travPost_R( x->rc, visit );
   visit( x->data );
}


template <typename T, typename VST> //元素类型、操作器
void travIn_R ( BinNodePosi<T> x, VST& visit ) { //二叉树中序遍历算法（递归版）
   if ( !x ) return;
   travIn_R ( x->lc, visit );
   visit ( x->data );
   travIn_R ( x->rc, visit );
}


/*template <typename T> template <typename VST> //元素类型、操作器
void BinNode <T>::travLevel( VST& visit ) { //二叉树层次遍历算法
   Queue<BinNodePosi<T>> Q; Q.enqueue( this ); //引入辅助队列，根节点入队
   while ( !Q.empty() ) { //在队列再次变空之前，反复迭代
      BinNodePosi<T> x = Q.dequeue(); visit( x->data ); //取出队首节点并访问之
      if ( x->lc ) Q.enqueue( x->lc ); //左孩子入队
      if ( x->rc ) Q.enqueue( x->rc ); //右孩子入队
   }
}*/

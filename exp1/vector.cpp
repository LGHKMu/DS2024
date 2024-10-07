#include <iostream> 
#include <ctime>
#include <cmath>
using namespace std;
typedef int Rank;//秩
#define DEFAULT_CAPACITY 3//默认的初始容量(实际应用中可设置为更大)
template <typename T> class Vector{ //向量模板类
protected:
    Rank _size;int _capacity;T*_elem;//规模、容量、数据区
    void copyFrom(T const*A,Rank lo,Rank hi);//复制数组区间A[1o,hi)
    void expand();//空间不足时扩容
    void shrink();//装填因子过小时压缩
    bool bubble(Rank lo,Rank hi);//扫描交换
    void bubbleSort(Rank lo,Rank hi);//起泡排序算法
    Rank max(Rank lo,Rank hi);//选取最大元素
    void selectionSort(Rank lo,Rank hi);//选择排序算法
    void merge ( Rank lo, Rank mi, Rank hi ); //归并算法
    void mergeSort(Rank lo,Rank hi);//归并排序算法
    Rank partition(Rank lo,Rank hi);//轴点构造算法
    void quickSort(Rank lo,Rank hi);//快速排序算法
    void heapSort(Rank lo,Rank hi);//堆排序
public:
//构造函数
    Vector( int c = DEFAULT_CAPACITY,int s = 0, T v=0)////容量为c、规模为s、所有元素初始为v
    { _elem = new T[_capacity = c]; for(_size = 0;_size < s;_elem[_size++] = v );}
    Vector(T const*A,Rank n){copyFrom(A,0,n);}//数组整体复制
    Vector(T const*A,Rank lo,Rank hi){copyFrom(A,lo,hi);}//区间
    Vector(Vector<T>const&V){copyFrom(V._elem,0,V._size );}//向量整体复制
    Vector(Vector<T>const&V,Rank lo,Rank hi){copyFrom(V._elem,lo,hi );}//区间
// 析构函数
    ~Vector(){ delete [] _elem; }//释放内部空间
//只读访问接口
    Rank size()const{return size;}//规模
    bool empty()const{return!_size;}//判空
    int disordered()const;//判断向量是否已排序
    Rank find(T const&e)const{return find(e,0,_size);}//无序向量整体查找
    Rank find(T const&e,Rank lo,Rank hi)const;//无序向量区间查找
    Rank search(T const&e)const //有序向量整体查找
    { return (0>=_size)?-1:search(e,0,_size );}
    Rank search(T const&e,Rank lo,Rank hi)const;//有序向量区间查找
// 可写访问接口
    T& operator[](Rank r)const;//重载下标操作符,可以类似于数组形式引用各元素
    Vector<T>& operator=(Vector<T>const&);//重载赋值操作符,以便直接克降向量
    T remove(Rank r);//删除秩为r的元素
    int remove(Rank lo,Rank hi);//删除秩在区间[1o,hi)之内的元素
    Rank insert(Rank r,T const&e);//插入元素
    Rank insert(T const&e){return insert(_size,e);}//默认作为末元素插入
    void OPsort(Rank lo,Rank hi);//对[1o,hi)排序
    void OPsort(){OPsort(0,_size);}//整体排序
    void sort(Rank lo,Rank hi);//对[1o,hi)排序
    void sort(){sort(0,_size);}//整体排序
    void unsort(Rank lo,Rank hi);//对[l0,hi)置乱
    void unsort(){unsort(0,_size);}//整体置乱
    int deduplicate();//无序去重
    int uniquify();//有序去重
// 遍历
    void traverse(void(*)(T&));//遍历(使用函数指针,只读或局部性修改)
    template<typename VST> void traverse ( VST& );//遍历(使用函数对象,可全局性修改)
    void print(){
        for (int i = 0; i <_size; i++)
        {
            cout<<_elem[i]<<"\t";
        }
        cout<<endl;
    }
};


template <typename T> void Vector<T>::copyFrom ( T const* A, Rank lo, Rank hi ) { //以数组区间A[lo, hi)为蓝本复制向量
    _elem = new T[_capacity = 2 * ( hi - lo ) ]; _size = 0; //分配空间,规模清零
    while ( lo < hi ) //A[lo, hi)内的元素统一
        _elem[_size++] = A[lo++]; //复制至_elem[0, hi - lo)
}

template <typename T> Vector<T>& Vector<T>::operator= ( Vector<T> const& V ) { //重载
    if ( _elem ) delete [] _elem; //释放原有内容
    copyFrom ( V._elem, 0, V.size() ); //整体复制
    return *this; //返回当前对象的引用,以便链式赋值
}
template <typename T> void Vector<T>::expand() { //向量空间丌足时扩容
    if ( _size < _capacity ) return; //尚未满员时,丌必扩容
    if ( _capacity < DEFAULT_CAPACITY ) _capacity = DEFAULT_CAPACITY; //丌低于最小容量
    T* oldElem = _elem; _elem = new T[_capacity <<= 1]; //容量加倍
    for ( int i = 0; i < _size; i++ )
        _elem[i] = oldElem[i]; //复刢原向量内容（T为基本类型,戒已重载赋值操作符'='）
    delete [] oldElem; //释放原空间
}
template <typename T> void Vector<T>::shrink() { //装填因子过小时压缩向量所占空间
    if ( _capacity < DEFAULT_CAPACITY << 1 ) return; //不致收缩到DEFAULT_CAPACITY以下
    if ( _size << 2 > _capacity ) return; //以25%为界
    T* oldElem = _elem; _elem = new T[_capacity >>= 1]; //容量减半
    for ( int i = 0; i < _size; i++ ) _elem[i] = oldElem[i]; //复制原向量内容
    delete [] oldElem; //释放原空间
}
template <typename T> void Vector<T>::unsort ( Rank lo, Rank hi ) { //等概率随机置乱区间[lo, hi)
    T* V = _elem + lo; //将子向量_elem[lo, hi)视作另一向量V[0, hi - lo)
    for ( Rank i = hi - lo; i > 0; i-- ) //自后向前
        swap( V[i - 1], V[rand() % i] ); //将V[i - 1]不V[0, i)中某一元素随机交换
}

template <typename T> Rank Vector<T>::find ( T const& e, Rank lo, Rank hi ) const { //assert: 0 <= lo < hi <= _size//无序向量的顺序查找：返回最后一个元素e的位置；失败时,返回lo - 1
    while ( ( lo < hi-- ) && ( e != _elem[hi] ) ); //从后向前,顺序查找
    return hi; //若hi < lo,则意味着失败；否则hi即命中元素的秩
}
template <typename T> Rank Vector<T>::insert ( Rank r, T const& e ) { //assert: 0 <= r <= size//将e作为秩为r元素插入
    expand(); //若有必要，扩容
    for ( int i = _size; i > r; i-- ) _elem[i] = _elem[i-1]; //自后向前，后继元素顺次后移一个单元
    _elem[r] = e; _size++; //置入新元素并更新容量
    return r; //返回秩
}
template <typename T> int Vector<T>::remove ( Rank lo, Rank hi ) { //删除区间[lo, hi)
    if ( lo == hi ) return 0; //出于效率考虑，单独处理退化情况，比如remove(0, 0)
    while ( hi < _size ) _elem[lo++] = _elem[hi++]; //[hi, _size)顺次前移hi - lo个单元
    _size = lo; //更新规模，直接丢弃尾部[lo, _size = hi)区间
    shrink(); //若有必要，则缩容
    return hi - lo; //返回被删除元素的数目
}
template <typename T> T Vector<T>::remove ( Rank r ) { //删除向量中秩为r的元素，0 <= r < size
    T e = _elem[r]; //备份被删除元素
    remove ( r, r + 1 ); //调用区间删除算法，等效于对区间[r, r + 1)的删除
    return e; //返回被删除元素
}
template <typename T> int Vector<T>::deduplicate() { //删除无序向量中重复元素（高效版）
    int oldSize = _size; //记录原规模
    Rank i = 1; //从_elem[1]开始
    while ( i < _size ) //自前向后逐一考查各元素_elem[i]
        ( find ( _elem[i], 0, i ) < 0 ) ? //在其前缀中寻找与之雷同者（至夗一个）
        i++ : remove ( i ); //若无雷同则继续考查其后继，否则删除雷同者
    return oldSize - _size; //向量规模变化量，即被删除元素总数
}
template <typename T> void Vector<T>::traverse ( void ( *visit ) ( T& ) ) //借助函数指针机刢
{ for ( int i = 0; i < _size; i++ ) visit ( _elem[i] ); } //遍历向量

template <typename T> template <typename VST>void Vector<T>::traverse ( VST& visit ) //借助函数对象机刢 //元素类型、操作器
{ for ( int i = 0; i < _size; i++ ) visit ( _elem[i] ); } //遍历向量

template <typename T> int Vector<T>::disordered() const { //返回向量中逆序相邻元素对的总数
    int n = 0; //计数器
    for ( int i = 1; i < _size; i++ ) //逐一检查_size - 1对相邻元素
        if ( _elem[i - 1] > _elem[i] ) n++; //逆序则计数
    return n; //向量有序当且仅当n = 0
}

template <typename T> int Vector<T>::uniquify() { //有序向量重复元素剔除算法（高效版）
    Rank i = 0, j = 0; //各对互异“相邻”元素的秩
    while ( ++j < _size ) //逐一扫描，直至末元素
        if ( _elem[i] != _elem[j] ) //跳过雷同者
            _elem[++i] = _elem[j]; //发现不同元素时，向前移至紧邻于前者右侧
    _size = ++i; shrink(); //直接截除尾部多余元素
    return j - i; //向量规模变化量，即被删除元素总数
}

template <typename T> Rank Vector<T>::search ( T const& e, Rank lo, Rank hi ) const { //assert: 0 <= lo < hi <= _size//在有序向量的区间[lo, hi)内，确定不大于e的最后一个节点的秩
    return (binSearch ( _elem, e, lo, hi ));
    /*return ( rand() % 2 ) ? //按各50%的概率随机使用二分查找或Fibonacci查找
        binSearch ( _elem, e, lo, hi ) : fibSearch ( _elem, e, lo, hi );*/
}

template <typename T> static Rank binSearch ( T* A, T const& e, Rank lo, Rank hi ) {
    while ( lo < hi ) { //每步迭代仅需做一次比较判断，有两个分支
        Rank mi = ( lo + hi ) >> 1; //以中点为轴点
        ( e < A[mi] ) ? hi = mi : lo = mi + 1; //经比较后确定深入[lo, mi)或(mi, hi)
    } //成功查找不能提前终止
    return --lo; //循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
} //有多个命中元素时，总能保证返回秩最大者；查找失败时，能够返回失败的位置

template <typename T> void Vector<T>::sort ( Rank lo, Rank hi ) { //向量区间[lo, hi)排序
    switch ( rand() % 2 ) { //随机选取排序算法。可根据具体问题的特点灵活选取或扩充
        case 0: bubbleSort ( lo, hi ); break; //起泡排序
        //case 2: selectionSort ( lo, hi ); break; //选择排序
        case 1: mergeSort ( lo, hi ); break; //弻幵排序
        //case 4: heapSort ( lo, hi ); break; //堆排序
        //default: quickSort ( lo, hi ); break; //快速排序
    }
}

template <typename T> void Vector<T>::OPsort ( Rank lo, Rank hi ) { //向量区间[lo, hi)排序
    sort();
    if (_size%2==0)
    {
        for ( int i = 0; i <_size/2; i++)
    {
        swap(_elem[i],_elem[_size-1-i]);
    }
    }
    else
        for ( int i = 0; i <_size-1/2; i++)
    {
        swap(_elem[i],_elem[_size-1-i]);
    }
    
}


template <typename T> //向量癿起泡排序
void Vector<T>::bubbleSort ( Rank lo, Rank hi ) //assert: 0 <= lo < hi <= size
{ while ( !bubble ( lo, hi-- ) ); } //逐趟做扫描交换，直至全序

template <typename T> bool Vector<T>::bubble ( Rank lo, Rank hi ) { //一趟扫描交换
    bool sorted = true; //整体有序标志
    while ( ++lo < hi ) //自左向右，逐一检查各对相邻元素
        if ( _elem[lo - 1] > _elem[lo] ) { //若逆序，则
            sorted = false; //意味着尚未整体有序，幵需要
            swap ( _elem[lo - 1], _elem[lo] ); //通过交换使尿部有序
        }
    return sorted; //迒回有序标志
}

template <typename T>
void Vector<T>::mergeSort ( Rank lo, Rank hi ) { //0 <= lo < hi <= size //向量归并排序
    if ( hi - lo < 2 ) return; //单元素的间自然有序，否则...
    int mi = ( lo + hi ) /2;
    mergeSort ( lo, mi ); mergeSort ( mi, hi ); //以中点为界分别排序
    merge ( lo, mi, hi ); //归并
}
template <typename T> void Vector<T>::merge ( Rank lo, Rank mi, Rank hi ) { //各自有序的子向量[lo, mi)和[mi, hi)//有序向量的归并
    T* A = _elem + lo; //合并后的向量A[0, hi - lo) = _elem[lo, hi)
    int lb = mi - lo; T* B = new T[lb]; //前子向量B[0, lb) = _elem[lo, mi)
    for ( Rank i = 0; i < lb; B[i] = A[i++] ); //复制前子向量
    int lc = hi - mi; T* C = _elem + mi; //后子向量C[0, lc) = _elem[mi, hi)
    for ( Rank i = 0, j = 0, k = 0; ( j < lb ) || ( k < lc ); ) { //B[j]和C[k]中的小者续至A末尾
        if ( ( j < lb ) && ( ! ( k < lc ) || ( B[j] <= C[k] ) ) ) A[i++] = B[j++];
        if ( ( k < lc ) && ( ! ( j < lb ) || ( C[k] < B[j] ) ) ) A[i++] = C[k++];
    }
    delete [] B; //释放临时空间B
} //归并后得到完整的有序向量[lo, hi)

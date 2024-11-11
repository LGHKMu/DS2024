#include <queue>
#include <vector>
#include <functional>
#include "BinTree.cpp"
#include "Bitmap.cpp" //基于Bitmap实现
#include "List.cpp"

#define  N_CHAR  (0x80 - 0x20) //仅以可打印字符为例
struct HuffChar { //Huffman（超）字符
   char ch; unsigned int weight; //字符、频率
   HuffChar ( char c = '^', unsigned int w = 0 ) : ch ( c ), weight ( w ) {};
   HuffChar ( HuffChar const & hc ) : ch( hc.ch ), weight( hc.weight ) {};
   // 比较器、判等器（各列其一，其余自行补充）
   bool operator< ( HuffChar const& hc ) { return weight > hc.weight; } //此处故意大小颠倒
   bool operator== ( HuffChar const& hc ) { return weight == hc.weight; }
};

#define HuffTree BinTree<HuffChar>
typedef List<HuffTree*> HuffForest;

HuffForest* initForest ( unsigned int* freq ) { //根据频率统计表，为每个字符创建一棵树
   HuffForest* forest = new HuffForest; //以List实现Huffman森林
   //HuffForest* forest = new HuffForest; //若不为测试扩容，可直接new HuffForest(0x80-0x20)
   for ( int i = 0; i < N_CHAR; i++ ) { //为每个字符
      forest->insertLast (  new HuffTree ); //生成一棵树，并将字符及其频率
      forest->last()->data->insert ( HuffChar ( 0x20 + i, freq[i] ) ); //存入其中
   }
   return forest;
}

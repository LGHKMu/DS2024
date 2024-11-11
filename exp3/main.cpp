#include <iostream> 
#include <cstring> 
#include <vector>
#include <queue>
#include <functional>
#include "HuffTree.cpp"
#include <unordered_map>
//const int N_CHAR = 26
unsigned int * stat( const char* sample_text_file ) { //统计字符出现频率
   unsigned int * freq = new unsigned int[N_CHAR];  //以下统计需随机访问，故以数组记录各字符出现次数
   memset ( freq, 0, sizeof ( int ) * N_CHAR ); //清零
   FILE* fp = fopen ( sample_text_file, "r" ); //assert: 文件存在且可正确打开
   for ( char ch; 0 < fscanf ( fp, "%c", &ch ); ) //逐个扫描样本文件中的每个字符
      if ( (0x20 <= ch) && (ch < 0x80) ) freq[ch - 0x20]++; //累计对应的出现次数
   fclose ( fp ); return freq;
}

HuffTree* minHChar ( HuffForest* forest ) { //在Huffman森枃中找出权重最小癿（超）字符
   ListNodePosi <HuffTree*> p = forest->first(); //从首节点出収查找
   ListNodePosi <HuffTree*> minChar = p; //最小Huffman树所在癿节点位置
   int minWeight = p->data->root()->data.weight; //目前癿最小权重
   while ( forest->valid ( p = p->succ ) ) //遍历所有节点
      if ( minWeight > p->data->root()->data.weight ) //若弼前节点所含树更小，则
         { minWeight = p->data->root()->data.weight; minChar = p; } //更新记弽
   return forest->remove ( minChar ); //将挑选出癿Huffman树从森枃中摘除，幵迒回
   }
HuffTree* generateTree ( HuffForest* forest ) { //Huffman编码算法
   while ( 1 < forest->size() ) {
      HuffTree* T1 = minHChar ( forest ); HuffTree* T2 = minHChar ( forest );
      HuffTree* S = new HuffTree();
      S->insert ( HuffChar ( '^', T1->root()->data.weight + T2->root()->data.weight ) );
      S->attach ( T1, S->root() ); S->attach ( S->root(), T2 );
      forest->insertLast (S);
   } //assert: 循环结束时，森枃中唯一（列表首节点中）癿那棵树即Huffman编码树
   return forest->first()->data;
}
std::string encodeWord( std::unordered_map<char, std::string>& codes, const std::string& word) {
    std::string encodedWord;
    for (char ch : word) {
        if (std::isalpha(ch)) { // 只编码字母
            ch = std::tolower(ch); // 转换为小写
            encodedWord += codes[ch];
        } else {
            // 非字母字符保持不变或进行特殊处理（这里简单处理为直接添加）
            encodedWord += ch;
        }
    }
    return encodedWord;
}

void generateCodesHelper(BinNodePosi<HuffChar> node, const std::string& prefix, std::unordered_map<char, std::string>& codes) {
    if (node == nullptr) return;
 
    // 如果是叶子节点，则记录编码
    if (IsLeaf(node)) {
        codes[node->data.ch] = prefix; // 假设 data.charValue 是存储字符的成员
    } else {
        // 递归遍历左子树和右子树，并更新前缀
        generateCodesHelper(node->lc, prefix + "0", codes);
        generateCodesHelper(node->rc, prefix + "1", codes);
    }
}
 
std::unordered_map<char, std::string> generateCodes(HuffTree* tree) {
    std::unordered_map<char, std::string> codes;
    if (tree != nullptr && tree->root() != nullptr) {
        generateCodesHelper(tree->root(), "", codes);
    }
    return codes;
}

int main () {
    const char* textFile = "ihaveadream.txt"; // 假设这是演讲原文的文件名
    unsigned int* freq = stat(textFile);
    HuffForest* forest = initForest(freq);delete freq;
    HuffTree* tree = generateTree(forest); delete forest;
    std::unordered_map<char, std::string> codes = generateCodes(tree);
 
    std::string word1 = "dream";
 
    std::string encodedWord1 = encodeWord(codes, word1);
 
    std::cout << "Huffman Codes for '" << word1 << "': " << encodedWord1 << std::endl;
 
    // 释放内存（这里只是示例，实际应该使用智能指针或更好的内存管理策略）
 
   return 0; //释放编码表、编码树
}
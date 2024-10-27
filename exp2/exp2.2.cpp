#include <iostream>  
#include <vector>  
#include <stack>  
#include <cstdlib>  
#include <ctime>  
#include <algorithm>  
  
using namespace std;  
  
// 计算柱状图中最大矩形面积的函数  
int largestRectangleArea(vector<int>& heights) {  
    stack<int> stk; // 使用栈来存储柱子的索引  
    int maxArea = 0;  
    heights.push_back(0); // 添加一个高度为0的柱子，以确保所有柱子都能被处理  
  
    for (int i = 0; i < heights.size(); ++i) {  
        // 当栈不为空且当前柱子高度小于栈顶索引对应的高度时  
        while (!stk.empty() && heights[stk.top()] > heights[i]) {  
            int height = heights[stk.top()];  
            stk.pop();  
            int width = stk.empty() ? i : i - stk.top() - 1;  
            maxArea = max(maxArea, height * width);  
        }  
        stk.push(i);  
    }  
  
    // 栈中可能还有元素，需要处理  
    while (!stk.empty()) {  
        int height = heights[stk.top()];  
        stk.pop();  
        int width = stk.empty() ? heights.size() - 1 : heights.size() - stk.top() - 1;  
        maxArea = max(maxArea, height * width);  
    }  
  
    heights.pop_back(); // 移除最后添加的高度为0的柱子  
    return maxArea;  
}  
  
// 随机生成高度数组的函数  
vector<int> generateRandomHeights(int length, int maxHeight) {  
    vector<int> heights(length);  
    srand(time(0)); // 使用当前时间作为随机数种子  
    for (int i = 0; i < length; ++i) {  
        heights[i] = rand() % (maxHeight + 1);  
    }  
    return heights;  
}  
  
int main() {  
    // 测试10组数据  
    for (int i = 0; i < 10; ++i) {  
        int length = rand() % 105 + 1; // 随机生成长度在1到105之间的整数  
        int maxHeight = rand() % 104 + 1; // 随机生成高度在0到104之间的整数（包括0和104）  
        vector<int> heights = generateRandomHeights(length, maxHeight);  
  
        // 打印部分高度数组以节省空间（这里打印前10个和后10个元素，如果数组长度小于20则打印全部）  
        cout << "Test " << i + 1 << ": Heights = [";  
        for (int j = 0; j < min(10, (int)heights.size()); ++j) {  
            cout << heights[j];  
            if (j < min(10, (int)heights.size()) - 1) {  
                cout << ", ";  
            }  
        }  
        if (heights.size() > 10) {  
            cout << ", ..., ";  
            for (int j = heights.size() - 10; j < heights.size(); ++j) {  
                cout << heights[j];  
                if (j < heights.size() - 1) {  
                    cout << ", ";  
                }  
            }  
        }  
        cout << "] (length=" << length << ")" << endl;  
  
        // 计算并打印最大矩形面积  
        int maxArea = largestRectangleArea(heights);  
        cout << "Maximum Rectangle Area = " << maxArea << endl;  
    }  
  
    return 0;  
}
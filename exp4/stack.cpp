#include <cmath>
#include <cstring> 
#include "vector.cpp" 

template <typename T> class Stack: public Vector<T> { 
public: 
   void push ( T const& e ) { Vector<T>::insert ( e ); } 
   T pop() { return Vector<T>::remove ( Vector<T>::size() - 1 ); } 
   T& top() { return ( *this ) [Vector<T>::size() - 1]; } 
};

void convert ( Stack<char>& S, unsigned long long n, int base ) { //整数n的1<base<=16进制打印（递归版）
   static char digit[] = "0123456789ABCDEF"; //数位符号，如有必要可相应扩充
   if ( 0 < n ) { //在尚有余数之前，反复地
      S.push ( digit[n % base] ); //逆向记录当前最低位，再
      convert ( S, n / base, base ); //通过递归得到所有更高位
    }
} //新进制下由高到低的各数位，自顶而下保存于栈S中


void trim ( const char exp[], Rank& lo, Rank& hi ) { //删除exp[lo, hi]不含括号的最长前缀、后缀
   while ( ( lo <= hi ) && ( exp[lo] != '(' ) && ( exp[lo] != ')' ) ) lo++; //查找第一个和
   while ( ( lo <= hi ) && ( exp[hi] != '(' ) && ( exp[hi] != ')' ) ) hi--; //最后一个括号
}

Rank divide ( const char exp[], Rank lo, Rank hi ) { //切分exp[lo, hi]，使exp匹配仅当子表达式匹配
   int crc = 1; //crc为[lo, mi]范围内左、右括号数目之差
   while ( ( 0 < crc ) && ( ++lo < hi ) ) //逐个检查各字符，直到左、右括号数目相等，或者越界
      if ( exp[lo] == '(' ) crc ++;
      else if ( exp[lo] == ')' ) crc --;
   return lo;
}

bool paren ( const char exp[], Rank lo, Rank hi ) { //检查表达式exp[lo, hi]是否括号匹配（递归版）
   trim ( exp, lo, hi ); if ( lo > hi ) return true; //清除不含括号的前缀、后缀
   if ( ( exp[lo] != '(' ) || ( exp[hi] != ')' ) ) return false; //首、末字符非左、右括号，则必不匹配
   Rank mi = divide ( exp, lo, hi ); //确定适当的切分点
   return paren ( exp, lo + 1, mi - 1 ) && paren ( exp, mi + 1, hi ); //分别检查左、右子表达式
}


#define N_OPTR 9 //运算符总数
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //运算符集合
//加、减、乘、除、乘方、阶乘、左括号、右括号、起始符与终止符

const char pri[N_OPTR][N_OPTR] = { //运算符优先等级 [栈顶] [当前]
   /*              |-------------------- 当 前 运 算 符 --------------------| */
   /*              +      -      *      /      ^      !      (      )      \0 */
   /* --  + */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
   /* |   - */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
   /* 栈  * */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
   /* 顶  / */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
   /* 运  ^ */    '>',   '>',   '>',   '>',   '>',   '<',   '<',   '>',   '>',
   /* 算  ! */    '>',   '>',   '>',   '>',   '>',   '>',   ' ',   '>',   '>',
   /* 符  ( */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   '~',   ' ',
   /* |   ) */    ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',
   /* -- \0 */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   ' ',   '~'
};



void readNumber ( char*& p, Stack<double>& stk ) { //将起始于p的子串解析为数值，并存入操作数栈
   stk.push ( ( double ) ( *p - '0' ) ); //当前数位对应的数值进栈
   while ( isdigit ( * ( ++p ) ) ) //若有后续数字（多位整数），则
      stk.push ( stk.pop() * 10 + ( *p - '0' ) ); //追加之（可能上溢）
   if ( '.' == *p ) { //若还有小数部分
      double fraction = 1; //则
      while ( isdigit ( * ( ++p ) ) ) //逐位
         stk.push ( stk.pop() + ( *p - '0' ) * ( fraction /= 10 ) ); //加入（可能下溢）
   }
}


Operator optr2rank ( char op ) { //由运算符转译出编号
   switch ( op ) {
      case '+' : return ADD; //加
      case '-' : return SUB; //减
      case '*' : return MUL; //乘
      case '/' : return DIV; //除
      case '^' : return POW; //乘方
      case '!' : return FAC; //阶乘
      case '(' : return L_P; //左括号
      case ')' : return R_P; //右括号
      case '\0': return EOE; //起始符与终止符
      default  : exit ( -1 ); //未知运算符
   }
}

char priority ( char op1, char op2 ) //比较两个运算符之间的优先级
{ return pri[optr2rank ( op1 ) ][optr2rank ( op2 ) ]; }

void append(char* rpn, double opnd) {
    char buf[64];
    if ((int)opnd < opnd) 
        sprintf(buf, "%6.2f ", opnd);  // 浮点格式
    else                       
        sprintf(buf, "%d ", (int)opnd); // 整数格式
    strcat(rpn, buf); // 追加至RPN
}

void append(char* rpn, char optr) {
    char buf[4]; // 足够大以容纳单个字符和空格
    sprintf(buf, "%c ", optr);
    strcat(rpn, buf);
}

/*
void append ( char* rpn, double opnd ) { //将操作数接至RPN末尾
   char buf[64];
   if ( ( int ) opnd < opnd ) sprintf ( buf, "%6.2f \0", opnd ); //浮点格式，或
   else                       sprintf ( buf, "%d \0", ( int ) opnd ); //整数格式
   strcat ( rpn, buf ); //RPN加长
}

void append ( char* rpn, char optr ) { //将运算符接至RPN末尾
   int n = strlen ( rpn ); //RPN当前长度（以'\0'结尾，长度n + 1）
   sprintf ( rpn + n, "%c \0", optr ); //接入指定的运算符
}
*/
int fact(int n){
    if(n==0) return 1;
    return n * fact(n-1);
}

float calcu(char op, float opnd){
    switch (op) {
    case '!':  
        return fact(opnd);  
    default:  
        throw std::invalid_argument("Unknown operator"); 
        }
}; // 一元运算  
float calcu(float opnd1, char op, float opnd2){
    switch (op) {
        case '+':  
            return opnd1 + opnd2;  
        case '-':  
            if (opnd2 == 0.0f) { // 处理一元负号  
                return -opnd1;  
            } else {  
                return opnd1 - opnd2;  
            }  
        case '*':  
            return opnd1 * opnd2;  
        case '/':  
            if (fabs(opnd2) < 1e-9) { // 避免除以零的错误  
                throw std::invalid_argument("Division by zero");  
            }
            return opnd1 / opnd2;  
        case '^':
            return pow(opnd1,opnd2);
        default:
            throw std::invalid_argument("Unknown operator");  
    }  
}; // 二元运算  



double evaluate ( char* S, char* RPN ) { //对（已剔除白空格的）表达式S求值，并转换为逆波兰式RPN
   Stack<double> opnd; Stack<char> optr; //运算数栈、运算符栈
   optr.push ( '\0' ); //尾哨兵'\0'也作为头哨兵首先入栈
   while ( !optr.empty() ) { //在运算符栈非空之前，逐个处理表达式中各字符
      if ( isdigit ( *S ) ) { //若当前字符为操作数，则
         readNumber ( S, opnd ); append ( RPN, opnd.top() ); //读入操作数，并将其接至RPN末尾
      } else //若当前字符为运算符，则
         switch ( priority ( optr.top(), *S ) ) { //视其与栈顶运算符之间优先级高低分别处理
            case '<': //栈顶运算符优先级更低时
               optr.push ( *S ); S++; //计算推迟，当前运算符进栈
               break;
            case '>': { //栈顶运算符优先级更高时，可实施相应的计算，并将结果重新入栈
               char op = optr.pop(); append ( RPN, op ); //栈顶运算符出栈并续接至RPN末尾
               if ( '!' == op ) //若属于一元运算符
                  opnd.push ( calcu ( op, opnd.pop() ) ); //则取一个操作数，计算结果入栈
               else { //对于其它（二元）运算符
                  double opnd2 = opnd.pop(), opnd1 = opnd.pop(); //取出后、前操作数
                  opnd.push ( calcu ( opnd1, op, opnd2 ) ); //实施二元计算，结果入栈
               }
               break;
            }
            case '~' : //匹配的运算符（括号或'\0'）时
               optr.pop(); S++; //脱括号并转至下一字符
               break;
            default : exit( -1 ); //逢语法错误，不做处理直接退出
         }//switch
   }//while
   return opnd.pop(); //弹出并返回最后的计算结果
}

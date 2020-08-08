/* v1
* Initial Version
*/

#include <iostream>
#include <cstring>
#include <vector>
#include <list>
#include <map>

using namespace std;

using u8 = unsigned char;
using u16 = unsigned short int;
using u32 = unsigned int;
using u64 = unsigned long long;

// Two messages
struct Hello {
    u8 X;
    u8 Y[3];
};

struct Hi {
    u8 X;
    u8 W[4];
};

u8* pMsg = NULL;
u32 msgLen;

int EncodeMsg(Hello * msg) {
    
    u32 sz = sizeof(Hello::X) + sizeof(Hello::Y);

    pMsg = new u8[sz];
    memset(pMsg,0,sz);
    int currLen = 0;

    // encode X
    pMsg[currLen] = msg->X;

    // increase currLen
    currLen += sizeof(Hello::X);

    // encode Y
    memcpy(pMsg + currLen,msg->Y,sizeof(Hello::Y));

    // increase currLen
    currLen += sizeof(Hello::Y);

    msgLen = currLen;
    return 0;
}

int EncodeMsg(Hi * msg) {
    // similar implementation
    return 0;
}

int main() {
    Hello m;
    
    memset(&m,0,sizeof(m));

    m.X = 32;
    
    u8 buf[3] = {0x10,0x20,0x14};
    
    memcpy(m.Y,buf,sizeof(buf));

    EncodeMsg(&m);
    
    for(int i = 0;i<msgLen;i++) {
        printf("0x%X ",pMsg[i]);
    }
    
    cout << endl;
    delete[] pMsg;
}
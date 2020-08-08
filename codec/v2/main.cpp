/* v2
* Improvement form the prev version:
* 1. Single Encode Interface for different message
* 2. Single context for all the messages, reuse context varibles
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
struct Context {
    // Three different types of Information Element
    u8 X;
    u8 Y[3];
    u8 W[4];
};

u8* pMsg = NULL;
u32 msgLen;

enum MessageType_e {
    Hello,
    Hi
};

int EncodeMsg(Context * ctx,MessageType_e msgType) {
    // clean pMsg if required ...
    int currLen = 0;
    switch (msgType) {
        case Hello:
        {
            // Specific encoding details of Hello message
            u32 sz = sizeof(Context::X) + sizeof(Context::Y);

            pMsg = new u8[sz];
            memset(pMsg,0,sz);

            // encode X
            pMsg[currLen] = ctx->X;

            // increase currLen
            currLen += sizeof(Context::X);

            // encode Y
            memcpy(pMsg + currLen,ctx->Y,sizeof(Context::Y));

            // increase currLen
            currLen += sizeof(Context::Y);

            break;
        }
        case Hi:
            // Specific encoding details of Hi message
            break;
        default:
            break;
    }
    msgLen = currLen;
    return 0;
}


int main() {
    Context ctx;
    
    memset(&ctx,0,sizeof(ctx));

    ctx.X = 32;
    
    u8 buf[3] = {0x10,0x20,0x14};
    
    memcpy(ctx.Y,buf,sizeof(buf));

    EncodeMsg(&ctx,Hello);
    
    for(int i = 0;i<msgLen;i++) {
        printf("0x%X ",pMsg[i]);
    }
    
    cout << endl;
    delete[] pMsg;
}
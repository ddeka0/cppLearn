/* v4
* Improvement form the prev version:
* 1. Completely decouple protocol message or IE encode logic 
* from encoder framework logic.
*    -> encoder framework will be untouched when there is a requirement
*   of adding or removing an IE, or helps in optional IE (next version)
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
const u32 Y_Size = 3;
const u32 W_Size = 4;


struct Context {
    // Three different types of Information Element
    u8 X;
    u8 Y[Y_Size];
    u8 W[W_Size];
};

void encode_X(u32 &offset,Context *ctx);
void encode_Y(u32 &offset,Context *ctx);
void encode_W(u32 &offset,Context *ctx);

using  encoderFunc = void (*)(u32&,Context*);
using encoderList = std::list<encoderFunc>;

u8* pMsg = NULL;
u32 msgLen;

enum MessageType_e : u8 {
    Hello,
    Hi
};

std::map<MessageType_e,encoderList> codecMap;

void initializeCodec() {
    codecMap.clear();
    // Hello Message
    codecMap.insert({Hello,encoderList()});
    codecMap.insert({Hi,encoderList()});

    {
        encoderList _list;
        _list.push_back(encode_X);
        _list.push_back(encode_Y);
        codecMap[Hello] = _list;
    }

    {
        encoderList _list;
        _list.push_back(encode_X);
        _list.push_back(encode_W);
        codecMap[Hi] = _list;
    }

}

int EncodeMsg(Context * ctx,MessageType_e msgType) {
    // clean pMsg if required ...
    u32 currLen = 0;
    switch (msgType) {
        case Hello:
        {
            u32 sz = sizeof(Context::X) + sizeof(Context::Y);
            pMsg = new u8[sz];
            memset(pMsg,0,sz);

            if(codecMap.find(Hello) != codecMap.end()) {
                for(auto func:codecMap[Hello]) {
                    (*func)(currLen,ctx);
                }
            }else {
                cout <<"codec list not found for message \"Hello\"" << endl;
            }
            

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

// reusable encode functions
void encode_X(u32 &offset,Context *ctx) {
    pMsg[offset] = ctx->X;
    offset += sizeof(Context::X);
}

void encode_Y(u32 &offset,Context *ctx) {
    memcpy(pMsg + offset,ctx->Y,sizeof(Context::Y));
    offset += sizeof(Context::Y);
}

void encode_W(u32 &offset, Context* ctx){return;}

int main() {
    initializeCodec();

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
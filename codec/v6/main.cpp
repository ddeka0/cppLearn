/* v5
* Improvement form the prev version:
* 1. Functionalities for optional parameters
*   -> IEI, Variable Len depending on protocol
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
const u32 Z_MAX_Size = 20;


struct Context {
    // Four different types of Information Element
    u8 X;
    u8 Y[Y_Size];
    u8 W[W_Size];
    u8 Z[Z_MAX_Size]; // Optional max len <= 255
};

void encode_X(std::vector<u8>& msgVector,Context *ctx);
void encode_Y(std::vector<u8>& msgVector,Context *ctx);
void encode_W(std::vector<u8>& msgVector,Context *ctx);
void encode_Z(std::vector<u8>& msgVector,Context *ctx);

enum IeType_e {
    Mandatory,
    Optional
};

using  encoderFunc = void (*)(std::vector<u8>&,Context*);

struct IeCodecItem {
public:
    IeCodecItem(IeType_e _type,u16 mnlen,u16 mxlen,u8 _iei,encoderFunc _func)
        : ieType(_type),minLen(mnlen),maxLen(mxlen),iei(_iei),func(_func) {}
    IeType_e ieType;
    u16 minLen;
    u16 maxLen;
    u8 iei;
    encoderFunc func;
    bool isLengthValid(u32 len) {
        if(minLen <= len && len <= maxLen)
            return true;
        
        return false;
    }
};

enum MessageType_e : u8 {
    Hello,
    Hi
};

using encoderList = std::list<IeCodecItem>;
std::map<MessageType_e,encoderList> codecMap;

void initializeCodec() {
    codecMap.clear();
    // Hello & Hi Message
    codecMap.insert({Hello,encoderList()});
    codecMap.insert({Hi,encoderList()});

    {
        encoderList _list;
        _list.emplace_back(Mandatory,1,1,-1,encode_X);
        _list.emplace_back(Mandatory,Y_Size,Y_Size,-1,encode_Y);
        _list.emplace_back(Optional,1,Z_MAX_Size,0x34,encode_Z); /*IEI is 0x34*/
        codecMap[Hello] = _list;
    }

    {
        encoderList _list;
        _list.emplace_back(Mandatory,1,1,-1,encode_X);
        _list.emplace_back(Mandatory,W_Size,W_Size,-1,encode_W);
        codecMap[Hi] = _list;
    }

}

struct EncodeResult {
    u8* msg;
    u16 msgLen;
    u8 err; /* implementation specific */
    EncodeResult():msg(nullptr),msgLen(0),err(0){}
};

EncodeResult EncodeMsg(Context * ctx,MessageType_e msgType) {
    // clean pMsg if required ...

    std::vector<u8> msgVector;
    EncodeResult result;

    if(codecMap.find(msgType) != codecMap.end()) {
        
        msgVector.clear();

        for(auto item:codecMap[msgType]) {
            
            u32 prevSize = msgVector.size();
            // call to protocol specific encode function
            (*item.func)(msgVector,ctx);
            
            u32 newSize = msgVector.size();
            
            u32 valueSize = newSize - prevSize;

            // optional IE specific encode logic

            if(valueSize && item.ieType == Optional) {
                if(item.isLengthValid(valueSize + 1)) { // 1 Byte value size
                    auto iter = msgVector.end() - valueSize;   
                    msgVector.insert(iter,(u8)valueSize);                    
                }else {
                    result.err = 0x23; // some error code for len error
                }
                if(item.iei != 0) {
                    auto iter = msgVector.end() - (valueSize + 1);
                    msgVector.insert(iter,item.iei);
                }
            }
        }
    }else {
        cout <<"Codec list not found for requested message" << endl;
    }

    if(result.err == 0) {
        result.msgLen = msgVector.size();
        result.msg = new u8[result.msgLen];
        memcpy(result.msg,msgVector.data(),result.msgLen);
    }

    return result;
}

// reusable encode functions
void encode_X(std::vector<u8> &msgVector,Context *ctx) {
    msgVector.push_back(ctx->X);
}

void encode_Y(std::vector<u8> &msgVector,Context *ctx) {
    for(int i = 0;i<sizeof(Context::Y);i++) {
        msgVector.push_back(ctx->Y[i]);
    }
}

void encode_W(std::vector<u8> &msgVector, Context* ctx){return;}

// optional param encode function
void encode_Z(std::vector<u8> &msgVector, Context* ctx) {
    // .... some protocol logic to determine the size and contents of Z
    // from ctx and outcome is tbuf

    u8 tbuf[] = {0x02,0x03,0x04,0x05};
    for(int i = 0;i<sizeof(tbuf);i++) {
        msgVector.push_back(tbuf[i]);
    }
}

int main() {
    initializeCodec();

    // protocol specific context
    Context ctx;
    memset(&ctx,0,sizeof(ctx));
    ctx.X = 32;
    u8 buf[3] = {0x10,0x20,0x14};
    memcpy(ctx.Y,buf,sizeof(buf));


    EncodeResult res = EncodeMsg(&ctx,Hello);

    if(res.err == 0) {
        for(int i = 0;i<res.msgLen;i++) {
            printf("0x%X ",res.msg[i]);
        }
    }else {
        cerr <<"Error : "<<res.err << endl;
    }
    
    cout << endl;

    if(res.msg != nullptr) {
        delete[] res.msg;
    }
}
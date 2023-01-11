#include "Message.h"
#include <iostream>
#include <cstring>


using namespace std;

int main()
{
    const char* m = "bitch";
    Message *message = new Message(CONNECT, (short)2, (short)strlen(m), m);
    cout<<"message   "<<message->message<<endl;
    cout<<"message packet   "<<message->message_packet<<endl;
    cout<<"message id   "<<message->id<<endl;
    cout<<"message type   "<<message->type<<endl;
    cout<<"message length   "<<message->length<<endl;
    cout<<"message length   "<<strlen(m)<<endl;
    return 0;
}
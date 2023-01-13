#include "Message.h"
#include <cstring>

#include <iostream>
using namespace std;

Message::Message(const char *message)
{
    unsigned char type = message[0] & 0xF0;
    unsigned char id = message[0] & 0x0F;
    unsigned char length = message[1];
    this->type = static_cast<MessageTypes>((int)(type >> 4));
    this->id = (short)id;
    this->length = (short)length - 2;
    std::strcpy(message_packet, message);

    for (short i = 0; i < this->length; i++)
    {
        this->message[i] = message[i + 2];
    }
}

Message::Message(MessageTypes type, short id, short length, const char *message)
{
    this->id = id;
    this->type = type;
    this->length = length;
    if (length != 0)
    {
        std::strcpy(this->message, message);
    }
    message_packet[0] = this->getTypeAndId(static_cast<int>(type), id);
    message_packet[1] = static_cast<unsigned char>(length + 2);
    for (short i = 0; i < length; i++)
    {
        message_packet[i + 2] = message[i];
    }
}

Message::Message(MessageTypes type, short id, short length, const char message)
{
    this->id = id;
    this->type = type;
    this->length = length;
    this->message[0] = message;

    message_packet[0] = this->getTypeAndId(static_cast<int>(type), id);
    message_packet[1] = static_cast<unsigned char>(length + 2);
    message_packet[2] = message;
}

short Message::getTypeAndId(int type, short id)
{
    int type_and_id = type << 4 + id;
    return (short)type_and_id;
}
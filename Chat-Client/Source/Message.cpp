#include "Message.h"
#include <cstring>

Message::Message(const char *message)
{
    unsigned char type = message[0] & 0b11110000;
    unsigned char id = message[0] & 0b00001111;
    unsigned char length = message[1];
    this->type = static_cast<MessageTypes>((int)type);
    this->id = (int)id;
    this->length = (int)length - 2;
    this->message_packet = new char[(int)length];
    std::strcpy(message_packet, message);

    this->message = new char[this->length];

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
    this->message = new char[length];
    std::strcpy(this->message, message);

    this->message_packet = new char[length + 2];
    message_packet[0] = this->getTypeAndId(static_cast<int>(type), id);
    message_packet[1] = static_cast<unsigned char>(length);
    for (short i = 0; i < length; i++)
    {
        message_packet[i + 2] = message[i];
    }
}

unsigned char Message::getTypeAndId(int type, short id)
{
    int typeAndId = type << 4 + id;
    return static_cast<unsigned char>(typeAndId);
}
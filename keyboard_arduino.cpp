#include "keyboard_arduino.h"

using namespace Z;

void set_key(const uint8_t index, const uint16_t key)
{
    switch(index)
    {
        case 0:
            Keyboard.set_key1(key);
        case 1:
            Keyboard.set_key2(key);
        case 2:
            Keyboard.set_key3(key);
        case 3:
            Keyboard.set_key4(key);
        case 4:
            Keyboard.set_key5(key);
        case 5:
            Keyboard.set_key6(key);
    }
}

void send_packet(const KeyPacket packet)
{
    for(uint8_t i = 0; i < packet.keys.size(); ++i)
    {
        set_key(i, packet.keys[i]);
    }
    for(uint8_t i = packet.keys.size(); i < KEYS_PER_PACKET; ++i)
    {
        set_key(i, 0);
    }
    Keyboard.set_modifier(packet.modifiers);
    Keyboard.send_now();
}

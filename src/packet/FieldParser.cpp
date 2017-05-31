#include "FieldParser.hpp"

Field FieldParser::Parse(FieldType type, byte *data, size_t len) {
    switch (type) {
        case VarIntType:
            return ParseVarInt(data, len);
        case Boolean:
            return ParseBool(data);
        case String:
            return ParseString(data);
        case Long:
            return ParseLong(data);
        case Int:
            return ParseInt(data);
        case UnsignedByte:
            return ParseUByte(data);
        case Byte8_t:
            return ParseByte(data);
        case Float:
            return ParseFloat(data);
        case Position:
            return ParsePosition(data);
        case Double:
            return ParseDouble(data);
        case ByteArray:
            return ParseByteArray(data, len);
        default:
            throw 105;
    }
}

Field FieldParser::ParseString(byte *data) {
    Field fLen = ParseVarInt(data, 0);
    Field f;
    f.SetRaw(data, fLen.GetLength() + fLen.GetVarInt(), String);
    return f;
}

Field FieldParser::ParseBool(byte *data) {
    Field f;
    f.SetRaw(data, 1, Boolean);
    return f;
}

Field FieldParser::ParseVarInt(byte *data, size_t len) {
    if (len != 0) {
        Field f;
        f.SetRaw(data, len, VarIntType);
        return f;
    }
    int val = VarIntRead(data, len);
    Field f;
    f.SetVarInt(val);
    return f;
}

Field FieldParser::ParseLong(byte *data) {
    Field f;
    f.SetRaw(data, 8, Long);
    return f;
}

Field FieldParser::ParseInt(byte *data) {
    Field f;
    f.SetRaw(data, 4, Int);
    return f;
}

Field FieldParser::ParseUByte(byte *data) {
    Field f;
    f.SetRaw(data, 1, UnsignedByte);
    return f;
}

Field FieldParser::ParseByte(byte *data) {
    Field f;
    f.SetRaw(data, 1, Byte8_t);
    return f;
}

Field FieldParser::ParseFloat(byte *data) {
    Field f;
    f.SetRaw(data, 4, Float);
    return f;
}

Field FieldParser::ParsePosition(byte *data) {
    Field f;
    f.SetRaw(data, 8, Position);
    return f;
}

Field FieldParser::ParseDouble(byte *data) {
    Field f;
    f.SetRaw(data, 8, Double);
    return f;
}

Field FieldParser::ParseByteArray(byte *data, size_t len) {
    if (len == 0)
        throw 119;
    Field f;
    f.SetRaw(data, len, Byte8_t);
    return f;
}

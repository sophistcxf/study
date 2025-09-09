import person_pb2
import binascii
import struct
def analyze_serialized_data(data):
    """分析序列化后的二进制数据"""
    print("原始字节:", data)
    print("十六进制:", binascii.hexlify(data).decode('utf-8'))
    print("长度:", len(data), "字节")

    # 详细解析每个字段
    print("\n=== 详细解析 ===")
    index = 0
    while index < len(data):
        # 读取tag (第一个字节)
        tag_byte = data[index]
        index += 1

        # 解析tag: field_number << 3 | wire_type
        field_number = tag_byte >> 3
        wire_type = tag_byte & 0x07

        print(f"\nfield_num: {field_number}, wire_type: {wire_type}")

        if wire_type == 0:  # Varint
            # 读取Varint值
            value = 0
            shift = 0
            while True:
                byte = data[index]
                index += 1
                value |= (byte & 0x7F) << shift
                if not (byte & 0x80):
                    break
                shift += 7
            print(f"  Varint值: {value}")

        elif wire_type == 2:  # Length-delimited
            # 先读取长度 (Varint)
            length = 0
            shift = 0
            while True:
                byte = data[index]
                index += 1
                length |= (byte & 0x7F) << shift
                if not (byte & 0x80):
                    break
                shift += 7
            print(f"  长度: {length} 字节")

            # 读取实际数据
            string_data = data[index:index+length]
            print(f"  字符串值: '{string_data.decode('utf-8')}'")
            index += length
        elif wire_type == 5:    # 固定32位
            # 读取4字节
            value = data[index:index+4]
            index += 4
            print("  十六进制:", binascii.hexlify(value).decode('utf-8'))
            float_value = struct.unpack('<f', value)[0]
            print(f"  Fixed32值: {float_value}")
        elif wire_type == 1:  # 固定64位
            # 读取8字节
            value = data[index:index+8]
            index += 8
            print(f"  Fixed64值: {value}")
        else:
            print(f"  未知线类型: {wire_type}")
            break

def test1():
    # 创建Person对象并设置值
    person = person_pb2.Person()
    person.age = 25       # 字段1: int32
    person.id = 300       # 字段2: int64
    person.name = "Alice" # 字段3: string
    person.height = 1.68

    serialized_data = person.SerializeToString()

    print("=== 序列化分析 ===")
    analyze_serialized_data(serialized_data)

test1()
    
import heapq
import os

class HuffmanNode:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

#不断地合并堆中频率最小的节点，直到堆中只剩下一个节点，即为根节点，构建了整个哈夫曼树。
def build_huffman_tree(frequencies):
    heap = [HuffmanNode(char, freq) for char, freq in frequencies.items()]
    heapq.heapify(heap)

    while len(heap) > 1:
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)
        merged = HuffmanNode(None, left.freq + right.freq)
        merged.left = left
        merged.right = right
        heapq.heappush(heap, merged)

    return heap[0]

#构建哈夫曼树并生成哈夫曼编码的函数:通过递归遍历整个哈夫曼树，就可以生成每个字符对应的哈夫曼编码，并保存在 huffman_codes 字典中
def build_huffman_codes(node, current_code, huffman_codes):
    if node is None:
        return

    if node.char is not None:
        huffman_codes[node.char] = current_code
        return

    build_huffman_codes(node.left, current_code + '0', huffman_codes)
    build_huffman_codes(node.right, current_code + '1', huffman_codes)


def compress(input_file, output_file):
    with open(input_file, 'rb') as f:           #读输入文件
        data = f.read()
    frequencies = {}
    for byte in data:                           #统计每个字节出现的频率
        if byte not in frequencies:
            frequencies[byte] = 0
        frequencies[byte] += 1

    root = build_huffman_tree(frequencies)
    huffman_codes = {}
    build_huffman_codes(root, '', huffman_codes)

    compressed_data = ''
    for byte in data:
        compressed_data += huffman_codes[byte]

    padding = 8 - len(compressed_data) % 8      #二进制补足8的倍数位
    compressed_data += '0' * padding

    with open(output_file, 'wb') as f:
        # Write frequency information
        f.write(bytes([len(frequencies)]))
        for byte, freq in frequencies.items():
            f.write(bytes([byte, (freq >> 24) & 0xFF, (freq >> 16) & 0xFF, (freq >> 8) & 0xFF, freq & 0xFF]))

        # Write compressed data
        for i in range(0, len(compressed_data), 8):
            byte = compressed_data[i:i+8]
            f.write(bytes([int(byte, 2)]))

def decompress(input_file, output_file):
    with open(input_file, 'rb') as f:
        data = f.read()

    # 读取频率信息
    num_chars = data[0]
    frequencies = {}
    index = 1

    for _ in range(num_chars):
        char = data[index]
        freq = int.from_bytes(data[index+1:index+5], byteorder='big')
        frequencies[char] = freq
        index += 5

    # 根据频率信息构建哈夫曼树
    root = build_huffman_tree(frequencies)

    # 解压缩
    current_node = root
    decompressed_data = ""
    byte_str = ""

    for byte in data[index:]:
        byte_str += bin(byte)[2:].rjust(8, '0')

        while len(byte_str) >= 8:
            bit = int(byte_str[0])
            byte_str = byte_str[1:]

            if bit == 0:  # '0'
                current_node = current_node.left
            else:
                current_node = current_node.right

            if current_node.left is None and current_node.right is None:
                decompressed_data += chr(current_node.char)
                current_node = root

    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(decompressed_data)

# 指定压缩文件和解压缩文件的路径
compressed_file = 'compressed.bin'
decompressed_file = 'decompressed.txt'


if __name__ == "__main__":
    input_file = 'input.txt'
    compressed_file = 'compressed.bin'
    decompressed_file = 'decompressed.txt'



    # 解压缩文件
    decompress('compressed.bin', 'decompressed.txt')

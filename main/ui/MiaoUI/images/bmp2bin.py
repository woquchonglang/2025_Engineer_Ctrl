# https://javl.github.io/image2cpp/

# https://www.iconfont.cn/collections/detail?spm=a313x.collections_index.i1.d9df05512.69a13a819Oqgi6&cid=50900

from PIL import Image


def bmp_to_c_array(bmp_file, array_name):
    img = Image.open(bmp_file).convert("1")  # 转换为单色图像
    width, height = img.size
    data = list(img.getdata())

    c_array = f"const unsigned char {array_name}[] = {{\n"
    for i in range(height):
        row = data[i * width : (i + 1) * width]
        byte_data = []
        for j in range(0, len(row), 8):
            byte = 0
            for k in range(8):
                if j + k < len(row) and row[j + k] == 0:
                    byte |= 1 << (7 - k)
            byte_data.append(f"0x{byte:02X}")
        c_array += ", ".join(byte_data) + ",\n"
    c_array += "};\n"
    return c_array


# 示例用法
bmp_file = "main/ui/MiaoUI/images/logo.bmp"
array_name = "UI_NAME_LOGO"
c_array = bmp_to_c_array(bmp_file, array_name)
print(c_array)


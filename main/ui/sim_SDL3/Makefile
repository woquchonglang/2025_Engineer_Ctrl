# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -O2 -I/usr/include/SDL2 -I. -I./lib/MiaoUI -I./lib/MiaoUI/fonts -I./lib/MiaoUI/core -I./lib/MiaoUI/display -I./lib/MiaoUI/widget -I./lib/MiaoUI/images -I./lib/MiaoUI/indev -I./lib/u8g2 -I./src
# 链接选项
LDFLAGS = -L/usr/lib -lSDL2 -lm

# 目录配置
OBJ_DIR = obj
TARGET_DIR = bin

# 源文件收集
SRC_DIRS = ./lib/MiaoUI ./lib/MiaoUI/core ./lib/MiaoUI/display ./lib/MiaoUI/widget ./lib/MiaoUI/images ./lib/MiaoUI/indev ./lib/MiaoUI/fonts ./lib/u8g2
MAIN_SRC = sim.c
SRCS = $(wildcard $(addsuffix /*.c, $(SRC_DIRS))) $(MAIN_SRC)  # 修复源文件收集

# 目标文件生成规则（保留目录结构）
OBJ = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# 最终可执行文件
TARGET = $(TARGET_DIR)/sim

# 默认目标
all: $(TARGET)

# 链接规则
$(TARGET): $(OBJ)
	@mkdir -p "$(dir $@)"
	$(CC) -o $@ $^ $(LDFLAGS)

# 编译规则（处理子目录）
$(OBJ_DIR)/%.o: %.c
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) -c $< -o $@

# 清理规则
clean:
	rm -rf $(OBJ_DIR) $(TARGET_DIR)

.PHONY: all clean
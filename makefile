# 接受三个参数
# lang=c/c++/undefined
# path=子工程路径
# os=windows/linux

OS_CHARSET != \
	if [ '$(os)' = windows ]; then \
		echo GBK; \
	else \
		echo UTF-8; \
	fi
C_FLAGS = -Wall -g -static -static-libgcc -fexec-charset=$(OS_CHARSET) -std=c11
CXX_FLAGS = -Wall -g -static -static-libgcc -static-libstdc++ -fexec-charset=$(OS_CHARSET) -std=c++17
UNKNOWN_FLAGS = -Wall -g -static -fexec-charset=$(OS_CHARSET)

CC != \
	if [ '$(lang)' = c ]; then \
		echo gcc; \
	elif [ '$(lang)' = c++ ]; then \
		echo g++; \
	else \
		echo cc; \
	fi
FLAGS != \
	if [ '$(lang)' = c ]; then \
		echo $(C_FLAGS); \
	elif [ '$(lang)' = c++ ]; then \
		echo $(CXX_FLAGS); \
	else \
		echo UNKNOWN_FLAGS; \
	fi
SUFFIX != \
	if [ '$(lang)' = c ]; then \
		echo .c; \
	elif [ '$(lang)' = c++ ]; then \
		echo .cpp; \
	else \
		echo UNKNOWN_SUFFIX; \
	fi
OS != \
	if [ '$(os)' = windows ]; then \
		echo win; \
	elif [ '$(os)' = linux ]; then \
		echo linux; \
	else \
		echo unknown; \
	fi
# 以 /src 为坐标获取工作目录
CUR_DIR = $(shell pwd)
CWD = $(shell echo $(CUR_DIR) | sed 's/\/src.*//g')

# 输出到 build 路径
BUILD_PATH = $(CWD)/build
# 从 path 变量获取工程路径作为源文件所在路径
PROJSRC_PATH = $(subst \,/,$(path))
# 设置 VPATH 自动推导依赖关系的路径，用于 inplace 目标 main：在当前目录产生目标文件和可执行文件
VPATH = $(PROJSRC_PATH)
# 获取源文件列表，.c 和 .cpp 文件；原为 -type f \( -name '*.c' -o -name '*.cpp' \)
SOURCES := $(shell find $(PROJSRC_PATH) -maxdepth 1 -type f -name '*$(SUFFIX)' -exec basename {} \;)
# 将源文件列表转换为 .o 目标文件列表；原为 $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
OBJECTS := $(patsubst %$(SUFFIX),%.o,$(SOURCES))


# 通过 ".OS_工程目录" 作为名称的标志文件来判断是否仍在编译同一个工程，否则就要删除所有中间和目标文件防止 make 编译出错
PROJNAME = $(shell echo $(PROJSRC_PATH) | sed 's/.*\///')
PROJFLAG_PATH = $(BUILD_PATH)/.$(OS)_$(PROJNAME)
# BUILD_PATH 下没有 PROJNAME 文件则说明第一次编译该工程，必须先清理
EXEC != \
	if [ '$(wildcard $(PROJFLAG_PATH))' = '' ]; then \
		echo 1; \
		find $(BUILD_PATH) -type f -not -name ".git*" -delete; \
		cd $(BUILD_PATH) && touch .$(OS)_$(PROJNAME); \
		echo 0; \
	fi


# 默认目标 build/main.exe
# /path/to/build/main.exe: /path/to/build/main.o /path/to/build/dep.o ...
$(BUILD_PATH)/main: $(patsubst %, $(BUILD_PATH)/%, $(OBJECTS))
	$(CC) $(FLAGS) -o $@ $^
# 制作 .o 中间文件，在规则中使用 % 通配
$(BUILD_PATH)/%.o: $(PROJSRC_PATH)/%$(SUFFIX)
	$(CC) $(FLAGS) -o $@ -c $< -I $(PROJSRC_PATH)


# inplace main 目标
main: $(OBJECTS)


.PHONY: debug clean
debug:
	@echo CC, FLAGS, SUFFIX, OS: $(CC), $(FLAGS), $(SUFFIX), $(OS)
	@echo CUR_DIR, CWD, BUILD_PATH, PROJSRC_PATH: $(CUR_DIR), $(CWD), $(BUILD_PATH), $(PROJSRC_PATH)
	@echo PROJNAME, PROJFLAG_PATH, EXEC: $(PROJNAME), $(PROJFLAG_PATH), $(EXEC)
	@echo SOURCES: $(SOURCES)
	@echo OBJECTS: $(OBJECTS)

clean:
	find $(BUILD_PATH) -type f -not -name ".git*" -delete;
	cd $(CWD) && $(RM) main *.o *.exe;
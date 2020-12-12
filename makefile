cc = g++
prom = ./bin/main # 目标
deps = $(shell find ./src/ -name "*.h") # 找到所有 .h 文件
src = $(shell find ./src/ -name "*.cpp") # 找到所有 .cpp 文件
obj = $(src:%.cpp=%.o)  # 把所有 .cpp 换成 .o

$(prom): $(obj) # 编译主文件
	$(cc) -o $(prom) $(obj) -Wall -std=c++11 -g 

%.o: %.cpp $(deps) # 编译依赖项
	$(cc) -c $< -o $@ -Wall -std=c++11 -g 

clean: # 清理主文件
	rm -rf $(obj) $(prom)
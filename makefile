cc = g++-10
prom = ./bin/main # 目标
deps = $(shell find ./ -name "*.h") # 找到所有 .h 文件
src = $(shell find ./ -name "*.cpp") # 找到所有 .cpp 文件
obj = $(src:%.cpp=%.o)  # 把所有 .cpp 换成 .o

$(prom): $(obj) # 编译主文件
	$(cc) -o $(prom) $(obj)

%.o: %.cpp $(deps) # 编译依赖项
	$(cc) -c $< -o $@

clean: # 清理主文件
	rm -rf $(obj) $(prom)
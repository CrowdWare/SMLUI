CXX ?= c++
AR ?= ar

LIB = libSMLUI.a
SRCS = \
	src/sml_ui.cpp \
	imgui/imgui.cpp \
	imgui/imgui_demo.cpp \
	imgui/imgui_draw.cpp \
	imgui/imgui_tables.cpp \
	imgui/imgui_widgets.cpp \
	imgui/backends/imgui_impl_glfw.cpp \
	imgui/backends/imgui_impl_vulkan.cpp
OBJS = $(SRCS:.cpp=.o)

CXXFLAGS = -std=c++11 -Iinclude -Iimgui -Iimgui/backends -I../SMLParser/include -O2 -Wall -MMD -MP
CXXFLAGS += $(shell pkg-config --cflags glfw3 vulkan)
DEPS = $(OBJS:.o=.d)

all: $(LIB)

$(LIB): $(OBJS)
	$(AR) rcs $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

-include $(DEPS)

clean:
	rm -f $(LIB) $(OBJS) $(DEPS)

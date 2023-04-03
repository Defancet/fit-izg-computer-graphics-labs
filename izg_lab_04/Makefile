CXXFLAGS= -Iinclude/
LDFLAGS=-losg -losg -losgDB -losgViewer -losgGA -losgManipulator -losgUtil -lOpenThreads
BUILDDIR=build

OBJS:= IZG_MultiDragger.o IZG_RotateCylinderDragger.o IZG_Scale1DDragger.o IZG_TrackballDragger.o IZG_Translate1DDragger.o

OBJS:=$(addprefix $(BUILDDIR)/,$(OBJS))

TARGET=izg4

$(BUILDDIR)/$(TARGET): main.cpp Student.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

VPATH=src

$(BUILDDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $^ -c -o $@

run: $(BUILDDIR)/$(TARGET)
	./$<

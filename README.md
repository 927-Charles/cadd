# 项目简介：
本项目主要是定义了半边数据结构，在此基础上实现了基本的欧拉操作，进一步基于欧拉操作，实现了扫掠操作。为了更好地可视化结果，本项目借助OpenGL的Glut图形库依赖，实现了对构建模型的显示和渲染，同时，本项目实现了鼠标移动操作和模型结果输出功能，从而可以有效观察建模结果。本项目主要实现功能如下所示。

1）定义半边数据结构；

2）基于半边数据结构，实现基本欧拉操作；

3）基于欧拉操作，实现扫掠操作；

4）利用OpenGL实现对点和面的渲染；

5）实现了鼠标移动操作和模型结果打印输出功能

# 环境配置Requirements：

1)Visual Studio 2022

2)GLFW

3)CMake

4)GLAD

5)Glut / FreeGlut

# 程序结构

本程序是一个Visual Studio 2022的一个工程文件，在include文件夹集成了需要的库文件，包括自定义的头文件“half_edge_structure.h”。在该头文件中，实现了对于半边数据结构的构建，实现了5种欧拉操作以及扫掠操作。在源文件main.cpp中主要是实现了对模型的渲染显示，以及相应的可视化操作。
  
# 使用说明

本程序提供两个预设DEMO,DEMO1是利用欧拉操作实现带通孔立方体。DEMO2是先利用欧拉操作实现带通孔的二维平面，然后利用SWEEP扫掠生成模型。另外程序提供了USER函数供用户在函数中进行模型的构建。

1，用户交互函数

通过编写用户交互函数Interactive（），用户输入1则生成DEMO1，输入2则生成DEMO2，输入3则生成用户自定义模型，用户自定义需要用户在USER函数中添加。

2，鼠标移动操作函数和模型结果打印输出功能

为了让用户能够多角度观察生成出来的具体模型，本程序通过编写reshape函数，mouse函数以及move函数，实现通过鼠标对观察角度的修改。具体来说，与市面上的FPS游戏类似，用户按住鼠标即可拖动视角。
   
接下来是结果打印操作功能，在大规模构建模型的时候，有大量的点线面，对于用户而言，如果知道点线面的具体编号，以及其对应关系，则将有利于其进一步构建复杂模型。于是，本程序就在Solid类中添加私有函数print（），以实体solid为基础打印出与其相关的面，内环、外环。
 
3，用户自定义函数的编写

用户如果想自己构建模型，则需要进入user函数，进行欧拉操作的编写。本程序提供用户5个欧拉操作以及1个扫掠操作。
 
MVSF函数

实现功能：生成一个点，一个面，一个体，一般用于开始一步。

输入：生成新体的序号，生成新面的序号，生成新点的序号，生成新点的x,y,z坐标。

输出：指向新体的指针

MEV函数

实现功能：生成一条边和一个点

输入：体的序号，面的序号，原本点的序号，新的点序号，新的点的x,y,z坐标

输出：是否能正确生成

MEF函数

实现功能：生成一条边和一个面，新面与旧面通过新边连接

输入： 操作对应体的序号，操作对应面的序号，操作对应边的两个点的坐标，新生成面的序号。

输出： 是否能够正确生成

KEMR函数

实现功能：删除一边，生成一个环

输入： 操作对应体的序号，操作对应面的序号，操作对应边的两个点的坐标

输出： 是否能够正确生成

KFMRH函数

实现功能：删除一面，产生一个内环和通孔

输入： 操作对应体的序号，生成后应该带有内环面的序号，删去面的序号

输出： 无

SWEEP函数

实现功能：扫掠函数

输入： 扫掠面对应的体序号，扫掠面对应的序号，

输出： 无

4，修改图像渲染颜色

点渲染函数DrawVertex写在源文件main.cpp中，修改glColor3d中的参数即可修改其对应的颜色。

面渲染函数DrawFace写在源文件main.cpp中glColor3f中的参数即可修改其对应的颜色。

不管是glColor3d还是glColor3f，都需要三个参数，分别表示红色分量、绿色分量、蓝色分量。

5，修改图像界面

对于图像界面的修改则只需要对在main.cpp中的main函数进行相应的修改。

修改窗口的名称->修改glutCreateWindow的参数

修改窗口的位置->修改glutInitWindowPosition的参数

修改窗口的大小->修改glutInitWindowSize的参数
   
# 部分参考

1）LearnOpenGL CN

链接：https://learnopengl-cn.github.io/

2）CSDN上博客

链接：https://www.csdn.net/

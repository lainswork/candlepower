# **candlepower 烛光** 
###### 一个以 C++ 作为后端的软件收费管理和验证服务器
_网络验证服务器 AuthorizationServer_

## _序言_

>桌面软件的收费与管理一直以来都是困扰个人软件开发者的一个问题。\
个人软件开发者普遍以 C/C++,QT c++ ,易语言 等作为技术栈 并不善于网络编程\
桌面软件无法像网页 web 应用靠引流与推广作为有效营收手段\
一个桌面软件的核心就是收费系统，可靠的收费管理是个人软件开发者的核心生命力

> 当前商业网络验证系统虽然种类也较多，但质量良莠不齐、更新无动力、收费后无心售后等问题更加突出\
candlepower的主要目标是让众多个人软件开发者能极快以自己熟悉的方式(c/c++)来解决软件的运营问题，\
以将更多的精力用于对软件用户体验的打磨。在此之上倒逼商业网络验证提升服务质量。

---
## 现状
目前candlepower刚刚建立，功能暂时全部不可用，以完成部分包括 
> 前端: web主页 管理员及代理商登录页面\
> 后端: http rpc库完成，后期客户端调用服务端功能开发工作会十分便利

无意中走到这里的小伙伴请点个star ^_^

二进制安全、windows内核安全防护、游戏安全防护、web前后端技术交流、windows编程交流、C/C++ 技术交流。可以加群 749360521

---

## 基本介绍
前端使用的是开源前端库layui，目前layui官网已经关闭。虽然很惋惜，但是layui可以说是能让c/c++开发人员最快入手的前端框架
后端使用开源C++网络服务器框架[Dorgon](https://github.com/drogonframework/drogon)，它通用极优的性能和相对快捷的开发方式(与脚本语言和解释型语言相比当然没那么快...)\
Drogon拥有完善的服务器功能、极其详细的文档、异常活跃的社区和认证负责的作者，社区QQ群为1137909452

---
## 编译
candlepower是跨平台应用，所以在win平台和linux平台都可以编译使用，这却决于Drogon对各平台的支持\
candlepower使用cmake构建系统，在编译前请确保你已经安装了cmake，版本在3.5以上\
candlepower使用std c++17 标准，这也取决于Drogon
为了编译便捷，请使用vcpkg作为包管理器参考[教程](https://blog.csdn.net/cjmqas/article/details/79282847)以安装vcpkg
>在安装vcpkg过程中请注意:vcpkg最好安装在磁盘根目录，如 C:\ D:\ 下 ，使用vcpkg安装包时，最好使用VPN网络加速器\
将安装好的vcpkg目录加入到系统环境变量后，打开powershell或cmd，分别输入以下命令并回车(以windows为例)
```
./vcpkg.exe install fmt
```
```
./vcpkg.exe install fmt:x64-windows
```
```
./vcpkg.exe install drogon[ctl,mysql,orm,postgres,redis,sqlite3]
```
```
./vcpkg.exe install drogon[ctl,mysql,orm,postgres,redis,sqlite3]:x64-windows
```

之后克隆项目进行编译即可，编译教程后面会一点点完善

---
## 使用
构建完成后修改config.json中的配置项并将可执行文件及其依赖文件放到同一目录，将www文件夹和config.json拷贝至该目录，运行即可

使用教程后面会一点点完善

---
## 致谢
...
致谢以后再加

---
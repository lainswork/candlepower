# candlepower

打算写一个基于c++的网络验证(收费软件在线授权系统)
服务端 将会使用sogo/workflow来作为http服务器，后期可能单独增加一个websocket服务器来实现实时在线模式
客户端 主要面向windows 但是基本上可以保证跨平台有效
管理端/分销商 用imgui做前端界面然后通过http或者websocket访问服务端

服务、管理、分销 部分代码都需要vcpkg包管理器
客户端会尽量不依赖任何库
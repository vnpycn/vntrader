# VNPY官方 VNTrader （基于期货CTP接口专用量化交易开源软件）

#### 介绍
VNTrader是VNPY官方 http://www.vnpy.cn 推出的一款国内期货量化交易开源软件，
基于MIT开源协议，任何机构和个人可以免费下载和使用，无需付费。

基于CTP接口的开源性，打破收费软件垄断，采用VNTrader开源项目也可解决自己造轮子导致周期长门槛高的问题。
VNTrader是专门针对商品期货CTP接口的GUI窗口程序，支持多个Python策略组成策略池，支持回测，支持多周期量化交易。

VNTrader客户端开源代码 VNTrader是VNPY官方提供的CTP开源项目客户端源代码，
支持国内149家期货公司的CTP接入，
支持股指期货，股指期权、商品期货、商品期权的程序化交易和量化交易的仿真回测。

全新架构，性能再次升级，python的便捷,C++性能加持，比老版本更好用，性能提升300%以上，全新系统命名未VNTrader，属于VNPY官方发布的重点全新架构的产品。


https://gitee.com/vnpycn/vntrader
最近会有大的更新，VNTrader的底层C++代码很快会开放出来，目前开放Python部份，而且功能在整理，这个是一个有具大性能提升大版本
https://www.zhihu.com/org/vnpy/zvideos

VNPY官方网站 http://www.vnpy.cn 

VNPY知乎专栏 https://www.zhihu.com/org/vnpy/zvideos/

官方QQ群： 256163463

实盘支持（手续费很低）
http://www.kaihucn.cn

仿真账户支持 (仅工作日白天注册，支持股指期货、股指期权、商品期货、商品期权仿真交易)
http://www.simnow.com.cn

重点：
在未来 VNTrader 将继承http://www.virtualapi.cn 的强大功能，具体可以见 http://www.gucps.cn
完全不同于历史老版本，这个版本不仅性能优异，开源，而且结合C++的特点，结合底层仿真（获得国家发明专利）成为程序化交易最佳利器。

抛弃历史曾出现的大杂烩版本，专门面向国内商品期货、股指期货实现程序化交易CTP接口的专属版本，符合“精简、高性能、精细化回测、功能强大、入门更容易”等特点。


![Image text](https://raw.githubusercontent.com/vnpycn/vntrader/master/pic/vnpy.png)

![Image text](https://raw.githubusercontent.com/vnpycn/vntrader/master/pic/VNTrader.png)

第1步设置
![Image text](https://raw.githubusercontent.com/vnpycn/vntrader/master/pic/step1.png)


第2步设置
![Image text](https://raw.githubusercontent.com/vnpycn/vntrader/master/pic/step2.png)

#### 软件架构
软件架构说明
Python3.0 + PYQT
python下载

https://www.python.org/

Pycharm下载

https://www.jetbrains.com/pycharm/

默认英文版，可安装中文版本插件

注意：Python、IDE、VNTrader DLL模块必须一致，必须同时是32位或同时是64位。

支持Windows平台

#### 安装教程

1.  安装Python3.0
2.  安装Pycharm
3.  在Pycharm安装PyQT插件
4.  在Pycharm 中菜单 “运行”-> "运行"

#### 使用说明

VNTrader是VNPY官方 http://www.vnpy.cn 推出的一款国内期货量化交易开源软件，
主要支持CTP接口，支持国内149家期货公司程序化交易，实现程序化交易是免费的。
支持股指期货、商品期货、股指期权、商品期权，
支持中国8大合规交易所中的5所，包括上海期货交易所，大连期货交易所、
郑州期货交易所、中金所、能源所。

#### 目录说明：

strategy  策略存放目录

temp CTP接口产生的临时流文件存放目录

setting.ini 账户和服务器配置文件

thostmduserapi_se.dll  CTP接口原生行情接口；

thosttraderapi_se.dll    CTP接口原生交易接口；

vnctpmd.dll  CTP接口原生交易接口的代理库，用于和ctypes方式封装的CTPMarket.py 引用；

vnctptd.dll    CTP接口原生交易接口的代理库，用于和ctypes方式封装的CTPTrader 引用；

CTPMarket.py           Python ctypes 方式封装；

CTPTrader.py            Python ctypes 方式封装；

CTPMarketType.py    Python类型定义；

CTPTraderType.py     Python类型定义；

VNTrader.py  基于PyQT5的GUI程序；

#### 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request



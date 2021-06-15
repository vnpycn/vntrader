import threading
import logging
import os
import sys
from PyQt5 import QtWidgets, QtCore
# make the example runnable without the need to install
from os.path import abspath, dirname
sys.path.insert(0, abspath(dirname(abspath(__file__)) + '/..'))
import multiprocessing
curPath = os.path.abspath(os.path.dirname(__file__))
rootPath = os.path.split(curPath)[0]
sys.path.append(os.path.split(rootPath)[0])
print(sys.path)
import qdarkstyle
import ui.example_pyqt5_ui as example_ui
from PyQt5.QtGui import QIcon

# CTP行情库
from CTPMarket import *
# CTP交易库
from CTPTrader import *
# 导入时间库
import time
from threading import Thread

global ui
ui = example_ui.Ui_MainWindow()

# MyCTPTrade类继承自CTPTrader类
class MyCTPTrade(CTPTrader):
    # 登录回调
    def OnRspUserLogin(self,a):
        # print(a.contents.a1, a.contents.a2)
        print(u'交易登录成功OnRspUserLogin')
        #self.SubscribeMarketData('rb2110')
        log_todaytd('交易登录成功OnRspUserLogin')

    # 退出登录回调
    def OnRspUserLogout(self, a):
        print(a.contents.a1, a.contents.a2)
        print(u'交易登出成功OnRspUserLogout')
        log_todaytd('交易登出成功OnRspUserLogout')

    # 建立连接回调
    def OnFrontConnected(self):
        print("连接交易服务器成功OnFrontConnected")
        log_todaytd('连接交易服务器成功OnFrontConnected')

    # 断开连接回调
    def OnFrontDisconnected(self, a):
        print("断开与交易服务器连接OnFrontDisconnected")
        log_todaytd('断开与交易服务器连接OnFrontDisconnected')

# MyCTPMarket类继承自CTPMarket类
class MyCTPMarket(CTPMarket):
    # 行情回调
    def OnRtnDepthMarketData(self, a):
        print(a.InstrumentID)
        print(u'订阅合约成功OnRtnDepthMarketData')
        log_todaymd('订阅合约成功OnRtnDepthMarketData')

    # 合约订阅回调
    def OnRspSubMarketData(self, a):
        print(a.contents.a1, a.contents.a2)
        print(u'反订阅合约成功OnRspSubMarketData')
        log_todaymd('反订阅合约成功OnRspSubMarketData')

    # 登录回调
    def OnRspUserLogin(self,a):
        # print(a.contents.a1, a.contents.a2)
        print(u'行情登录成功OnRspUserLogin')
        log_todaymd('行情登录成功OnRspUserLogin')
        self.SubscribeMarketData(u'rb2110')
        # 订阅品种zn1610，接收Tick数据,不根据Tick生成其他周期价格数据,但可根据AddPeriod函数添加周期价格数据的设置

    # 退出登录回调
    def OnRspUserLogout(self, a):
        print(a.contents.a1, a.contents.a2)
        print(u'行情登出成功OnRspUserLogout')
        log_todaymd('行情登出成功OnRspUserLogout')
    # 建立连接回调
    def OnFrontConnected(self):
        print("连接行情服务器成功OnFrontConnected")
        log_todaymd('连接行情服务器成功OnFrontConnected')

    # 断开连接回调
    def OnFrontDisconnected(self, a):
        print("断开与行情服务器连接OnFrontDisconnected")
        log_todaymd('断开与行情服务器连接OnFrontDisconnected')


class RegTdThreadOnFrontConnected(Thread):
    def __init__(self, name, point):
        super().__init__()
        self.name = name
        self.point = point
    def run(self):  # 一定要叫这个名字，不能是别的
        print('td %s is running\n' % self.name)
        self.point.VNRegOnFrontConnected()

class RegTdThreadOnFrontDisconnected(Thread):
    def __init__(self, name, point):
        super().__init__()
        self.name = name
        self.point=point
    def run(self):  # 一定要叫这个名字，不能是别的
        print('td %s is running\n' % self.name)
        self.point.VNRegOnFrontDisconnected()

class RegTdThreadOnRspUserLogin(Thread):
    def __init__(self, name, point):
        super().__init__()
        self.name = name
        self.point=point
    def run(self):  # 一定要叫这个名字，不能是别的
        print('td %s is running\n' % self.name)
        self.point.VNRegOnRspUserLogin()

class RegTdThreadOnRspUserLogout(Thread):
    def __init__(self, name, point):
        super().__init__()
        self.name = name
        self.point=point
    def run(self):  # 一定要叫这个名字，不能是别的
        print('td %s is running\n' % self.name)
        self.point.VNRegOnRspUserLogout()


class RegTdThreadOnRspQryTradingAccount(Thread):
    def __init__(self, name, point):
        super().__init__()
        self.name = name
        self.point=point
    def run(self):  # 一定要叫这个名字，不能是别的
        print('td %s is running\n' % self.name)
        self.point.VNRegOnRspQryTradingAccount()
#---------------------------------------


class RegMdThreadOnFrontConnected(Thread):
    def __init__(self, name, point):
        super().__init__()
        self.name = name
        self.point = point
    def run(self):  # 一定要叫这个名字，不能是别的
        print('md %s is running\n' % self.name)
        self.point.VNRegOnFrontConnected()

class RegMdThreadOnFrontDisconnected(Thread):
    def __init__(self, name, point):
        super().__init__()
        self.name = name
        self.point=point
    def run(self):  # 一定要叫这个名字，不能是别的
        print('md %s is running\n' % self.name)
        self.point.VNRegOnFrontDisconnected()

#RegThreadTdOnRspUserLogin
class RegMdThreadOnRspUserLogin(Thread):
    def __init__(self, name, point):
        super().__init__()
        self.name = name
        self.point=point
    def run(self):  # 一定要叫这个名字，不能是别的
        print('md %s is running\n' % self.name)
        self.point.VNRegOnRspUserLogin()

#RegThreadTdOnRspUserLogin
class RegMdThreadOnRspUserLogout(Thread):
    def __init__(self, name, point):
        super().__init__()
        self.name = name
        self.point=point
    def run(self):  # 一定要叫这个名字，不能是别的
        print('md %s is running\n' % self.name)
        self.point.VNRegOnRspUserLogout()

class RegMdThreadOnRtnDepthMarketData(Thread):
    def __init__(self, name, point):
        super().__init__()
        self.name = name
        self.point=point
    def run(self):
        print('md %s is running\n' % self.name)
        self.point.VNRegOnRtnDepthMarketData()

def log_todaytd(mystr):
    _translate = QtCore.QCoreApplication.translate
    item = QtWidgets.QListWidgetItem()
    ui.Tdloglist.addItem(item)
    item = ui.Tdloglist.item(ui.Tdloglist.count() - 1)
    tstr = time.strftime("%Y-%m-%d %H:%M:%S ", time.localtime())
    item.setText(_translate("MainWindow", tstr + mystr))

def log_todaymd(mystr):
    _translate = QtCore.QCoreApplication.translate
    item = QtWidgets.QListWidgetItem()
    ui.Mdloglist.addItem(item)
    item = ui.Mdloglist.item(ui.Mdloglist.count() - 1)
    tstr = time.strftime("%Y-%m-%d %H:%M:%S ", time.localtime())
    item.setText(_translate("MainWindow", tstr + mystr))

def function_td(td, tname):
    RegTdThreadOnFrontConnected('OnFrontConnected', td).start()
    RegTdThreadOnFrontDisconnected('OnFrontDisconnected', td).start()
    RegTdThreadOnRspUserLogin('OnRspUserLogin', td).start()
    RegTdThreadOnRspUserLogout('OnRspUserLogout', td).start()
    RegTdThreadOnRspQryTradingAccount('OnRspQryTradingAccount', td).start()

    time.sleep(1)
    td.InitTD()
    #if td.InitTD() != 0:
    #    log_todaytd('初始化失败，请检查vnctptd.ini配置文件是否配置正确')
    # 调用交易接口元素，通过 “ 接口变量.元素（接口类内部定义的方法或变量） ” 形式调用
    # Login()，不需要参数，Login读取TD.ini的配置信息，并登录
    # 返回0， 表示登录成功，
    # 返回1， FutureTDAccount.ini错误
    # 返回2， 登录超时
    if td.ReqUserLogin() == 0:
        log_todaytd('发送登录交易服务器请求成功')
    else:
        log_todaytd('发送登录交易服务器请求失败')

    # 持仓数据在后台更新时，参数True为显示持仓状态，False为不显示持仓状态（仅对控制台有效）
    td.SetShowPosition(True)

    # 注意simnow模拟盘的交易服务器不稳定，经常会出现查询不到的情况。实盘账户绑定的交易服务器无此问题。

    while 1:  # 死循环，反复执行
        # 如果值为-999999999（初始值），则表示尚未获得数据
        print(u'(1)动态权益：%0.02f' % td.QryBalance(True))
        print(u'(2)静态权益：%0.02f' % td.QryBalance(False))
        print(u'(3)可用资金：%0.02f' % td.QryAvailable())
        print(u'(4)zn1701今日空单持仓：%d' % td.QryPosition('rb1701', VN_POSITION_Sell_Today))
        print(u'(5)zn1701今日多单持仓：%d' % td.QryPosition('rb1701', VN_POSITION_Buy_Today))
        print(u'(6)zn1701非今日空单持仓：%d' % td.QryPosition('rb1701', VN_POSITION_Sell_History))
        print(u'(7)zn1701非今日多单持仓：%d' % td.QryPosition('rb1701', VN_POSITION_Buy_History))

        print('--------------------------------------------------------')
        time.sleep(3)  # sleep1秒，防止死循环导致CPU占有率过高，1即可，不宜过大，若过大会导致程序进程长时间无响应，丢失行情数据

    pass

def function_md(md, tname):
    RegMdThreadOnFrontConnected('OnFrontConnected', md).start()
    RegMdThreadOnFrontDisconnected('OnFrontDisconnected', md).start()
    RegMdThreadOnRspUserLogin('OnRspUserLogin', md).start()
    RegMdThreadOnRspUserLogout('OnRspUserLogout', md).start()
    RegMdThreadOnRtnDepthMarketData('OnRtnDepthMarketData', md).start()
    time.sleep(1)
    if md.InitMD() != 0:
        log_todaymd('初始化失败，请检查vnctpmd.ini配置文件是否配置正确')
        return
    # md.RegisterFront()
    # Login()，不需要参数，Login读取QuickLibTD.ini的配置信息，并登录
    # 返回0， 表示登录成功，
    # 返回1， FutureTDAccount.ini错误
    # 返回2， 登录超时
    # 第一次是自动登录，退出登录后，如需再登陆，需添加以下登陆代码
    # 调用交易接口元素，通过 “ 接口变量.元素（接口类内部定义的方法或变量） ” 形式调用
    # Login()，不需要参数，Login读取QuickLibTD.ini的配置信息，并登录
    # 返回0， 表示登录成功，
    # 返回1， FutureTDAccount.ini错误
    # 返回2， 登录超时
    # print ('login: ', retLogin)

    #if md.Login() == 0:
    if md.ReqUserLogin() == 0:
        log_todaymd('发送登录行情服务器请求成功')
    else:
        log_todaymd('发送登录行情服务器请求失败')
    # 若登录成功，会触发 OnRspUserLogin 回调
    # 若登录失败，会触发 OnRspUserLogout 回调
    # 登录成功后，就可以执行情订阅等操作了，所以行情订阅放置在OnRspUserLogin()中执行

    # 设置拒绝接收行情服务器数据的时间，有时候（特别是模拟盘）在早晨6-8点会发送前一天的行情数据，若不拒收的话，会导致历史数据错误，本方法最多可以设置4个时间段进行拒收数据
    # md.SetRejectdataTime(0.0400, 0.0840, 0.1530, 0.2030, NULL, NULL, NULL, NULL);


    # main()函数内的以上部分代码只执行一次，以下while(1)循环内的代码，会一直循环执行，可在这个循环内需增加策略判断，达到下单条件即可下单

    # 判断当前时间是否在交易时间内，如果在返回真，则开始执行
    # if (IsStockTrade()):
    # 取消订阅
    # md.UnSubscribeMarketData(u'rb1810')
    # 退出登录
    # md.Logout()
    # md.SubscribeMarketData(u'rb2110')
    # time.sleep(1)  # 系统休眠0.1秒


    return
    # 重新订阅行情
    # md.Subcribe(u'rb1810')
    time.sleep(3)  # 系统休眠0.1秒
    while 1:  # 死循环，反复执行
        # print(md.LastPrice("rb2110"))
        # 打印该品种在行情接口的变量LastPrice (最新价)
        # print(md.BidPrice1('rb2110'))
        # 打印该品种在行情接口的变量BidPrice
        time.sleep(30)  # 系统休眠

    # else:
    # print u"没到开盘时间"



def main():
    """
    Application entry point
    """
    logging.basicConfig(level=logging.DEBUG)
    # create the application and the main window
    app = QtWidgets.QApplication(sys.argv)
    window = QtWidgets.QMainWindow()

    # setup ui
    #ui = example_ui.Ui_MainWindow()
    ui.setupUi(window)

    class MDThread(threading.Thread):
        def __init__(self ,tname):
            super(MDThread, self).__init__()
            # 重构run函数必须写
            self.tname = tname
        def run(self):
            md = MyCTPMarket()
            md.ui = ui
            function_md(md, self.tname)

    class TDThread(threading.Thread):
        def __init__(self, tname):
            super(TDThread, self).__init__()
            # 重构run函数必须写
            self.tname = tname
        def run(self):
            td = MyCTPTrade()
            td.ui = ui
            function_td(td,self.tname)

    """
    ui.bt_delay_popup.addActions([
        ui.Navgiate1,
        ui.Navgiate1_C
    ])
    """
    ui.menu_b_popup.addActions([
        ui.Navgiate1,
        ui.Navgiate1_C
    ])
    """
    ui.bt_menu_button_popup.addActions([
        ui.Navgiate1,
        ui.Navgiate1_C
    ])
    """

    item = QtWidgets.QTableWidgetItem("1")
    item.setCheckState(QtCore.Qt.Unchecked)

    # 进程数量，应该不大于CPU逻辑核心数
    processnum = multiprocessing.cpu_count()
    window.setWindowTitle("VNTrader (website: www.vnpy.cn) CPU逻辑核心数 :" + str(processnum))

    # tabify dock widgets to show bug #6
    window.tabifyDockWidget(ui.dockWidget1, ui.dockWidget2)

    # setup stylesheet
    print(qdarkstyle.load_stylesheet_pyqt5())
    app.setStyleSheet(qdarkstyle.load_stylesheet_pyqt5())



    # auto quit after 2s when testing on travis-ci
    if "--travis" in sys.argv:
        QtCore.QTimer.singleShot(2000, app.exit)

    timer = threading.Timer(10, ui.OnTimer_checkstranger)
    timer.start()

    # run
    window.resize(1068, 700)
    window.setWindowIcon(QIcon('vnpy.ico'))
    window.showMaximized()
    window.show()
    tt = TDThread('tt')
    tt.start()
    tm = MDThread('tm')
    tm.start()
    app.exec_()


if __name__ == "__main__":
    main()

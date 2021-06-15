# -*- coding=utf-8 -*-
import time
from CTPTraderType import *
from ctypes import *
# 官方网站：http://www.quicklib.cn
class CTPTrader(object):
    def __init__(self):
        self.vntd = CDLL('vnctptd.dll')
        self.fReqUserLogin = self.vntd.ReqUserLogin
        self.fReqUserLogin.argtypes = []
        self.fReqUserLogin.restype = c_int32

        self.fInsertOrder = self.vntd.InsertOrder
        self.fInsertOrder.argtypes = [c_char_p, c_char, c_char, c_char, c_double, c_int32]
        self.fInsertOrder.restype = c_int32

        self.fInsertOrderByRate = self.vntd.InsertOrderByRate
        self.fInsertOrderByRate.argtypes = [c_char_p, c_char, c_char, c_char, c_double, c_double, c_int32, c_int32]
        self.fInsertOrderByRate.restype = c_int32

        self.fDeleteOrder = self.vntd.DeleteOrder
        self.fDeleteOrder.argtypes = [c_char_p, c_int32]
        self.fDeleteOrder.restype = c_int32

        self.fQryTradedVol = self.vntd.QryTradedVol
        self.fQryTradedVol.argtypes = [c_int32]
        self.fQryTradedVol.restype = c_int32

        self.fQryPosition = self.vntd.QryPosition
        self.fQryPosition.argtypes = [c_char_p, c_int32]
        self.fQryPosition.restype = c_int32

        self.fQryPositionList = self.vntd.QryPositionList
        self.fQryPositionList.argtypes = [c_int32]
        self.fQryPositionList.restype = c_void_p

        self.fQryBalance = self.vntd.QryBalance
        self.fQryBalance.argtypes = [c_bool]
        self.fQryBalance.restype = c_double

        self.fQryAvailable = self.vntd.QryAvailable
        self.fQryAvailable.argtypes = []
        self.fQryAvailable.restype = c_double

        self.fSetShowPosition = self.vntd.SetShowPosition
        self.fSetShowPosition.argtypes = [c_bool]
        self.fSetShowPosition.restype = c_void_p

        self.fQryExchangeMarginRate = self.vntd.QryExchangeMarginRate
        self.fQryExchangeMarginRate.argtypes = [c_char_p, c_int32]
        self.fQryExchangeMarginRate.restype = c_double

        self.fQryUnderlyingMultiple = self.vntd.QryUnderlyingMultiple
        self.fQryUnderlyingMultiple.argtypes = [c_char_p]
        self.fQryUnderlyingMultiple.restype = c_double

        self.fQryQueryMaxOrderVolume = self.vntd.QryQueryMaxOrderVolume
        self.fQryQueryMaxOrderVolume.argtypes = [c_char_p, c_char_p, c_char_p, c_char, c_char, c_char, c_int32]
        self.fQryQueryMaxOrderVolume.restype = c_int32

        self.fInitTD = self.vntd.InitTD
        self.fInitTD.argtypes = []
        self.fInitTD.restype = c_int32

    def ReqUserLogin(self):
        return self.fReqUserLogin()

    def InsertOrder(self, InstrumentID, Direction, OffsetFlag, PriceType, Price, Num):
        return self.fInsertOrder(InstrumentID.encode('gb2312'), Direction.encode('gb2312'), OffsetFlag.encode('gb2312'), PriceType.encode('gb2312'),
                                 c_double(Price), c_int32(Num))

    # 根据资金比例下单
    def InsertOrderByRate(self, InstrumentID, Direction, OffsetFlag, PriceType, Price, Rate, BalanceType, Multiplier):
        return self.fInsertOrderByRate(InstrumentID.encode('gb2312'), Direction, OffsetFlag, PriceType,
                                       c_double(Price), Rate, BalanceType, Multiplier)

    def DeleteOrder(self, InstrumentID, OrderRef):
        return self.fDeleteOrder(InstrumentID.encode('gb2312'), OrderRef)

    def QryTradedVol(self, OrderRef):
        return self.fQryTradedVol(OrderRef)

    # 查询品种持仓
    def QryPosition(self, InstrumentID, Positiontype):
        return self.fQryPosition(InstrumentID.encode('gb2312'), Positiontype)

    # 查询持仓列表
    def QryPositionList(self, OrderRef):
        return self.fQryPositionList(OrderRef)

    # 查询权益   BalanceType=True动态权益    BalanceType=False静态权益
    def QryBalance(self, BalanceType):
        return self.fQryBalance(BalanceType)

    # 查询可用资金
    def QryAvailable(self):
        return self.fQryAvailable()

    # 设置更新持仓数据时显示,仅对控制台有效
    def SetShowPosition(self, Showstate):
        self.fSetShowPosition(Showstate)

    # 查询保证金比例
    def QryExchangeMarginRate(self, InstrumentID, Type):
        return self.fQryExchangeMarginRate(InstrumentID.encode('gb2312'), Type)

    # 查询合约乘数
    def QryUnderlyingMultiple(self, InstrumentID):
        return self.fQryUnderlyingMultiple(InstrumentID.encode('gb2312'))

    # 查询
    def QryQueryMaxOrderVolume(self, BrokerID, InvestorID, InstrumentID, Direction, OffsetFlag, HedgeFlag, MaxVolume):
        return self.fQryQueryMaxOrderVolume(BrokerID, InvestorID, InstrumentID, Direction, OffsetFlag, HedgeFlag,
                                            MaxVolume)




    #行情回调
    def OnRtnDepthMarketData(self, a):
        pass

    #合约订阅回调
    def OnRspSubMarketData(self, a):
        pass

    #登录回调
    def OnRspUserLogin(self, a):
        pass

    #退出登录回调
    def OnRspUserLogout(self, a):
        pass

    # 建立连接回调
    def OnFrontConnected(self):
        pass

    # 断开连接回调
    def OnFrontDisconnected(self, a):
        pass

    # 注册行情回调
    def VNRegOnRtnDepthMarketData(self):
        CMPFUNC = CFUNCTYPE(None, POINTER(VNCThostFtdcRspUserLoginField), c_void_p)
        self.vntd.VNRegOnRtnDepthMarketData(CMPFUNC(self.OnRtnDepthMarketData))

    # 注册Python的OnRspUserLogin回调函数指针，对应CTP c++的OnRspUserLogin方法
    def VNRegTdOnRspUserLogin(self):
        CMPFUNC = CFUNCTYPE(None, POINTER(VNCThostFtdcRspUserLoginField))
        self.vntd.VNRegOnRspUserLogin(CMPFUNC(self.OnRspUserLogin))

    # 注册Python的OnRspUserLogin回调函数指针，对应CTP c++的OnRspUserLogin方法
    def VNRegOnRspUserLogin(self):
        CMPFUNC = CFUNCTYPE(None, POINTER(VNCThostFtdcRspUserLoginField))
        self.vntd.VNRegOnRspUserLogin(CMPFUNC(self.OnRspUserLogin))

    # 注册Python的OnRspUserLogout回调函数指针，对应CTP c++的OnRspUserLogout方法
    def VNRegOnRspUserLogout(self):
        CMPFUNC = CFUNCTYPE(None, POINTER(VNCThostFtdcRspUserLoginField))
        self.vntd.VNRegOnRspUserLogout(CMPFUNC(self.OnRspUserLogout))

    # 注册Python的OnRspSubMarketData回调函数指针，对应CTP c++的OnRspSubMarketData方法
    def VNRegOnRspSubMarketData(self):
        CMPFUNC = CFUNCTYPE(None, POINTER(VNCThostFtdcRspUserLoginField))
        self.vntd.VNRegOnRspSubMarketData(CMPFUNC(self.OnRspSubMarketData))

    # 注册Python的OnFrontConnected回调函数指针，对应CTP c++的OnFrontConnected方法
    def VNRegOnFrontConnected(self):
        CMPFUNC = CFUNCTYPE(None)
        self.vntd.VNRegOnFrontConnected(CMPFUNC(self.OnFrontConnected))

    # 注册Python的OnFrontDisconnected回调函数指针，对应CTP c++的OnFrontDisconnected方法
    def VNRegOnFrontDisconnected(self):
        CMPFUNC = CFUNCTYPE(None, c_void_p)
        self.vntd.VNRegOnFrontDisconnected(CMPFUNC(self.OnFrontDisconnected))

    # 注册Python的OnRspQryTradingAccount回调函数指针，对应CTP c++的OnRspQryTradingAccount方法
    def VNRegOnRspQryTradingAccount(self):
        CMPFUNC = CFUNCTYPE(None, c_void_p)
        self.vntd.VNRegOnRspQryTradingAccount(CMPFUNC(self.OnFrontDisconnected))

    def InitTD(self):
        return self.fInitTD()
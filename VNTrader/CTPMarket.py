# -*- coding=utf-8 -*-
import time
from CTPMarketType import *
from ctypes import *
class CTPMarket(object):
    def __init__(self):
        self.vnmd = CDLL('vnctpmd.dll')
        """
        i = 0
        while (i < 5):
            time.sleep(1)
            if (self.vnmd.IsInitOK() == 0):
                i += 1
            else:
                break
        else:
            print('market init error')
            return
        """

        self.fReqUserLogin = self.vnmd.ReqUserLogin
        self.fReqUserLogin.argtypes = []
        self.fReqUserLogin.restype = c_int32

        self.fReqUserLogout = self.vnmd.ReqUserLogout
        self.fReqUserLogout.argtypes = []
        self.fReqUserLogout.restype = c_int32

        self.fUnSubscribeMarketData = self.vnmd.UnSubscribeMarketData
        self.fUnSubscribeMarketData.argtypes = [c_char_p]
        self.fUnSubscribeMarketData.restype = c_int32

        self.fGetApiVersion = self.vnmd.GetApiVersion
        self.fGetApiVersion.argtypes = []
        self.fGetApiVersion.restype = c_char_p

        self.fGetTradingDay = self.vnmd.GetTradingDay
        self.fGetTradingDay.argtypes = []
        self.fGetTradingDay.restype = c_char_p

        self.fRegisterFront = self.vnmd.RegisterFront
        self.fRegisterFront.argtypes = [c_char_p]

        self.fRegisterNameServer = self.vnmd.RegisterNameServer
        self.fRegisterNameServer.argtypes = [c_char_p]

        self.fSetPrintState = self.vnmd.SetPrintState
        self.fSetPrintState.argtypes = [c_bool]

        self.fGetData = self.vnmd.GetData
        self.fGetData.argtypes = [c_int32]
        self.fGetData.restype = c_void_p

        self.fSubscribeMarketData = self.vnmd.SubscribeMarketData
        self.fSubscribeMarketData.argtypes = [c_char_p]
        self.fSubscribeMarketData.restype = c_int32

        self.fSubscribeForQuoteRsp = self.vnmd.SubscribeForQuoteRsp
        self.fSubscribeForQuoteRsp.argtypes = [c_char_p]

        self.InstrumentNum = self.vnmd.GetInstrumentNum()

        self.fAddStopMonitor = self.vnmd.AddStopMonitor
        self.fAddStopMonitor.argtypes = [c_int32, c_int32, c_double]
        self.fAddStopMonitor.restype = c_int32

        self.fGetPeriodData = self.vnmd.GetPeriodData
        self.fGetPeriodData.argtypes = [c_char_p, c_int32, c_int32, c_int32]
        self.fGetPeriodData.restype = c_double

        self.fReadInstrument = self.vnmd.ReadInstrument
        self.fReadInstrument.argtypes = []
        self.fReadInstrument.restype = c_bool

        self.fInitMD = self.vnmd.InitMD
        self.fInitMD.argtypes = []

        self.fLog = self.vnmd.Log
        self.fLog.argtypes = [c_char_p, c_char_p]
        self.fLog.restype = c_void_p

        self.fSetRejectdataTime = self.vnmd.SetRejectdataTime
        self.fSetRejectdataTime.argtypes = [c_double, c_double, c_double, c_double, c_double, c_double, c_double,
                                            c_double]
        self.fSetRejectdataTime.restype = c_void_p

        self.fAddPeriod = self.vnmd.AddPeriod
        self.fAddPeriod.argtypes = [c_char_p, c_int32, c_bool]

        self.Index = dict()
        for i in range(self.InstrumentNum):
            data = self.fGetData(i)
            #data = cast(data, POINTER(sDepMarketData))
            self.Index[str(data[0].InstrumentID.encode('gb2312'))] = data
        pass

    def TradingDay(self, InstrumentID):
        # 交易日
        if InstrumentID.encode('gb2312') in self.Index:
            return self.Index[InstrumentID.encode('gb2312')][0].TradingDay
        else:
            return ''

    def InstrumentID(self, InstrumentID):
        # 合约代码
        if InstrumentID.encode('gb2312') in self.Index:
            return self.Index[InstrumentID.encode('gb2312')][0].InstrumentID
        else:
            return ''

    def ExchangeID(self, InstrumentID):
        # 交易所代码
        if InstrumentID.encode('gb2312') in self.Index:
            return self.Index[InstrumentID.encode('gb2312')][0].ExchangeID
        else:
            return ''

    def ExchangeInstID(self, InstrumentID):
        # 合约在交易所的代码
        if InstrumentID.encode('gb2312') in self.Index:
            return self.Index[InstrumentID.encode('gb2312')][0].ExchangeInstID
        else:
            return ''

    def LastPrice(self, InstrumentID):
        # 最新价
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].LastPrice, 1)
        else:
            return -1

    def PreSettlementPrice(self, InstrumentID):
        # 上次结算价
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].PreSettlementPrice, 1)
        else:
            return -1

    def PreClosePrice(self, InstrumentID):
        # 昨收盘
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].PreClosePrice, 1)
        else:
            return -1

    def PreOpenInterest(self, InstrumentID):
        # 昨持仓量
        if InstrumentID.encode('gb2312') in self.Index:
            return self.Index[InstrumentID.encode('gb2312')][0].PreOpenInterest
        else:
            return -1

    def OpenPrice(self, InstrumentID):
        # 今开盘
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].OpenPrice, 1)
        else:
            return -1

    def highestprice(self, InstrumentID):
        # 最高价
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].HighestPrice, 1)
        else:
            return -1

    def LowestPrice(self, InstrumentID):
        # 最低价
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].LowestPrice, 1)
        else:
            return -1

    def Volume(self, InstrumentID):
        # 数量
        if InstrumentID.encode('gb2312') in self.Index:
            return self.Index[InstrumentID.encode('gb2312')][0].Volume
        else:
            return -1

    def Turnover(self, InstrumentID):
        # 成交金额
        if InstrumentID.encode('gb2312') in self.Index:
            return self.Index[InstrumentID.encode('gb2312')][0].Turnover
        else:
            return -1

    def OpenInterest(self, InstrumentID):
        # 持仓量
        if InstrumentID.encode('gb2312') in self.Index:
            return self.Index[InstrumentID.encode('gb2312')][0].OpenInterest
        else:
            return -1

    def ClosePrice(self, InstrumentID):
        # 今收盘
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].ClosePrice, 1)
        else:
            return -1

    def SettlementPrice(self, InstrumentID):
        # 本次结算价
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].SettlementPrice, 1)
        else:
            return -1

    def UpperLimitPrice(self, InstrumentID):
        # 涨停板价
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].UpperLimitPrice, 1)
        else:
            return -1

    def LowerLimitPrice(self, InstrumentID):
        # 跌停板价
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].LowerLimitPrice, 1)
        else:
            return -1

    def PreDelta(self, InstrumentID):
        # 昨虚实度
        if InstrumentID.encode('gb2312') in self.Index:
            return self.Index[InstrumentID.encode('gb2312')][0].PreDelta
        else:
            return -1

    def CurrDelta(self, InstrumentID):
        # 今虚实度
        if InstrumentID.encode('gb2312') in self.Index:
            return self.Index[InstrumentID.encode('gb2312')][0].CurrDelta
        else:
            return -1

    def UpdateTime(self, InstrumentID):
        # 最后修改时间
        if InstrumentID.encode('gb2312') in self.Index:
            return self.Index[InstrumentID.encode('gb2312')][0].UpdateTime
        else:
            return ''

    def UpdateMillisec(self, InstrumentID):
        # 最后修改毫秒
        if InstrumentID.encode('gb2312') in self.Index:
            return self.Index[InstrumentID.encode('gb2312')][0].UpdateMillisec
        else:
            return 0

    def BidPrice1(self, InstrumentID):
        # 申买价一
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].BidPrice1, 1)
        else:
            return -1

    def BidVolume1(self, InstrumentID):
        # 申买量一
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].BidVolume1, 1)
        else:
            return -1

    def AskPrice1(self, InstrumentID):
        # 申卖价一
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].AskPrice1, 1)
        else:
            return -1

    def AskVolume1(self, InstrumentID):
        # 申卖量一
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].AskVolume1, 1)
        else:
            return -1

    def BidPrice2(self, InstrumentID):
        # 申买价二
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].BidPrice2, 1)
        else:
            return -1

    def BidVolume2(self, InstrumentID):
        # 申买量二
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].BidVolume2, 1)
        else:
            return None

    def AskPrice2(self, InstrumentID):
        # 申卖价二
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].AskPrice2, 1)
        else:
            return None

    def AskVolume2(self, InstrumentID):
        # 申卖量二
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].AskVolume2, 1)
        else:
            return None

    def BidPrice3(self, InstrumentID):
        # 申买价三
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].BidPrice3, 1)
        else:
            return None

    def BidVolume3(self, InstrumentID):
        # 申买量三
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].BidVolume3, 1)
        else:
            return None

    def AskPrice3(self, InstrumentID):
        # 申卖价三
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].AskPrice3, 1)
        else:
            return None

    def AskVolume3(self, InstrumentID):
        # 申卖量三
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].AskVolume3, 1)
        else:
            return None

    def BidPrice4(self, InstrumentID):
        # 申买价四
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].BidPrice4, 1)
        else:
            return None

    def BidVolume4(self, InstrumentID):
        # 申买量四
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].BidVolume4, 1)
        else:
            return None

    def AskPrice4(self, InstrumentID):
        # 申卖价四
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].AskPrice4, 1)
        else:
            return None

    def AskVolume4(self, InstrumentID):
        # 申卖量四
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].AskVolume4, 1)
        else:
            return None

    def BidPrice5(self, InstrumentID):
        # 申买价五
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].BidPrice5, 1)
        else:
            return None

    def BidVolume5(self, InstrumentID):
        # 申买量五
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].BidVolume5, 1)
        else:
            return None

    def AskPrice5(self, InstrumentID):
        # 申卖价五
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].AskPrice5, 1)
        else:
            return None

    def AskVolume5(self, InstrumentID):
        # 申卖量五
        if InstrumentID.encode('gb2312') in self.Index:
            return (self.Index[InstrumentID.encode('gb2312')][0].AskVolume5, 1)
        else:
            return None

    def AveragePrice(self, InstrumentID):
        # 当日均价
        if InstrumentID.encode('gb2312') in self.Index:
            return round(self.Index[InstrumentID.encode('gb2312')][0].AveragePrice, 1)
        else:
            return None

    # 订阅询价
    def SubscribeForQuoteRsp(self, InstrumentID):
        # 订阅合约时，请注意合约的大小写，中金所和郑州交易所是大写，上海和大连期货交易所是小写的
        self.fSubscribeForQuoteRsp(c_char_p(InstrumentID.encode('gb2312')))
        # data = self.fGetData(len(self.Index))
        # data = cast(data, POINTER(sDepMarketData))
        # self.Index[InstrumentID] = data

    def SubscribeMarketData(self, InstrumentID):
        # 订阅合约时，请注意合约的大小写，中金所和郑州交易所是大写，上海和大连期货交易所是小写的
        #return self.fSubscribeMarketData(InstrumentID)
        #return self.fSubscribeMarketData(c_char_p(bytes('rb2110', 'utf-8')))
        #return self.fSubscribeMarketData(c_char_p(InstrumentID.encode('gb2312')))
        pass
        #POINTER(VN_DepMarketData)
        # data = self.fGetData(len(self.Index))
        # data = cast(data, POINTER(sDepMarketData))
        # self.Index[InstrumentID] = data

    # def testMA():
    # MA测试
    # self.fTestMA()
    '''
    def MA(self, InstrumentID, periodtype, pricetype, ref, number):
        # MA测试
        return self.fMA(InstrumentID, periodtype, pricetype, ref, number)

    def CrossUp(self, InstrumentID, periodtype, pricetype, period1, period2):
        self.fCrossUp(InstrumentID, periodtype, pricetype, period1, period2)

    def CrossDown(self, InstrumentID, periodtype, pricetype, period1, period2):
        self.fCrossDown(InstrumentID, periodtype, pricetype, period1, period2)

        # 策略函数

    def CrossDown_s(self, strategyid, periodtype, pricetype, period1, period2):
        self.fCrossDown_s(strategyid, periodtype, pricetype, period1, period2)

    def AddPeriod(self, InstrumentID, periodtype):
        # 订阅合约时，请注意合约的大小写，中金所和郑州交易所是大写，上海和大连期货交易所是小写的
        self.fAddPeriod(InstrumentID, periodtype, True)
        # data = self.fGetData(len(self.Index))
        # data = cast(data, POINTER(sDepMarketData))
        # self.Index[InstrumentID] = data
    '''
    # def AddPeriod(self, InstrumentID):
    # MA测试
    # self.fAddPeriod(c_char_p(InstrumentID))
    # 设置止损监控
    # def AddStopMonitor(self, orderRef,Mode,percent):
    #   return self.fAddStopMonitor(orderRef,Mode,percent)

    # def GetPeriodData(self, InstrumentID,periodtype,pricetype,ref):
    #    return self.fGetPeriodData(InstrumentID,periodtype,pricetype,ref)

    # def SaveTick(self,index):
    #        self.fSaveTick(index)

    def ReadInstrumentIni(self):
        return self.fReadInstrument()

    def Log(self, content):
        return self.fLog(content.encode('gb2312'))

    def SetRejectdataTime(self, begintime1, endtime1, begintime2, endtime2, begintime3, endtime3, begintime4, endtime4):
        return self.fSetRejectdataTime(begintime1, endtime1, begintime2, endtime2, begintime3, endtime3, begintime4,
                                       endtime4)

    def ReqUserLogin(self):
        return self.fReqUserLogin()

    def Logout(self):
        return self.fReqUserLogout()

    def UnSubscribeMarketData(self, InstrumentID):
        return self.fUnSubscribeMarketData(InstrumentID.encode('gb2312'))

    def SetPrintState(self, printstate):
        return self.fSetPrintState(printstate)

    def GetApiVersion(self):
        return self.fGetApiVersion()

    def GetTradingDay(self):
        return self.fGetTradingDay()

    def RegisterFront(self, pszFrontAddress):
        self.fRegisterFront(pszFrontAddress.encode('gb2312'))

    def RegisterNameServer(self, pszNsAddress):
        self.fRegisterNameServer(pszNsAddress.encode('gb2312'))

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
        CMPFUNC = CFUNCTYPE(None, POINTER(VNDepMarketData), c_void_p)
        self.vnmd.VNRegOnRtnDepthMarketData(CMPFUNC(self.OnRtnDepthMarketData))

    # 注册Python的OnRspUserLogin回调函数指针，对应CTP c++的OnRspUserLogin方法
    def VNRegOnRspUserLogin(self):
        CMPFUNC = CFUNCTYPE(None, POINTER(VNCThostFtdcRspUserLoginField))
        self.vnmd.VNRegOnRspUserLogin(CMPFUNC(self.OnRspUserLogin))

    # 注册Python的OnRspUserLogout回调函数指针，对应CTP c++的OnRspUserLogout方法
    def VNRegOnRspUserLogout(self):
        CMPFUNC = CFUNCTYPE(None, POINTER(VNDepMarketData))
        self.vnmd.VNRegOnRspUserLogout(CMPFUNC(self.OnRspUserLogout))

    # 注册Python的OnRspSubMarketData回调函数指针，对应CTP c++的OnRspSubMarketData方法
    def VNRegOnRspSubMarketData(self):
        CMPFUNC = CFUNCTYPE(None, POINTER(VNDepMarketData))
        self.vnmd.VNRegOnRspSubMarketData(CMPFUNC(self.OnRspSubMarketData))

    # 注册Python的OnFrontConnected回调函数指针，对应CTP c++的OnFrontConnected方法
    def VNRegOnFrontConnected(self):
        CMPFUNC = CFUNCTYPE(None)
        self.vnmd.VNRegOnFrontConnected(CMPFUNC(self.OnFrontConnected))

    # 注册Python的OnFrontDisconnected回调函数指针，对应CTP c++的OnFrontDisconnected方法
    def VNRegOnFrontDisconnected(self):
        CMPFUNC = CFUNCTYPE(None, c_void_p)
        self.vnmd.VNRegOnFrontDisconnected(CMPFUNC(self.OnFrontDisconnected))

    def InitMD(self):
        self.fInitMD()
# -*- coding=utf-8 -*-
from ctypes import *


# 官方网站：http://www.quicklib.cn



class VN_char11(Structure):
    _fields_ = [('value', c_char * 11)]
    pass
class VNLoginInfo(Structure):
    _fields_ = [('brokeid', c_char * 9),
                ('investor', c_char * 31),
                ('password', c_char * 31),
                ('appid', c_char * 31),
                ('authcode',  c_char * 31),
                ('productInfo',  c_char * 31),
                ('adder1', c_char * 50),
                ('adder2', c_char * 50),
                ('adder3', c_char * 50)

                ]
    pass

class VNDepMarketData(Structure):
    _fields_ = [('TradingDay', c_char * 9),
                ('InstrumentID', c_char * 31),
                ('ExchangeID', c_char * 9),
                ('ExchangeInstID', c_char * 31),
                ('LastPrice', c_double),
                ('PreSettlementPrice', c_double),
                ('PreClosePrice', c_double),
                ('PreOpenInterest', c_double),
                ('OpenPrice', c_double),
                ('HighestPrice', c_double),
                ('LowestPrice', c_double),
                ('Volume', c_int),
                ('Turnover', c_double),
                ('OpenInterest', c_double),
                ('ClosePrice', c_double),
                ('SettlementPrice', c_double),
                ('UpperLimitPrice', c_double),
                ('LowerLimitPrice', c_double),
                ('PreDelta', c_double),
                ('CurrDelta', c_double),
                ('UpdateTime', c_char * 9),
                ('UpdateMillisec', c_int32),
                ('BidPrice1', c_double),
                ('BidVolume1', c_int32),
                ('AskPrice1', c_double),
                ('AskVolume1', c_int32),
                ('BidPrice2', c_double),
                ('BidVolume2', c_int32),
                ('AskPrice2', c_double),
                ('AskVolume2', c_int32),
                ('BidPrice3', c_double),
                ('BidVolume3', c_int32),
                ('AskPrice3', c_double),
                ('AskVolume3', c_int32),
                ('BidPrice4', c_double),
                ('BidVolume4', c_int32),
                ('AskPrice4', c_double),
                ('AskVolume4', c_int32),
                ('BidPrice5', c_double),
                ('BidVolume5', c_int32),
                ('AskPrice5', c_double),
                ('AskVolume5', c_int32),
                ('AveragePrice', c_double)
                ]
    pass


class VNCThostFtdcRspUserLoginField(Structure):
    _fields_ = [('TradingDay', c_char * 9),  # 交易日
                ('LoginTime', c_char * 9),  # 登录成功时间
                ('BrokerID', c_char * 11),  # 经纪公司代码
                ('UserID', c_char * 16),  # 用户代码
                ('SystemName', c_char * 41),  # 交易系统名称
                ('FrontID', c_int32),  # 前置编号
                ('SessionID', c_int32),  # 会话编号
                ('MaxOrderRef', c_char * 13),  # 最大报单引用
                ('SHFETime', c_char * 9),  # 上期所时间
                ('DCETime', c_char * 9),  # 大商所时间
                ('CZCETime', c_char * 9),  # 郑商所时间
                ('FFEXTime', c_char * 9),  # 中金所时间
                ('INETime', c_char * 9)  # 能源中心时间
                ]
    pass


class VNInstrument(Structure):
    _fields_ = [('InstrumentID', c_char * 31)]
    pass


class VN_CThostFtdcRspInfoField(Structure):
    _fields_ = [('ErrorID', c_int32),  # 错误代码
                ('ErrorMsg', c_char * 81)  # 错误信息
                ]
    pass


class VN_CThostFtdcForQuoteRspField(Structure):
    _fields_ = [('TradingDay', c_char * 9),  # 交易日
                ('InstrumentID', c_char * 31),  # 合约代码
                ('ForQuoteSysID', c_char * 21),  # 询价编号
                ('ForQuoteTime', c_char * 9),  # 询价时间
                ('ActionDay', c_char * 9),  # 业务日期
                ('ExchangeID', c_char * 9)  # 交易所代码
                ]
    pass


NULL = 100

# 周期定义
VN_ALL = 10000  # 所有周期
VN_M1 = 10001  # M1   1分钟
VN_M3 = 10002  # M3   3分钟
VN_M5 = 10003  # M5   5分钟
VN_M10 = 10004  # M10  10分钟
VN_M15 = 10005  # M15  15分钟
VN_M30 = 10006  # M30  30分钟
VN_M60 = 10007  # M60  60分钟
VN_M120 = 10008  # M120 120分钟
VN_D1 = 10009  # D1   1日

# PRICE TYPE
VN_CLOSE = 10010  # c_char('0')	# 收盘价
VN_OPEN = 10011  # c_char('1')	# 开盘价
VN_HIGH = 10012  # c_char('2')	# 最高价
VN_LOW = 10013  # c_char('3')	# 最低价

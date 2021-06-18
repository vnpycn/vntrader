# -*- coding=utf-8 -*-
from ctypes import *


class VNDEFTradingAccountField(Structure):
    _fields_ = [('BrokerID', c_char * 11),  # 经纪公司代码
                ('InvestorID', c_char * 13),  # 投资者代码
                ('Prebalance', c_double),  # 合约代码
                ('Current', c_double),  # 报单引用
                ('Available', c_double),  # 用户代码
                ('Rate', c_double),  # 报单价格条件
                ('Positionrate', c_double),  # 买卖方向
                ('TradingDay', c_char * 9)
                ]
    pass



class VNCThostFtdcOrderField(Structure):
    _fields_ = [('BrokerID', c_char * 11),  # 经纪公司代码
                ('InvestorID', c_char * 13),  # 投资者代码
                ('InstrumentID', c_char * 31),  # 合约代码
                ('OrderRef', c_char * 13),  # 报单引用
                ('UserID', c_char * 16),  # 用户代码
                ('OrderPriceType', c_char * 1),  # 报单价格条件
                ('Direction', c_char * 1),  # 买卖方向
                ('CombOffsetFlag', c_char * 5),  # 组合开平标志
                ('CombHedgeFlag', c_char * 5),  # 组合投机套保标志
                ('LimitPrice', c_double),  # 价格
                ('VolumeTotalOriginal', c_int),  # 数量
                ('TimeCondition', c_char * 1),  # 有效期类型
                ('GTDDate', c_char * 9),  # GTD日期
                ('VolumeCondition', c_char * 1),  # 成交量类型
                ('MinVolume', c_int),  # 最小成交量
                ('ContingentCondition', c_char * 1),  # 触发条件
                ('StopPrice', c_double),  # 止损价
                ('ForceCloseReason', c_char * 1),  # 强平原因
                ('IsAutoSuspend', c_int),  # 自动挂起标志
                ('BusinessUnit', c_char * 21),  # 业务单元
                ('RequestID', c_int),  # 请求编号
                ('OrderLocalID', c_char * 13),  # 本地报单编号
                ('ExchangeID', c_char * 9),  # 交易所代码
                ('ParticipantID', c_char * 11),  # 会员代码
                ('ClientID', c_char * 11),  # 客户代码
                ('ExchangeInstID', c_char * 31),  # 合约在交易所的代码
                ('TraderID', c_char * 21),  # 交易所交易员代码
                ('InstallID', c_int32),  # 安装编号
                ('OrderSubmitStatus', c_char * 1),  # 报单提交状态
                ('NotifySequence', c_int32),  # 报单提示序号
                ('TradingDay', c_char * 9),  # 交易日
                ('SettlementID', c_int32),  # 结算编号
                ('OrderSysID', c_char * 21),  # 报单编号
                ('OrderSource', c_char * 1),  # 报单来源
                ('OrderStatus', c_char * 1),  # 报单状态
                ('OrderType', c_char * 1),  # 报单类型
                ('VolumeTraded', c_int32),  # 今成交数量
                ('VolumeTotal', c_int32),  # 剩余数量
                ('InsertDate', c_char * 9),  # 报单日期
                ('InsertTime', c_char * 9),  # 委托时间
                ('ActiveTime', c_char * 9),  # 激活时间
                ('SuspendTime', c_char * 9),  # 挂起时间
                ('UpdateTime', c_char * 9),  # 最后修改时间
                ('CancelTime', c_char * 9),  # 撤销时间
                ('ActiveTraderID', c_char * 21),  # 最后修改交易所交易员代码
                ('ClearingPartID', c_char * 11),  # 结算会员编号
                ('SequenceNo', c_int32),  # 序号
                ('FrontID', c_int32),  # 前置编号
                ('SessionID', c_int32),  # 会话编号
                ('UserProductInfo', c_char * 11),  # 用户端产品信息
                ('StatusMsg', c_char * 81),  # 状态信息
                ('UserForceClose', c_int32),  # 用户强评标志
                ('ActiveUserID', c_char * 16),  # 操作用户代码
                ('BrokerOrderSeq', c_int32),  # 经纪公司报单编号
                ('RelativeOrderSysID', c_char * 21),  # 相关报单
                ('ZCETotalTradedVolume', c_int32),  # 郑商所成交数量
                ('IsSwapOrder', c_int32),  # 互换单标志
                ('BranchID', c_char * 9),  # 营业部编号
                ('InvestUnitID', c_char * 17),  # 投资单元代码
                ('AccountID', c_char * 13),  # 资金账号
                ('CurrencyID', c_char * 4),  # 币种代码
                ('IPAddress', c_char * 16),  # IP地址
                ('MacAddress', c_char * 21)  # Mac地址
                ]
    pass


class VNCThostFtdcSettlementInfoConfirmField(Structure):
    _fields_ = [('BrokerID', c_char * 11),  # 经纪公司代码
                ('InvestorID', c_char * 13),  # 投资者代码
                ('ConfirmDate', c_char * 9),  # 确认日期
                ('ConfirmTime', c_char * 9)  # 确认时间
                ]
    pass


class VNCThostFtdcQueryMaxOrderVolumeField(Structure):
    _fields_ = [('BrokerID', c_char * 11),  # 经纪公司代码
                ('InvestorID', c_char * 13),  # 投资者代码
                ('InstrumentID', c_char * 31),  # 合约代码
                ('Direction', c_char * 1),  # 买卖方向
                ('OffsetFlag', c_char * 1),  # 开平标志
                ('HedgeFlag', c_char * 1),  # 投机套保标志
                ('MaxVolume', c_int32),  # 最大允许报单数量
                ]
    pass


class VNCThostFtdcRspInfoField(Structure):
    _fields_ = [('ErrorID', c_int32),  # 错误代码
                ('ErrorMsg', c_char * 81)  # 错误信息
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


VN_Long = 0  # 买
VN_Short = 1  # 卖

# Direction or bsflag
VN_D_Buy = ('0')  # 买
VN_D_Sell = ('1')  # 卖

# offsetFlag
VN_OF_Open = ('0')  # 开仓
VN_OF_Close = c_wchar('1')  # 平仓
VN_OF_ForceClose = c_wchar('2')  # 强平
VN_OF_CloseToday = c_wchar('3')  # 平今
VN_OF_CloseYesterday = c_wchar('4')  # 平昨
VN_OF_ForceOff = c_wchar('5')  # 强减
VN_OF_LocalForceClose = c_wchar('6')  # 本地强平

# price type
VN_OPT_AnyPrice = c_wchar('1')  # 任意价
VN_OPT_LimitPrice = c_wchar('2')  # 限价
VN_OPT_BestPrice = c_wchar('3')  # 最优价
VN_OPT_LastPrice = c_wchar('4')  # 最新价
VN_OPT_LastPricePlusOneTicks = c_wchar('5')  # 最新价浮动上浮1个ticks
VN_OPT_LastPricePlusTwoTicks = c_wchar('6')  # 最新价浮动上浮2个ticks
VN_OPT_LastPricePlusThreeTicks = c_wchar('7')  # 最新价浮动上浮3个ticks
VN_OPT_AskPrice1 = c_wchar('8')  # 卖一价
VN_OPT_AskPrice1PlusOneTicks = c_wchar('9')  # 卖一价浮动上浮1个ticks
VN_OPT_AskPrice1PlusTwoTicks = c_wchar('A')  # 卖一价浮动上浮2个ticks
VN_OPT_AskPrice1PlusThreeTicks = c_wchar('B')  # 卖一价浮动上浮3个ticks
VN_OPT_BidPrice1 = c_wchar('C')  # 买一价
VN_OPT_BidPrice1PlusOneTicks = c_wchar('D')  # 买一价浮动上浮1个ticks
VN_OPT_BidPrice1PlusTwoTicks = c_wchar('E')  # 买一价浮动上浮2个ticks
VN_OPT_BidPrice1PlusThreeTicks = c_wchar('F')  # 买一价浮动上浮3个ticks

VN_Dynamic = 1  # 动态止损
VN_Static = 2  # 静态止损

VN_Dynamic_Capital = 0  # 动态止损
VN_Static_Capital = 1  # 静态止损

VN_POSITION_Sell_Today = 9001  # 今日空单
VN_POSITION_Buy_Today = 9002  # 今日多单
VN_POSITION_Sell_History = 9003  # 非今日空单
VN_POSITION_Buy_History = 9004  # 非今日多单
VN_POSITION_Sell_All = 9005  # 所有空单
VN_POSITION_Buy_All = 9006  # 所有多单

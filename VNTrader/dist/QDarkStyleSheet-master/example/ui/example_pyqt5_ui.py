# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'example.ui'
#
# Created by: PyQt5 UI code generator 5.7
#
# WARNING! All changes made in this file will be lost!

import webbrowser

import numpy as np
# K线图
import pyqtgraph as pg
from PyQt5 import QtWidgets
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from pyqtgraph import QtCore, QtGui


# K线图
class CandlestickItem(pg.GraphicsObject):
    def __init__(self, data):
        pg.GraphicsObject.__init__(self)
        self.data = data  ## data must have fields: time, open, close, min, max
        self.generatePicture()

    def generatePicture(self):
        self.picture = QtGui.QPicture()
        p = QtGui.QPainter(self.picture)
        p.setPen(pg.mkPen('w'))
        w = (self.data[1][0] - self.data[0][0]) / 3.
        for (t, open, close, min, max) in self.data:
            p.drawLine(QtCore.QPointF(t, min), QtCore.QPointF(t, max))
            if open > close:
                p.setBrush(pg.mkBrush('r'))
            else:
                p.setBrush(pg.mkBrush('g'))
            p.drawRect(QtCore.QRectF(t - w, open, w * 2, close - open))
        p.end()

    def paint(self, p, *args):
        p.drawPicture(0, 0, self.picture)

    def boundingRect(self):
        return QtCore.QRectF(self.picture.boundingRect())


data = [  ## 数据对应关系 (time, open, close, min, max).
    (1., 10, 13, 5, 15),
    (2., 13, 17, 9, 20),
    (3., 17, 14, 11, 23),
    (4., 14, 15, 5, 19),
    (5., 15, 9, 8, 22),
    (6., 9, 15, 8, 16),
    (7., 1.0, 1.3, 0.5, 1.5),  # 这里把官方示例的数据缩小10倍,为的是能明显开出K线的差别
    (8., 1.3, 1.7, 0.9, 2.0),
    (9., 1.7, 1.4, 1.1, 2.3),
    (10., 1.4, 1.5, 0.5, 1.9),
    (11., 1.5, 0.9, 0.8, 2.2),
    (12., 0.9, 1.5, 0.8, 1.6),
]

# 分时图
data2 = []
count = 0


# 对话框
class DialogOptionsWidget(QWidget):
    def __init__(self, parent=None):
        super(DialogOptionsWidget, self).__init__(parent)

    def addCheckBox(self, text, value):
        pass


class StandardDialog(QDialog):
    def __init__(self, parent=None):
        super(StandardDialog, self).__init__(parent)

        self.setWindowTitle("Standard Dialog")

        frameStyle = QFrame.Sunken | QFrame.Panel

        mainLayout = QVBoxLayout(self)
        toolbox = QToolBox()
        mainLayout.addWidget(toolbox)

        self.errorMessageDialog = QErrorMessage(self)

        pushButton_integer = QPushButton("QInputDialog.get&Int()")
        pushButton_double = QPushButton("QInputDialog.get&Double()")
        pushButton_item = QPushButton("QInputDialog.getIte&m()")
        pushButton_text = QPushButton("QInputDialog.get&Text()")
        pushButton_multiLineText = QPushButton("QInputDialog.get&MultiLineText()")
        pushButton_color = QPushButton("QColorDialog.get&Color()")
        pushButton_font = QPushButton("QFontDialog.get&Font()")
        pushButton_directory = QPushButton("QFileDialog.getE&xistingDirectory()")
        pushButton_openFileName = QPushButton("QFileDialog.get&OpenFileName()")
        pushButton_openFileNames = QPushButton("QFileDialog.&getOpenFileNames()")
        pushButton_saveFileName = QPushButton("QFileDialog.get&SaveFileName()")
        pushButton_critical = QPushButton("QMessageBox.critica&l()")
        pushButton_information = QPushButton("QMessageBox.i&nformation()")
        pushButton_question = QPushButton("QQMessageBox.&question()")
        pushButton_warning = QPushButton("QMessageBox.&warning()")
        pushButton_error = QPushButton("QErrorMessage.showM&essage()")

        self.label_integer = QLabel()
        self.label_double = QLabel()
        self.label_item = QLabel()
        self.label_text = QLabel()
        self.label_multiLineText = QLabel()
        self.label_color = QLabel()
        self.label_font = QLabel()
        self.label_directory = QLabel()
        self.label_openFileName = QLabel()
        self.label_openFileNames = QLabel()
        self.label_saveFileName = QLabel()
        self.label_critical = QLabel()
        self.label_information = QLabel()
        self.label_question = QLabel()
        self.label_warning = QLabel()
        self.label_error = QLabel()

        self.label_integer.setFrameStyle(frameStyle)
        self.label_double.setFrameStyle(frameStyle)
        self.label_item.setFrameStyle(frameStyle)
        self.label_text.setFrameStyle(frameStyle)
        self.label_multiLineText.setFrameStyle(frameStyle)
        self.label_color.setFrameStyle(frameStyle)
        self.label_font.setFrameStyle(frameStyle)
        self.label_directory.setFrameStyle(frameStyle)
        self.label_openFileName.setFrameStyle(frameStyle)
        self.label_openFileNames.setFrameStyle(frameStyle)
        self.label_saveFileName.setFrameStyle(frameStyle)
        self.label_critical.setFrameStyle(frameStyle)
        self.label_information.setFrameStyle(frameStyle)
        self.label_question.setFrameStyle(frameStyle)
        self.label_warning.setFrameStyle(frameStyle)
        self.label_error.setFrameStyle(frameStyle)

        page = QWidget()
        layout = QGridLayout(page)
        layout.setColumnStretch(1, 1)
        layout.setColumnMinimumWidth(1, 250)
        layout.addWidget(pushButton_integer, 0, 0)
        layout.addWidget(self.label_integer, 0, 1)
        layout.addWidget(pushButton_double, 1, 0)
        layout.addWidget(self.label_double, 1, 1)
        layout.addWidget(pushButton_item, 2, 0)
        layout.addWidget(self.label_item, 2, 1)
        layout.addWidget(pushButton_text, 3, 0)
        layout.addWidget(self.label_text, 3, 1)
        layout.addWidget(pushButton_multiLineText, 4, 0)
        layout.addWidget(self.label_multiLineText, 4, 1)
        layout.addItem(QSpacerItem(0, 0, QSizePolicy.Ignored, QSizePolicy.MinimumExpanding), 5, 0)
        toolbox.addItem(page, "Input Dialog")

        page = QWidget()
        layout = QGridLayout(page)
        layout.setColumnStretch(1, 1)
        layout.setColumnMinimumWidth(1, 250)
        layout.addWidget(pushButton_color, 0, 0)
        layout.addWidget(self.label_color, 0, 1)
        colorDialogOptionsWidget = DialogOptionsWidget()
        colorDialogOptionsWidget.addCheckBox("Do not use native dialog", QColorDialog.DontUseNativeDialog)
        colorDialogOptionsWidget.addCheckBox("Show alpha channel", QColorDialog.ShowAlphaChannel)
        colorDialogOptionsWidget.addCheckBox("No buttons", QColorDialog.NoButtons)
        layout.addItem(QSpacerItem(0, 0, QSizePolicy.Ignored, QSizePolicy.MinimumExpanding), 1, 0)
        layout.addWidget(colorDialogOptionsWidget, 2, 0, 1, 2)
        toolbox.addItem(page, "Color Dialog")

        page = QWidget()
        layout = QGridLayout(page)
        layout.setColumnStretch(1, 1)
        layout.addWidget(pushButton_font, 0, 0)
        layout.addWidget(self.label_font, 0, 1)
        fontDialogOptionsWidget = DialogOptionsWidget()
        fontDialogOptionsWidget.addCheckBox("Do not use native dialog", QFontDialog.DontUseNativeDialog)
        fontDialogOptionsWidget.addCheckBox("No buttons", QFontDialog.NoButtons)
        layout.addItem(QSpacerItem(0, 0, QSizePolicy.Ignored, QSizePolicy.MinimumExpanding), 1, 0)
        layout.addWidget(fontDialogOptionsWidget, 2, 0, 1, 2)
        toolbox.addItem(page, "Font Dialog")

        page = QWidget()
        layout = QGridLayout(page)
        layout.setColumnStretch(1, 1)
        layout.addWidget(pushButton_directory, 0, 0)
        layout.addWidget(self.label_directory, 0, 1)
        layout.addWidget(pushButton_openFileName, 1, 0)
        layout.addWidget(self.label_openFileName, 1, 1)
        layout.addWidget(pushButton_openFileNames, 2, 0)
        layout.addWidget(self.label_openFileNames, 2, 1)
        layout.addWidget(pushButton_saveFileName, 3, 0)
        layout.addWidget(self.label_saveFileName, 3, 1)
        fileDialogOptionsWidget = DialogOptionsWidget()
        fileDialogOptionsWidget.addCheckBox("Do not use native dialog", QFileDialog.DontUseNativeDialog)
        fileDialogOptionsWidget.addCheckBox("Show directories only", QFileDialog.ShowDirsOnly)
        fileDialogOptionsWidget.addCheckBox("Do not resolve symlinks", QFileDialog.DontResolveSymlinks)
        fileDialogOptionsWidget.addCheckBox("Do not confirm overwrite", QFileDialog.DontConfirmOverwrite)
        fileDialogOptionsWidget.addCheckBox("Do not use sheet", QFileDialog.DontUseSheet)
        fileDialogOptionsWidget.addCheckBox("Readonly", QFileDialog.ReadOnly)
        fileDialogOptionsWidget.addCheckBox("Hide name filter details", QFileDialog.HideNameFilterDetails)
        layout.addItem(QSpacerItem(0, 0, QSizePolicy.Ignored, QSizePolicy.MinimumExpanding), 4, 0)
        layout.addWidget(fileDialogOptionsWidget, 5, 0, 1, 2)
        toolbox.addItem(page, "File Dialogs")

        page = QWidget()
        layout = QGridLayout(page)
        layout.setColumnStretch(1, 1)
        layout.addWidget(pushButton_critical, 0, 0)
        layout.addWidget(self.label_critical, 0, 1)
        layout.addWidget(pushButton_information, 1, 0)
        layout.addWidget(self.label_information, 1, 1)
        layout.addWidget(pushButton_question, 2, 0)
        layout.addWidget(self.label_question, 2, 1)
        layout.addWidget(pushButton_warning, 3, 0)
        layout.addWidget(self.label_warning, 3, 1)
        layout.addWidget(pushButton_error, 4, 0)
        layout.addWidget(self.label_error, 4, 1)
        layout.addItem(QSpacerItem(0, 0, QSizePolicy.Ignored, QSizePolicy.MinimumExpanding), 5, 0)
        toolbox.addItem(page, "Message Boxes")

        pushButton_integer.clicked.connect(self.setInteger)
        pushButton_double.clicked.connect(self.setDouble)
        pushButton_item.clicked.connect(self.setItem)
        pushButton_text.clicked.connect(self.setText)
        pushButton_multiLineText.clicked.connect(self.setMultiLineText)
        pushButton_color.clicked.connect(self.setColor)
        pushButton_font.clicked.connect(self.setFont)
        pushButton_directory.clicked.connect(self.setExistingDirectory)
        pushButton_openFileName.clicked.connect(self.setOpenFileName)
        pushButton_openFileNames.clicked.connect(self.setOpenFileNames)
        pushButton_saveFileName.clicked.connect(self.setSaveFileName)
        pushButton_critical.clicked.connect(self.criticalMessage)
        pushButton_information.clicked.connect(self.informationMessage)
        pushButton_question.clicked.connect(self.questionMessage)
        pushButton_warning.clicked.connect(self.warningMessage)
        pushButton_error.clicked.connect(self.errorMessage)

    # 输入对话框 取整数

    def setInteger(self):
        intNum, ok = QInputDialog.getInt(self, "QInputDialog.getInteger()", "Percentage:", 25, 0, 100, 1)
        if ok:
            self.label_integer.setText(str(intNum))

    # 输入对话框 取实数

    def setDouble(self):
        doubleNum, ok = QInputDialog.getDouble(self, "QInputDialog.getDouble()", "Amount:", 37.56, -10000, 10000, 2)
        if ok:
            self.label_double.setText(str(doubleNum))

    # 输入对话框 取列表项

    def setItem(self):
        items = ["Spring", "Summer", "Fall", "Winter"]
        item, ok = QInputDialog.getItem(self, "QInputDialog.getItem()", "Season:", items, 0, False)
        if ok and item:
            self.label_item.setText(item)

    # 输入对话框 取文本

    def setText(self):
        text, ok = QInputDialog.getText(self, "QInputDialog.getText()", "User name:", QLineEdit.Normal,
                                        QDir.home().dirName())
        if ok and text:
            self.label_text.setText(text)

    # 输入对话框 取多行文本

    def setMultiLineText(self):
        text, ok = QInputDialog.getMultiLineText(self, "QInputDialog.getMultiLineText()", "Address:",
                                                 "John Doe\nFreedom Street")
        if ok and text:
            self.label_multiLineText.setText(text)

    # 颜色对话框 取颜色

    def setColor(self):
        # options = QColorDialog.ColorDialogOptions(QFlag.QFlag(colorDialogOptionsWidget.value()))
        color = QColorDialog.getColor(Qt.green, self, "Select Color")
        if color.isValid():
            self.label_color.setText(color.name())
            self.label_color.setPalette(QPalette(color))
            self.label_color.setAutoFillBackground(True)

    # 字体对话框 取字体

    def setFont(self):
        # options = QFontDialog.FontDialogOptions(QFlag(fontDialogOptionsWidget.value()))
        # font, ok = QFontDialog.getFont(ok, QFont(self.label_font.text()), self, "Select Font",options)
        font, ok = QFontDialog.getFont()
        if ok:
            self.label_font.setText(font.key())
            self.label_font.setFont(font)

    # 目录对话框 取目录


def setExistingDirectory(self):
    # options = QFileDialog.Options(QFlag(fileDialogOptionsWidget->value()))
    # options |= QFileDialog.DontResolveSymlinks | QFileDialog.ShowDirsOnly
    directory = QFileDialog.getExistingDirectory(self,
                                                 "QFileDialog.getExistingDirectory()",
                                                 self.label_directory.text())
    if directory:
        self.label_directory.setText(directory)

    # 打开文件对话框 取文件名


def setOpenFileName(self):
    # options = QFileDialog.Options(QFlag(fileDialogOptionsWidget.value()))
    # selectedFilter
    fileName, filetype = QFileDialog.getOpenFileName(self,
                                                     "QFileDialog.getOpenFileName()",
                                                     self.label_openFileName.text(),
                                                     "All Files (*);;Text Files (*.txt)")
    if fileName:
        self.label_openFileName.setText(fileName)

    # 打开文件对话框 取一组文件名


def setOpenFileNames(self):
    # options = QFileDialog.Options(QFlag(fileDialogOptionsWidget.value()))
    # selectedFilter
    openFilesPath = "D:/documents/pyMarksix/draw/"
    files, ok = QFileDialog.getOpenFileNames(self,
                                             "QFileDialog.getOpenFileNames()",
                                             openFilesPath,
                                             "All Files (*);;Text Files (*.txt)")

    if len(files):
        self.label_openFileNames.setText(", ".join(files))

    # 保存文件对话框 取文件名


def setSaveFileName(self):
    # options = QFileDialog.Options(QFlag(fileDialogOptionsWidget.value()))
    # selectedFilter
    fileName, ok = QFileDialog.getSaveFileName(self,
                                               "QFileDialog.getSaveFileName()",
                                               self.label_saveFileName.text(),
                                               "All Files (*);;Text Files (*.txt)")
    if fileName:
        self.label_saveFileName.setText(fileName)


def criticalMessage(self):
    # reply = QMessageBox.StandardButton()
    MESSAGE = "批评！"
    reply = QMessageBox.critical(self,
                                 "QMessageBox.critical()",
                                 MESSAGE,
                                 QMessageBox.Abort | QMessageBox.Retry | QMessageBox.Ignore)
    if reply == QMessageBox.Abort:
        self.label_critical.setText("Abort")
    elif reply == QMessageBox.Retry:
        self.label_critical.setText("Retry")
    else:
        self.label_critical.setText("Ignore")


def informationMessage(self):
    MESSAGE = "信息"
    reply = QMessageBox.information(self, "QMessageBox.information()", MESSAGE)
    if reply == QMessageBox.Ok:
        self.label_information.setText("OK")
    else:
        self.label_information.setText("Escape")


def questionMessage(self):
    MESSAGE = "疑问"
    reply = QMessageBox.question(self, "QMessageBox.question()",
                                 MESSAGE,
                                 QMessageBox.Yes | QMessageBox.No | QMessageBox.Cancel)
    if reply == QMessageBox.Yes:
        self.label_question.setText("Yes")
    elif reply == QMessageBox.No:
        self.label_question.setText("No")
    else:
        self.label_question.setText("Cancel")


def warningMessage(self):
    MESSAGE = "警告文本"
    msgBox = QMessageBox(QMessageBox.Warning,
                         "QMessageBox.warning()",
                         MESSAGE,
                         QMessageBox.Retry | QMessageBox.Discard | QMessageBox.Cancel,
                         self)
    msgBox.setDetailedText("详细信息。。。")
    # msgBox.addButton("Save &Again", QMessageBox.AcceptRole)
    # msgBox.addButton("&Continue", QMessageBox.RejectRole)
    if msgBox.exec() == QMessageBox.AcceptRole:
        self.label_warning.setText("Retry")
    else:
        self.label_warning.setText("Abort")


def errorMessage(self):
    self.errorMessageDialog.showMessage(
        "This dialog shows and remembers error messages. "
        "If the checkbox is checked (as it is by default), "
        "the shown message will be shown again, "
        "but if the user unchecks the box the message "
        "will not appear again if QErrorMessage.showMessage() "
        "is called with the same message.")
    self.label_error.setText("If the box is unchecked, the message "
                             "won't appear again.")


class Ui_MainWindow(object):
    def OnStart(self):
        self.Button_Start.setEnabled(False)
        self.Button_Stop.setEnabled(True)

    def OnStop(self):
        self.Button_Start.setEnabled(True)
        self.Button_Stop.setEnabled(False)

    global logname
    logname = u'20210601'

    def OpenLog_TD(self):
        import subprocess as sp
        programName = "notepad.exe"
        fileName = "log//td//" + logname + ".txt"
        sp.Popen([programName, fileName])
        pass

    def OpenLog_MD(self):
        import subprocess as sp
        programName = "notepad.exe"
        fileName = "log//md//" + logname + ".txt"
        sp.Popen([programName, fileName])
        pass

    def OpenUrl_VNPY(self):
        webbrowser.open('http://www.vnpy.cn/')

    def OpenUrl_ZHIHU(self):
        webbrowser.open('https://www.zhihu.com/org/vnpy/zvideos/')

    def OpenUrl_COOLQUANT(self):
        webbrowser.open('http://www.coolquant.cn/')

    def OpenUrl_KAIHU(self):
        webbrowser.open('http://www.kaihucn.cn/')

    def double_trade(self):
        '''
         作用：双击事件监听，显示被选中的单元格
        '''
        # 打印被选中的单元格
        for i in self.table_account.selectedItems():
            print(i.row(), i.column(), i.text())
            self.setWindowTitle(i.text())

    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")

        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.verticalLayout_5 = QtWidgets.QVBoxLayout(self.centralwidget)
        self.verticalLayout_5.setObjectName("verticalLayout_5")
        self.tabWidget = QtWidgets.QTabWidget(self.centralwidget)
        self.tabWidget.setTabPosition(QtWidgets.QTabWidget.East)
        self.tabWidget.setTabsClosable(True)
        self.tabWidget.setObjectName("tabWidget")
        self.tab = QtWidgets.QWidget()
        self.tab.setObjectName("tab")
        self.gridLayout = QtWidgets.QGridLayout(self.tab)
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.groupBox = QtWidgets.QGroupBox(self.tab)
        self.groupBox.setObjectName("groupBox")
        self.verticalLayout_3 = QtWidgets.QVBoxLayout(self.groupBox)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.toolBox1 = QtWidgets.QToolBox(self.groupBox)
        # 组标题 toolBox1
        self.toolBox1.setObjectName("")

        self.toolBox2 = QtWidgets.QToolBox(self.groupBox)
        self.toolBox2.setObjectName("toolBox2")

        self.page_order = QtWidgets.QWidget()
        self.page_order.setGeometry(QtCore.QRect(0, 0, 98, 44))
        self.page_order.setObjectName("page_order")
        self.gridLayout_4 = QtWidgets.QGridLayout(self.page_order)
        self.gridLayout_4.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_4.setObjectName("gridLayout_4")
        """
        #1，1 委托记录
        self.lineEdit = QtWidgets.QLineEdit(self.page_order)
        self.lineEdit.setObjectName("lineEdit")
        self.gridLayout_4.addWidget(self.lineEdit, 0, 0, 1, 1)
        
        
        """
        self.table_order = QtWidgets.QTableWidget(self.page_order)
        self.table_order.setObjectName("table_Order")
        self.table_order.verticalHeader().setVisible(False)  # 隐藏垂直表头
        self.table_order.horizontalHeader().setVisible(True)  # 隐藏水平表头
        self.table_order.setColumnCount(8)
        self.table_order.setRowCount(4)
        self.table_order.setHorizontalHeaderLabels(['id', '合约', '方向', '学号', '地址'])  # 设置表头文字
        self.table_order.setSortingEnabled(True)  # 设置表头可以自动排序

        """
        item = QtWidgets.Qtable_accountItem()
        self.table_order.setVerticalHeaderItem(0, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_order.setVerticalHeaderItem(1, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_order.setVerticalHeaderItem(2, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_order.setVerticalHeaderItem(3, item)


        item = QtWidgets.Qtable_accountItem()
        self.table_order.setHorizontalHeaderItem(0, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_order.setHorizontalHeaderItem(1, item)
        """

        self.gridLayout_4.addWidget(self.table_order, 0, 0, 1, 1)

        self.toolBox1.addItem(self.page_order, "")

        self.page_instrument = QtWidgets.QWidget()
        self.page_instrument.setGeometry(QtCore.QRect(0, 0, 98, 44))
        self.page_instrument.setObjectName("page_instrument")
        self.gridLayout_4 = QtWidgets.QGridLayout(self.page_instrument)
        self.gridLayout_4.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_4.setObjectName("gridLayout_4")
        """
        #2，1 委托记录
        self.lineEdit = QtWidgets.QLineEdit(self.page_instrument)
        self.lineEdit.setObjectName("lineEdit")
        self.gridLayout_4.addWidget(self.lineEdit, 0, 0, 1, 1)
        """
        self.table_instrument = QtWidgets.QTableWidget(self.page_instrument)
        self.table_instrument.setObjectName("table_Order")
        self.table_instrument.verticalHeader().setVisible(False)  # 隐藏垂直表头
        self.table_instrument.horizontalHeader().setVisible(True)  # 隐藏水平表头
        self.table_instrument.setEditTriggers(self.table_instrument.NoEditTriggers)
        self.table_instrument.setColumnCount(8)
        self.table_instrument.setRowCount(4)
        self.table_instrument.setHorizontalHeaderLabels(['id', '合约', '方向', '学号', '地址'])  # 设置表头文字
        self.table_instrument.setSelectionBehavior(self.table_instrument.SelectRows)
        self.table_instrument.setSortingEnabled(True)  # 设置表头可以自动排序

        """
        item = QtWidgets.Qtable_accountItem()
        self.table_instrument.setVerticalHeaderItem(0, item)
        #item = QtWidgets.Qtable_accountItem()
        self.table_instrument.setVerticalHeaderItem(1, item)
        #item = QtWidgets.Qtable_accountItem()
        self.table_instrument.setVerticalHeaderItem(2, item)
        #item = QtWidgets.Qtable_accountItem()
        self.table_instrument.setVerticalHeaderItem(3, item)

        item = QtWidgets.Qtable_accountItem()
        self.table_instrument.setHorizontalHeaderItem(0, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_instrument.setHorizontalHeaderItem(1, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_instrument.setHorizontalHeaderItem(2, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_instrument.setHorizontalHeaderItem(3, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_instrument.setHorizontalHeaderItem(4, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_instrument.setHorizontalHeaderItem(5, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_instrument.setHorizontalHeaderItem(6, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_instrument.setHorizontalHeaderItem(7, item)

        item = self.table_instrument.horizontalHeaderItem(0).setText("合约")
        item = self.table_instrument.horizontalHeaderItem(1).setText("方向")
        item = self.table_instrument.horizontalHeaderItem(2).setText("数量")
        item = self.table_instrument.horizontalHeaderItem(3).setText("时间1")
        item = self.table_instrument.horizontalHeaderItem(4).setText("时间2")
        item = self.table_instrument.horizontalHeaderItem(5).setText("时间3")
        item = self.table_instrument.horizontalHeaderItem(6).setText("时间4")
        item = self.table_instrument.horizontalHeaderItem(7).setText("时间5")
        """
        self.gridLayout_4.addWidget(self.table_instrument, 0, 0, 1, 1)

        self.toolBox2.addItem(self.page_instrument, "")

        self.page_trade = QtWidgets.QWidget()
        self.page_trade.setGeometry(QtCore.QRect(0, 0, 697, 210))
        self.page_trade.setObjectName("page_trade")
        self.gridLayout_5 = QtWidgets.QGridLayout(self.page_trade)
        self.gridLayout_5.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_5.setObjectName("gridLayout_5")
        self.listWidget = QtWidgets.QListWidget(self.page_trade)
        self.listWidget.setObjectName("listWidget")
        """
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        """
        self.table_trade = QtWidgets.QTableWidget(self.page_trade)
        self.table_trade.setObjectName("table_trade")
        self.table_trade.verticalHeader().setVisible(False)  # 隐藏垂直表头
        self.table_trade.horizontalHeader().setVisible(True)  # 隐藏水平表头
        self.table_trade.setEditTriggers(self.table_trade.NoEditTriggers)
        self.table_trade.doubleClicked.connect(self.double_trade)
        self.table_trade.setColumnCount(8)
        self.table_trade.setRowCount(4)
        self.table_trade.setHorizontalHeaderLabels(['id', '合约', '方向', '学号', '地址'])  # 设置表头文字
        self.table_trade.setSelectionBehavior(self.table_trade.SelectRows)
        self.table_trade.setSortingEnabled(True)  # 设置表头可以自动排序
        """
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setVerticalHeaderItem(0, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setVerticalHeaderItem(1, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setVerticalHeaderItem(2, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setVerticalHeaderItem(3, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setVerticalHeaderItem(4, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setVerticalHeaderItem(5, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setVerticalHeaderItem(6, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setVerticalHeaderItem(7, item)

        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setHorizontalHeaderItem(0, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setHorizontalHeaderItem(1, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setHorizontalHeaderItem(2, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setHorizontalHeaderItem(3, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setHorizontalHeaderItem(4, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setHorizontalHeaderItem(5, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setHorizontalHeaderItem(6, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_trade.setHorizontalHeaderItem(7, item)

        item = self.table_trade.horizontalHeaderItem(0).setText("合约")
        item = self.table_trade.horizontalHeaderItem(1).setText("方向")
        item = self.table_trade.horizontalHeaderItem(2).setText("数量")
        item = self.table_trade.horizontalHeaderItem(3).setText("时间1")
        item = self.table_trade.horizontalHeaderItem(4).setText("时间2")
        item = self.table_trade.horizontalHeaderItem(5).setText("时间3")
        item = self.table_trade.horizontalHeaderItem(6).setText("时间4")
        item = self.table_trade.horizontalHeaderItem(7).setText("时间5")
        """
        self.gridLayout_5.addWidget(self.table_trade, 1, 1, 1, 1)

        # self.gridLayout_5.addWidget(self.listWidget, 0, 0, 1, 1)

        self.page_position = QtWidgets.QWidget()
        self.page_position.setGeometry(QtCore.QRect(0, 0, 697, 210))
        self.page_position.setObjectName("page_position")
        self.gridLayout_5 = QtWidgets.QGridLayout(self.page_position)
        self.gridLayout_5.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_5.setObjectName("gridLayout_5")
        """
        self.listWidget = QtWidgets.QListWidget(self.page_position)
        self.listWidget.setObjectName("listWidget")
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        item = QtWidgets.QListWidgetItem()
        self.listWidget.addItem(item)
        """

        self.table_position = QtWidgets.QTableWidget(self.page_trade)
        self.table_position.setObjectName("table_trade")
        self.table_position.verticalHeader().setVisible(False)  # 隐藏垂直表头
        self.table_position.horizontalHeader().setVisible(True)  # 隐藏水平表头
        self.table_position.setEditTriggers(self.table_position.NoEditTriggers)
        self.table_position.setColumnCount(8)
        self.table_position.setRowCount(4)
        self.table_position.setHorizontalHeaderLabels(['id', '合约', '方向', '数量', '交易时间', '订单编号', '操作', '备注'])  # 设置表头文字
        # 设置行高
        self.table_position.setRowHeight(0, 35)
        self.table_position.setSortingEnabled(True)  # 设置表头可以自动排序
        # self.table_position.sortItems(1,  Qt.DescendingOrder)  # 设置按照第二列自动降序排序
        self.table_position.horizontalHeader().setStretchLastSection(True)  ##设置最后一列拉伸至最大
        # self.table_position.setItemDelegateForColumn(0,  self )  # 设置第0列不可编辑

        # TOdo 优化7 在单元格内放置控件
        """
        comBox=QtWidgets.QComboBox()
        comBox.addItems(['卖','买'])
        comBox.setStyleSheet('QComboBox{margin:3px}')
        self.table_position.setCellWidget(0,1,comBox)
        """
        Trade_CancelBtn = QtWidgets.QPushButton('双击人工平仓')
        Trade_CancelBtn.setFlat(True)
        Trade_CancelBtn.setStyleSheet('background-color:#ff0000;');
        # searchBtn.setDown(True)
        Trade_CancelBtn.setStyleSheet('QPushButton{margin:3px}')
        self.table_position.setCellWidget(0, 6, Trade_CancelBtn)

        """
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setVerticalHeaderItem(0, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setVerticalHeaderItem(1, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setVerticalHeaderItem(2, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setVerticalHeaderItem(3, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setVerticalHeaderItem(4, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setVerticalHeaderItem(5, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setVerticalHeaderItem(6, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setVerticalHeaderItem(7, item)
 
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setHorizontalHeaderItem(0, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setHorizontalHeaderItem(1, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setHorizontalHeaderItem(2, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setHorizontalHeaderItem(3, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setHorizontalHeaderItem(4, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setHorizontalHeaderItem(5, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setHorizontalHeaderItem(6, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_position.setHorizontalHeaderItem(7, item)
        item = self.table_position.horizontalHeaderItem(0).setText("合约")
        item = self.table_position.horizontalHeaderItem(1).setText("方向")
        item = self.table_position.horizontalHeaderItem(2).setText("数量")
        item = self.table_position.horizontalHeaderItem(3).setText("时间")
        item = self.table_position.horizontalHeaderItem(4).setText("备注1")
        item = self.table_position.horizontalHeaderItem(5).setText("备注2")
        item = self.table_position.horizontalHeaderItem(6).setText("备注3")
        item = self.table_position.horizontalHeaderItem(7).setText("备注4")
        """
        self.gridLayout_5.addWidget(self.table_position, 1, 1, 1, 1)

        self.toolBox1.addItem(self.page_trade, "")
        self.toolBox2.addItem(self.page_position, "")

        self.verticalLayout_3.addWidget(self.toolBox1)
        self.verticalLayout_3.addWidget(self.toolBox2)

        self.gridLayout.addWidget(self.groupBox, 1, 0, 1, 1)
        self.tabWidget_2 = QtWidgets.QTabWidget(self.tab)
        self.tabWidget_2.setObjectName("tabWidget_2")
        self.tab_3 = QtWidgets.QWidget()
        self.tab_3.setObjectName("tab_3")
        self.gridLayout_6 = QtWidgets.QGridLayout(self.tab_3)
        self.gridLayout_6.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_6.setObjectName("gridLayout_6")
        # self.checkableButton = QtWidgets.QPushButton(self.tab_3)
        # self.checkableButton.setCheckable(True)
        # self.checkableButton.setChecked(True)
        # self.checkableButton.setObjectName("checkableButton")
        # self.gridLayout_6.addWidget(self.checkableButton, 1, 0, 1, 1)

        # self.pushButton = QtWidgets.QPushButton(self.tab_3)
        # self.pushButton.setObjectName("pushButton")
        # self.gridLayout_6.addWidget(self.pushButton, 0, 0, 1, 1)
        # self.pushButton_5 = QtWidgets.QPushButton(self.tab_3)
        # self.pushButton_5.setObjectName("pushButton_5")
        # self.gridLayout_6.addWidget(self.pushButton_5, 2, 0, 1, 1)
        self.tabWidget_2.addTab(self.tab_3, "")
        self.tab_4 = QtWidgets.QWidget()
        self.tab_4.setObjectName("tab_4")
        self.gridLayout_b5 = QtWidgets.QGridLayout(self.tab_4)
        self.gridLayout_b5.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_b5.setObjectName("gridLayout_b5")

        self.table_account = QtWidgets.QTableWidget(self.tab_4)
        self.table_account.setObjectName("table_account")
        self.table_account.verticalHeader().setVisible(False)  # 隐藏垂直表头
        self.table_account.horizontalHeader().setVisible(True)  # 隐藏水平表头
        self.table_account.setColumnCount(8)
        self.table_account.setRowCount(4)
        self.table_account.setEditTriggers(self.table_account.NoEditTriggers)
        self.table_account.setHorizontalHeaderLabels(
            ['期货公司BrokeID', '期货账户', '静态权益', '动态权益', '收益', '可用资金', '风险度', '可转资金'])  # 设置表头文字

        self.Button_Investor = QPushButton("修改交易账户")
        self.Button_Simnow = QPushButton("注册模拟账户（仅限工作日白天）")
        self.Button_SetServiceUser = QPushButton("设置数据会员账户")
        self.Button_BuyService = QPushButton("购买数据服务会员")

        self.table_strategy = QtWidgets.QTableWidget(self.tab_3)
        self.table_strategy.setObjectName("table_strategy")
        self.table_strategy.verticalHeader().setVisible(False)  # 隐藏垂直表头
        self.table_strategy.horizontalHeader().setVisible(True)  # 隐藏水平表头
        self.table_strategy.setColumnCount(9)
        self.table_strategy.setRowCount(4)
        self.table_strategy.setEditTriggers(self.table_strategy.NoEditTriggers)
        self.table_strategy.setHorizontalHeaderLabels(
            ['id', '策略', '执行合约数', '胜率', '盈亏比', '今日交易', '合约设置', '回测', '查看策略仓位'])  # 设置表头文字
        # 设置行高
        self.table_strategy.setRowHeight(0, 35)
        Trade_InstrumentBtn = QtWidgets.QPushButton('合约设置')
        Trade_InstrumentBtn.setFlat(True)
        Trade_InstrumentBtn.setStyleSheet('background-color:#ff0000;');
        # searchBtn.setDown(True)
        Trade_InstrumentBtn.setStyleSheet('QPushButton{margin:3px}')
        self.table_strategy.setCellWidget(0, 6, Trade_InstrumentBtn)

        Trade_BacktestingBtn = QtWidgets.QPushButton('回测')
        Trade_BacktestingBtn.setFlat(True)
        Trade_BacktestingBtn.setStyleSheet('background-color:#ff0000;');
        # searchBtn.setDown(True)
        Trade_BacktestingBtn.setStyleSheet('QPushButton{margin:3px}')
        self.table_strategy.setCellWidget(0, 7, Trade_BacktestingBtn)

        Trade_BacktestingBtn = QtWidgets.QPushButton('查看策略仓位')
        Trade_BacktestingBtn.setFlat(True)
        Trade_BacktestingBtn.setStyleSheet('background-color:#ff0000;');
        # searchBtn.setDown(True)
        Trade_BacktestingBtn.setStyleSheet('QPushButton{margin:3px}')
        self.table_strategy.setCellWidget(0, 8, Trade_BacktestingBtn)

        """"
        item = QtWidgets.Qtable_accountItem()
        self.table_account.setVerticalHeaderItem(0, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_account.setVerticalHeaderItem(1, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_account.setVerticalHeaderItem(2, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_account.setVerticalHeaderItem(3, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_account.setHorizontalHeaderItem(0, item)
        item = QtWidgets.Qtable_accountItem()
        self.table_account.setHorizontalHeaderItem(1, item)
        """
        self.gridLayout_b5.addWidget(self.table_account, 0, 0, 2, 2)
        self.gridLayout_b5.addWidget(self.Button_Investor, 1, 0, 1, 1)
        self.gridLayout_b5.addWidget(self.Button_Simnow, 1, 1, 1, 1)

        self.gridLayout_6.addWidget(self.table_strategy, 0, 0, 1, 1)
        # self.gridLayout_5.addWidget(self.Button_Simnow)

        self.tabWidget_2.addTab(self.tab_4, "")

        self.tab_5 = QtWidgets.QWidget()
        self.tab_5.setObjectName("tab_5")
        self.tabWidget_2.addTab(self.tab_5, "")

        self.tab_6 = QtWidgets.QWidget()
        self.tab_6.setObjectName("tab_6")
        self.tabWidget_2.addTab(self.tab_6, "")

        self.tab_7 = QtWidgets.QWidget()
        self.tab_7.setObjectName("tab_7")
        self.tabWidget_2.addTab(self.tab_7, "")

        self.tab_8 = QtWidgets.QWidget()
        self.tab_8.setObjectName("tab_8")
        self.tabWidget_2.addTab(self.tab_8, "")

        self.tab_9 = QtWidgets.QWidget()
        self.tab_9.setObjectName("tab_9")
        self.tabWidget_2.addTab(self.tab_9, "")

        self.tab_10 = QtWidgets.QWidget()
        self.tab_10.setObjectName("tab_10")
        self.tabWidget_2.addTab(self.tab_10, "")

        self.tab_11 = QtWidgets.QWidget()
        self.tab_11.setObjectName("tab_11")
        self.tabWidget_2.addTab(self.tab_11, "")

        self.gridLayout.addWidget(self.tabWidget_2, 0, 0, 1, 1)

        self.gridLayout_c9 = QtWidgets.QGridLayout(self.tab_9)
        self.gridLayout_c9.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_c9.setObjectName("gridLayout_c9")

        self.table_data = QtWidgets.QTableWidget(self.tab_9)
        self.table_data.setObjectName("table_data")
        self.table_data.verticalHeader().setVisible(False)  # 隐藏垂直表头
        self.table_data.horizontalHeader().setVisible(True)  # 隐藏水平表头
        self.table_data.setColumnCount(8)
        self.table_data.setRowCount(4)
        self.gridLayout_c9.addWidget(self.table_data, 0, 0, 2, 2)
        self.gridLayout_c9.addWidget(self.Button_SetServiceUser, 1, 0, 1, 1)
        self.gridLayout_c9.addWidget(self.Button_BuyService, 1, 1, 1, 1)

        self.gridLayout_c7 = QtWidgets.QGridLayout(self.tab_7)
        self.gridLayout_c7.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_c7.setObjectName("gridLayout_8")

        self.gridLayout_c8 = QtWidgets.QGridLayout(self.tab_8)
        self.gridLayout_c8.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_c8.setObjectName("gridLayout_9")

        # --------------------------------------
        # 历史交易日志标签
        self.table_historytd = QtWidgets.QTableWidget(self.tab_7)
        self.table_historytd.setObjectName("table_historymd")
        self.table_historytd.verticalHeader().setVisible(False)  # 隐藏垂直表头
        self.table_historytd.horizontalHeader().setVisible(True)  # 隐藏水平表头
        self.table_historytd.setColumnCount(7)
        self.table_historytd.setRowCount(4)
        self.table_historytd.setRowHeight(0, 28)
        self.table_historytd.setEditTriggers(self.table_historytd.NoEditTriggers)
        self.table_historytd.setHorizontalHeaderLabels(
            ['id', '日期', '时间', '期货公司', '账户', '错误', '操作'])  # 设置表头文字

        self.Trade_historytdBtn = QPushButton("打开日志")
        self.table_historytd.setCellWidget(0, 6, self.Trade_historytdBtn)
        self.gridLayout_c7.addWidget(self.table_historytd, 0, 0, 2, 2)
        self.Trade_historytdBtn.clicked.connect(self.OpenLog_TD)
        '''
        Trade_historytdBtn = QtWidgets.QPushButton('打开日志')
        Trade_historytdBtn.setFlat(True)
        Trade_historytdBtn.setStyleSheet('background-color:#ff0000;');
        # searchBtn.setDown(True)
        Trade_historytdBtn.setStyleSheet('QPushButton{margin:3px}')
        '''

        # 历史行情日志标签
        self.table_historymd = QtWidgets.QTableWidget(self.tab_8)
        self.table_historymd.setObjectName("table_historymd")
        self.table_historymd.verticalHeader().setVisible(False)  # 隐藏垂直表头
        self.table_historymd.horizontalHeader().setVisible(True)  # 隐藏水平表头
        self.table_historymd.setColumnCount(7)
        self.table_historymd.setRowCount(4)
        self.table_historymd.setRowHeight(0, 28)
        self.table_historymd.setEditTriggers(self.table_historymd.NoEditTriggers)
        self.table_historymd.setHorizontalHeaderLabels(
            ['id', '日期', '时间', '期货公司', '账户', '错误', '操作'])  # 设置表头文字
        self.Trade_historymdBtn = QPushButton("打开日志")
        self.table_historymd.setCellWidget(0, 6, self.Trade_historymdBtn)
        self.gridLayout_c8.addWidget(self.table_historymd, 0, 0, 2, 2)
        self.Trade_historymdBtn.clicked.connect(self.OpenLog_MD)
        '''
        Trade_historymdBtn = QtWidgets.QPushButton('打开日志')
        Trade_historymdBtn.setFlat(True)
        Trade_historymdBtn.setStyleSheet('background-color:#ff0000;');
        # searchBtn.setDown(True)
        Trade_historymdBtn.setStyleSheet('QPushButton{margin:3px}')
        self.table_historymd.setCellWidget(0, 6, Trade_historymdBtn)
        self.gridLayout_c8.addWidget(self.table_historymd, 0, 0, 2, 2)
        '''
        # self.Trade_historymdBtn.clicked.connect(self.OpenLog_MD)

        # -------------------------------------

        self.table_data.setEditTriggers(self.table_data.NoEditTriggers)
        self.table_data.setColumnCount(8)
        self.table_data.setRowCount(4)
        self.table_data.setHorizontalHeaderLabels(['id', '开始日期', '结束日期', '数据大小', '周期', '进度', '数据', '数据服务会员'])  # 设置表头文字
        # 设置行高

        self.table_data.setRowHeight(0, 35)
        self.table_data.setColumnWidth(5, 250)
        self.table_data.setColumnWidth(7, 300)
        Trade_DownloadData = QtWidgets.QPushButton('下载数据')
        Trade_DownloadData.setFlat(True)
        Trade_DownloadData.setStyleSheet('background-color:#ff0000;');
        # searchBtn.setDown(True)
        Trade_DownloadData.setStyleSheet('QPushButton{margin:3px}')
        self.table_data.setCellWidget(0, 6, Trade_DownloadData)
        # Trade_DownloadData.triggered.connect(self.OpenUrl_COOLQUANT)
        Trade_DownloadData.clicked.connect(self.OnStart)

        """

        self.dateEdit = QtWidgets.QDateEdit(self.tab)
        self.dateEdit.setObjectName("dateEdit")
        self.gridLayout.addWidget(self.dateEdit, 2, 0, 1, 1)
        """
        self.tabWidget.addTab(self.tab, "")
        self.tab_2 = QtWidgets.QWidget()
        self.tab_2.setObjectName("tab_2")
        self.gridLayout_2 = QtWidgets.QGridLayout(self.tab_2)
        self.gridLayout_2.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.groupBox_2 = QtWidgets.QGroupBox(self.tab_2)
        self.groupBox_2.setObjectName("groupBox_2")
        self.verticalLayout_4 = QtWidgets.QVBoxLayout(self.groupBox_2)
        self.verticalLayout_4.setObjectName("verticalLayout_4")
        self.label = QtWidgets.QLabel(self.groupBox_2)
        self.label.setObjectName("label")
        self.verticalLayout_4.addWidget(self.label)
        self.radioButton = QtWidgets.QRadioButton(self.groupBox_2)
        self.radioButton.setObjectName("radioButton")
        self.verticalLayout_4.addWidget(self.radioButton)
        self.checkBox = QtWidgets.QCheckBox(self.groupBox_2)
        self.checkBox.setObjectName("checkBox")
        self.verticalLayout_4.addWidget(self.checkBox)
        self.checkBox_2 = QtWidgets.QCheckBox(self.groupBox_2)
        self.checkBox_2.setTristate(True)
        self.checkBox_2.setObjectName("checkBox_2")
        self.verticalLayout_4.addWidget(self.checkBox_2)
        self.treeWidget = QtWidgets.QTreeWidget(self.groupBox_2)
        self.treeWidget.setObjectName("treeWidget")
        item_0 = QtWidgets.QTreeWidgetItem(self.treeWidget)
        item_0 = QtWidgets.QTreeWidgetItem(self.treeWidget)
        self.verticalLayout_4.addWidget(self.treeWidget)
        self.gridLayout_2.addWidget(self.groupBox_2, 0, 0, 1, 1)
        self.tabWidget.addTab(self.tab_2, "")
        self.verticalLayout_5.addWidget(self.tabWidget)
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")

        self.menu_b_popup = QtWidgets.QToolButton(self.centralwidget)
        self.menu_b_popup.setPopupMode(QtWidgets.QToolButton.InstantPopup)
        self.menu_b_popup.setObjectName("menu_b_popup")
        self.horizontalLayout.addWidget(self.menu_b_popup)

        self.Button_Start = QtWidgets.QPushButton(self.centralwidget)
        self.Button_Start.setObjectName("Button_Start")
        self.horizontalLayout.addWidget(self.Button_Start)
        # self.menu_a_popup = QtWidgets.QToolButton(self.centralwidget)
        # self.menu_a_popup.setObjectName("menu_a_popup")
        # self.horizontalLayout.addWidget(self.menu_a_popup)

        # self.menu_c_popup = QtWidgets.QToolButton(self.centralwidget)
        # self.menu_c_popup.setPopupMode(QtWidgets.QToolButton.MenuButtonPopup)
        # self.menu_c_popup.setObjectName("menu_c_popup")
        # self.horizontalLayout.addWidget(self.menu_c_popup)
        self.line_2 = QtWidgets.QFrame(self.centralwidget)
        self.line_2.setFrameShape(QtWidgets.QFrame.VLine)
        self.line_2.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_2.setObjectName("line_2")
        self.horizontalLayout.addWidget(self.line_2)
        self.Button_Stop = QtWidgets.QPushButton(self.centralwidget)
        self.Button_Stop.setEnabled(False)
        self.Button_Stop.setObjectName("Button_Stop")
        self.horizontalLayout.addWidget(self.Button_Stop)
        self.doubleSpinBox = QtWidgets.QDoubleSpinBox(self.centralwidget)
        self.doubleSpinBox.setObjectName("doubleSpinBox")
        self.horizontalLayout.addWidget(self.doubleSpinBox)
        self.toolButton = QtWidgets.QToolButton(self.centralwidget)
        self.toolButton.setPopupMode(QtWidgets.QToolButton.InstantPopup)
        self.toolButton.setObjectName("toolButton")
        self.horizontalLayout.addWidget(self.toolButton)
        self.verticalLayout_5.addLayout(self.horizontalLayout)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1068, 23))
        self.menubar.setObjectName("menubar")
        self.menuMenu = QtWidgets.QMenu(self.menubar)
        self.menuMenu.setObjectName("menuMenu")
        self.menuSubmenu_2 = QtWidgets.QMenu(self.menuMenu)
        self.menuSubmenu_2.setObjectName("menuSubmenu_2")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.dockWidget1 = QtWidgets.QDockWidget(MainWindow)
        self.dockWidget1.setObjectName("dockWidget1")
        self.dockWidget1.setFixedSize(800, QDesktopWidget().availableGeometry().height() - 20)  # 分别为宽度和高度

        MainWindow.setFixedSize(QDesktopWidget().availableGeometry().width(),
                                QDesktopWidget().availableGeometry().height())
        self.dockWidgetContents = QtWidgets.QWidget()
        self.dockWidgetContents.setObjectName("dockWidgetContents")

        self.verticalLayout_2 = QtWidgets.QVBoxLayout(self.dockWidgetContents)
        self.verticalLayout_2.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.verticalLayout = QtWidgets.QVBoxLayout()
        self.verticalLayout.setObjectName("verticalLayout")
        self.comboBox = QtWidgets.QComboBox(self.dockWidgetContents)
        self.comboBox.setObjectName("comboBox")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")

        self.verticalLayout.addWidget(self.comboBox)

        '''
        self.horizontalSlider = QtWidgets.QSlider(self.dockWidgetContents)
        self.horizontalSlider.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider.setObjectName("horizontalSlider")
        self.verticalLayout.addWidget(self.horizontalSlider)
        '''
        '''
        self.textEdit = QtWidgets.QTextEdit(self.dockWidgetContents)
        self.textEdit.setObjectName("textEdit")
        self.verticalLayout.addWidget(self.textEdit)
        '''
        # K线图位置
        self.item = CandlestickItem(data)  # 原始数据,对应（0,0）幅图
        npdata = np.array(data)
        logYdata = np.log(npdata[:, 1:])
        logYdata = np.insert(logYdata, 0, values=npdata[:, 0], axis=1)
        logYdata = list(map(tuple, logYdata))
        # item2 = CandlestickItem(logYdata)  # 把原始数据里Y轴相关的数据,全部预先用log对数化处理一下,对应（0,1）
        # item3 = CandlestickItem(data)  # 原始数据,对应第三幅图（1,0）
        # item4 = CandlestickItem(logYdata)  # 验证一下,是不是setLogMode在自定义组件的时候,得自己去实现（1,1）
        self.w = pg.GraphicsWindow()
        self.w.setWindowTitle('pyqtgraph example: customGraphicsItem')
        self.plt = self.w.addPlot(0, 0, title="M5 rb2110")
        self.plt.addItem(self.item)
        self.plt.showGrid(True, True)
        self.plt.setMouseEnabled(x=True, y=True)  # 禁用轴操作
        self.verticalLayout.addWidget(self.w)
        self.w.showMaximized()
        # K线图位置
        '''
        #分割线
        self.line = QtWidgets.QFrame(self.dockWidgetContents)
        self.line.setFrameShape(QtWidgets.QFrame.HLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")
        self.verticalLayout.addWidget(self.line)
        '''
        # 分时图
        self.win = pg.GraphicsLayoutWidget(show=True)
        self.win.setWindowTitle('分时图')
        self.plt = self.win.addPlot(title="M5 分时图")
        # plt.setAutoVisibleOnly(y=True)
        self.curve = self.plt.plot()
        self.plt.setMouseEnabled(x=False, y=False)  # 禁用轴操作
        self.verticalLayout.addWidget(self.win)
        self.win.showMaximized()

        def update():
            global data2, count
            data2.append(np.random.normal(size=10) + np.sin(count * 0.1) * 5)
            if len(data2) > 100:
                data2.pop(0)
            self.curve.setData(np.hstack(data2))
            count += 1

        timer = QtCore.QTimer()
        timer.timeout.connect(update)
        timer.start(50)
        for i in range(101):
            update()
        # 分时图

        # 资金曲线位置

        self.win = pg.GraphicsLayoutWidget(show=True)
        self.win.setWindowTitle('资金曲线')
        self.plt = self.win.addPlot(title="资金曲线")

        # self.win.setBackground('y')
        # plt.setAutoVisibleOnly(y=True)
        self.curve = self.plt.plot(pen='y')

        self.plt.setMouseEnabled(x=False, y=False)  # 禁用轴操作
        self.verticalLayout.addWidget(self.win)
        self.win.showMaximized()

        def update():
            global data2, count
            data2.append(np.random.normal(size=10) + np.sin(count * 0.1) * 5)
            if len(data2) > 100:
                data2.pop(0)
            self.curve.setData(np.hstack(data2))
            count += 1

        timer = QtCore.QTimer()
        timer.timeout.connect(update)
        timer.start(50)
        for i in range(101):
            update()
        # 资金曲线位置

        # 进度条

        self.progressBar = QtWidgets.QProgressBar(self.dockWidgetContents)
        self.progressBar.setProperty("value", 24)
        self.progressBar.setObjectName("账户仓位")
        self.verticalLayout.addWidget(self.progressBar)
        # self.gridLayout_6.addWidget(self.progressBar)

        self.verticalLayout_2.addLayout(self.verticalLayout)
        self.frame = QtWidgets.QFrame(self.dockWidgetContents)
        self.frame.setMinimumSize(QtCore.QSize(0, 100))
        self.frame.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtWidgets.QFrame.Raised)
        self.frame.setLineWidth(3)
        self.frame.setObjectName("frame")
        self.verticalLayout_2.addWidget(self.frame)
        self.dockWidget1.setWidget(self.dockWidgetContents)
        MainWindow.addDockWidget(QtCore.Qt.DockWidgetArea(1), self.dockWidget1)
        self.toolBar = QtWidgets.QToolBar(MainWindow)
        self.toolBar.setObjectName("toolBar")
        MainWindow.addToolBar(QtCore.Qt.TopToolBarArea, self.toolBar)
        self.dockWidget2 = QtWidgets.QDockWidget(MainWindow)
        self.dockWidget2.setObjectName("dockWidget2")
        self.dockWidgetContents_2 = QtWidgets.QWidget()
        self.dockWidgetContents_2.setObjectName("dockWidgetContents_2")
        self.gridLayout_3 = QtWidgets.QGridLayout(self.dockWidgetContents_2)
        self.gridLayout_3.setContentsMargins(0, 0, 0, 0)
        self.gridLayout_3.setObjectName("gridLayout_3")
        # 左边行情滚动位置
        '''
        self.verticalSlider = QtWidgets.QSlider(self.dockWidgetContents_2)
        self.verticalSlider.setOrientation(QtCore.Qt.Vertical)
        self.verticalSlider.setObjectName("verticalSlider")
        self.gridLayout_3.addWidget(self.verticalSlider, 0, 0, 1, 1)
        '''
        self.dockWidget2.setWidget(self.dockWidgetContents_2)
        MainWindow.addDockWidget(QtCore.Qt.DockWidgetArea(1), self.dockWidget2)

        # 热力图
        self.gr_wid = pg.GraphicsLayoutWidget(show=True)
        ## Create image items
        data4 = np.fromfunction(lambda i, j: (1 + 0.3 * np.sin(i)) * (i) ** 2 + (j) ** 2, (100, 100))
        noisy_data = data4 * (1 + 0.2 * np.random.random(data4.shape))
        noisy_transposed = noisy_data.transpose()

        # --- add non-interactive image with integrated color -----------------
        i1 = pg.ImageItem(image=data4)
        p1 = self.gr_wid.addPlot(title="参数优化")
        p1.addItem(i1)
        p1.setMouseEnabled(x=False, y=False)
        p1.disableAutoRange()
        # p1.hideButtons()
        p1.setRange(xRange=(0, 100), yRange=(0, 100), padding=0)
        for key in ['left', 'right', 'top', 'bottom']:
            p1.showAxis(key)
            axis = p1.getAxis(key)
            axis.setZValue(1)
            if key in ['top', 'right']:
                p1.getAxis(key).setStyle(showValues=False)

        cmap = pg.colormap.get('CET-L9')
        bar = pg.ColorBarItem(
            interactive=False, values=(0, 30_000), cmap=cmap,
            label='vertical fixed color bar'
        )
        bar.setImageItem(i1, insert_in=p1)
        '''
        # --- add interactive image with integrated horizontal color bar --------------
        i2 = pg.ImageItem(image=noisy_data)
        p2 = self.gr_wid.addPlot(1, 0, 1, 1, title="interactive")
        p2.addItem(i2, title='')
        # inserted color bar also works with labels on the right.
        p2.showAxis('right')
        p2.getAxis('left').setStyle(showValues=False)
        p2.getAxis('bottom').setLabel('bottom axis label')
        p2.getAxis('right').setLabel('right axis label')

        cmap = pg.colormap.get('CET-L4')
        bar = pg.ColorBarItem(
            values=(0, 30_000),
            cmap=cmap,
            label='horizontal color bar',
            limits=(0, None),
            rounding=1000,
            orientation='horizontal',
            pen='#8888FF', hoverPen='#EEEEFF', hoverBrush='#EEEEFF80'
        )
        bar.setImageItem(i2, insert_in=p2)

        # --- multiple images adjusted by a separate color bar ------------------------
        i3 = pg.ImageItem(image=noisy_data)
        p3 = self.gr_wid.addPlot(0, 1, 1, 1, title="shared 1")
        p3.addItem(i3)

        i4 = pg.ImageItem(image=noisy_transposed)
        p4 = self.gr_wid.addPlot(1, 1, 1, 1, title="shared 2")
        p4.addItem(i4)

        cmap = pg.colormap.get('CET-L8')
        bar = pg.ColorBarItem(
            # values = (-15_000, 15_000),
            limits=(-30_000, 30_000),  # start with full range...
            rounding=1000,
            width=10,
            cmap=cmap)
        bar.setImageItem([i3, i4])
        bar.setLevels(low=-5_000, high=15_000)  # ... then adjust to retro sunset.
        '''
        # manually adjust reserved space at top and bottom to align with plot
        bar.getAxis('bottom').setHeight(21)
        bar.getAxis('top').setHeight(31)
        self.gr_wid.addItem(bar, 0, 2, 2, 1)  # large bar spanning both rows

        # self.verticalLayout.addWidget(self.gr_wid)
        self.gridLayout_3.addWidget(self.gr_wid, 0, 0, 1, 1)
        # self.win.showMaximized()
        # 热力图

        # 热力图结果
        '''
        self.table_optimizationresult = QtWidgets.QTableWidget(self.tab_4)
        self.table_optimizationresult.setObjectName("table_account")
        self.table_optimizationresult.verticalHeader().setVisible(False)  # 隐藏垂直表头
        self.table_optimizationresult.horizontalHeader().setVisible(True)  # 隐藏水平表头
        self.table_optimizationresult.setColumnCount(5)
        self.table_optimizationresult.setRowCount(4)
        self.table_optimizationresult.setEditTriggers(self.table_optimizationresult.NoEditTriggers)
        self.table_optimizationresult.setHorizontalHeaderLabels(
            ['ID', '参数1', '参数2', '参数3', '参数4', '收益率', '胜率', '交易次数'])  # 设置表头文字
        self.table_optimizationresult.setRowHeight(0, 28)
        # 设置行高

        self.table_optimizationresult.setRowHeight(0, 35)
        Trade_optimizationresultBtn = QtWidgets.QPushButton('合约设置')
        Trade_optimizationresultBtn.setFlat(True)
        Trade_optimizationresultBtn.setStyleSheet('background-color:#ff0000;');
        # searchBtn.setDown(True)
        Trade_optimizationresultBtn.setStyleSheet('QPushButton{margin:3px}')
        self.table_optimizationresult.setCellWidget(0, 4, Trade_optimizationresultBtn)
        '''
        # self.gridLayout_3.addWidget(self.table_optimizationresult, 0, 1, 1, 1)

        # 热力图结果

        # 资金曲线位置
        self.win5 = pg.GraphicsLayoutWidget(show=True)
        self.win5.setWindowTitle('资金曲线')
        self.plt = self.win5.addPlot(title="资金曲线")

        # self.win.setBackground('y')
        # plt.setAutoVisibleOnly(y=True)
        self.curve = self.plt.plot(pen='y')

        self.plt.setMouseEnabled(x=False, y=False)  # 禁用轴操作
        self.gridLayout_3.addWidget(self.win5, 1, 0, 1, 1)
        self.win5.showMaximized()

        def update2():
            global data2, count
            data2.append(np.random.normal(size=10) + np.sin(count * 0.1) * 5)
            if len(data2) > 100:
                data2.pop(0)
            self.curve.setData(np.hstack(data2))
            count += 1

        timer = QtCore.QTimer()
        timer.timeout.connect(update2)
        timer.start(50)
        for i in range(101):
            update2()
        # 资金曲线位置

        self.Function1 = QtWidgets.QAction(MainWindow)
        self.Function1.setObjectName("Function1")
        self.Function2 = QtWidgets.QAction(MainWindow)
        self.Function2.setObjectName("Function2")

        self.Navgiate1 = QtWidgets.QAction(MainWindow)
        self.Navgiate1.setObjectName("Navgiate1")
        self.Navgiate2 = QtWidgets.QAction(MainWindow)
        self.Navgiate2.setObjectName("Navgiate2")
        self.Navgiate3 = QtWidgets.QAction(MainWindow)
        self.Navgiate3.setObjectName("Navgiate3")
        self.Navgiate4 = QtWidgets.QAction(MainWindow)
        self.Navgiate4.setObjectName("Navgiate4")
        self.Navgiate5 = QtWidgets.QAction(MainWindow)
        self.Navgiate5.setObjectName("Navgiate5")
        self.Navgiate6 = QtWidgets.QAction(MainWindow)
        self.Navgiate6.setObjectName("Navgiate6")
        self.Navgiate7 = QtWidgets.QAction(MainWindow)
        self.Navgiate7.setObjectName("Navgiate7")
        self.Navgiate8 = QtWidgets.QAction(MainWindow)
        self.Navgiate8.setObjectName("Navgiate8")

        self.Navgiate1_C = QtWidgets.QAction(MainWindow)
        self.Navgiate1_C.setObjectName("Navgiate1_C")
        self.menuSubmenu_2.addAction(self.Navgiate2)
        self.menuSubmenu_2.addAction(self.Navgiate1_C)
        self.menuMenu.addAction(self.Function1)
        self.menuMenu.addAction(self.Function2)
        # self.menuMenu.addAction(self.menuSubmenu_2.menuAction())
        self.menubar.addAction(self.menuMenu.menuAction())

        # self.menuMenu.addAction(self.Navgiate1)
        # self.menuMenu.addAction(self.menuSubmenu_2.menuAction())
        # self.menubar.addAction(self.menuMenu.menuAction())

        self.toolBar.addAction(self.Navgiate1)
        self.toolBar.addAction(self.Navgiate2)
        self.toolBar.addAction(self.Navgiate3)
        self.toolBar.addAction(self.Navgiate4)
        self.toolBar.addAction(self.Navgiate5)
        self.toolBar.addAction(self.Navgiate6)
        self.toolBar.addAction(self.Navgiate7)
        self.toolBar.addAction(self.Navgiate8)

        self.retranslateUi(MainWindow)
        self.tabWidget.setCurrentIndex(0)
        self.toolBox1.setCurrentIndex(1)
        self.toolBox2.setCurrentIndex(1)
        self.tabWidget_2.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)
        # MainWindow.setTabOrder(self.pushButton, self.checkableButton)
        # MainWindow.setTabOrder(self.checkableButton, self.pushButton_5)
        # MainWindow.setTabOrder(self.pushButton_5, self.tabWidget_2)
        MainWindow.setTabOrder(self.tabWidget_2, self.table_account)
        # MainWindow.setTabOrder(self.tabWidget, self.table_strategy)

        MainWindow.setTabOrder(self.table_account, self.radioButton)
        MainWindow.setTabOrder(self.radioButton, self.checkBox)
        MainWindow.setTabOrder(self.checkBox, self.checkBox_2)
        MainWindow.setTabOrder(self.checkBox_2, self.treeWidget)
        MainWindow.setTabOrder(self.treeWidget, self.Button_Start)
        # MainWindow.setTabOrder(self.Button_Start, self.menu_a_popup)
        # MainWindow.setTabOrder(self.menu_a_popup, self.menu_b_popup)
        # MainWindow.setTabOrder(self.menu_b_popup, self.menu_c_popup)
        # MainWindow.setTabOrder(self.menu_c_popup, self.Button_Stop)
        MainWindow.setTabOrder(self.Button_Stop, self.doubleSpinBox)
        MainWindow.setTabOrder(self.doubleSpinBox, self.toolButton)
        MainWindow.setTabOrder(self.toolButton, self.comboBox)

        # 左边行情滚动位置
        # MainWindow.setTabOrder(self.comboBox, self.horizontalSlider)
        # MainWindow.setTabOrder(self.horizontalSlider, self.textEdit)
        # 左边行情滚动位置
        # MainWindow.setTabOrder(self.textEdit, self.verticalSlider)
        # MainWindow.setTabOrder(self.verticalSlider, self.tabWidget)

        # 1，1 委托记录
        # MainWindow.setTabOrder(self.tabWidget, self.lineEdit)
        # MainWindow.setTabOrder(self.lineEdit, self.listWidget)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        # 交易信息
        self.groupBox.setTitle(_translate("MainWindow", ""))
        self.toolBox1.setItemText(self.toolBox1.indexOf(self.page_order), _translate("MainWindow", "委托记录"))
        self.toolBox2.setItemText(self.toolBox2.indexOf(self.page_instrument), _translate("MainWindow", "合约"))

        """
        __sortingEnabled = self.listWidget.isSortingEnabled()
        self.listWidget.setSortingEnabled(False)
        
        item = self.listWidget.item(0)
        item.setText(_translate("MainWindow", "New Item"))
        item = self.listWidget.item(1)
        item.setText(_translate("MainWindow", "New Item"))
        item = self.listWidget.item(2)
        item.setText(_translate("MainWindow", "New Item"))
        item = self.listWidget.item(3)
        item.setText(_translate("MainWindow", "New Item"))
        item = self.listWidget.item(4)
        item.setText(_translate("MainWindow", "New Item"))
        item = self.listWidget.item(5)
        item.setText(_translate("MainWindow", "New Item"))
        item = self.listWidget.item(6)
        item.setText(_translate("MainWindow", "New Item"))
        item = self.listWidget.item(7)
        item.setText(_translate("MainWindow", "New Item"))

        self.listWidget.setSortingEnabled(__sortingEnabled)
        """
        self.toolBox1.setItemText(self.toolBox1.indexOf(self.page_trade), _translate("MainWindow", "交易记录"))
        self.toolBox2.setItemText(self.toolBox2.indexOf(self.page_position), _translate("MainWindow", "全部持仓/策略持仓"))

        # self.checkableButton.setText(_translate("MainWindow", "Checkable button"))
        # self.pushButton.setText(_translate("MainWindow", "PushButton"))
        # self.pushButton_5.setText(_translate("MainWindow", "PushButton"))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_3), _translate("MainWindow", "策略执行"))

        """
        item = self.table_account.verticalHeaderItem(0)
        item.setText(_translate("MainWindow", "New Row"))
        item = self.table_account.verticalHeaderItem(1)
        item.setText(_translate("MainWindow", "New Row"))
        item = self.table_account.verticalHeaderItem(2)
        item.setText(_translate("MainWindow", "New Row"))
        item = self.table_account.verticalHeaderItem(3)
        item.setText(_translate("MainWindow", "New Row"))
        item = self.table_account.horizontalHeaderItem(0)
        item.setText(_translate("MainWindow", "New Column"))
        item = self.table_account.horizontalHeaderItem(1)
        item.setText(_translate("MainWindow", "New Column 2"))
        """

        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_4), _translate("MainWindow", "账号"))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_5), _translate("MainWindow", "今日交易日志"))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_6), _translate("MainWindow", "今日行情日志"))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_7), _translate("MainWindow", "历史交易日志"))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_8), _translate("MainWindow", "历史行情日志"))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_9), _translate("MainWindow", "量化回测数据"))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_10), _translate("MainWindow", "量化回测报告"))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_11), _translate("MainWindow", "参数优化结果"))

        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), _translate("MainWindow", "交易主界面"))
        self.groupBox_2.setTitle(_translate("MainWindow", "GroupBox"))
        self.label.setText(_translate("MainWindow", "TextLabel"))
        self.radioButton.setText(_translate("MainWindow", "RadioB&utton"))
        self.checkBox.setText(_translate("MainWindow", "CheckBox"))
        self.checkBox_2.setText(_translate("MainWindow", "CheckBox Tristate"))
        self.treeWidget.headerItem().setText(0, _translate("MainWindow", "qdz"))
        __sortingEnabled = self.treeWidget.isSortingEnabled()
        self.treeWidget.setSortingEnabled(False)
        self.treeWidget.topLevelItem(0).setText(0, _translate("MainWindow", "qzd"))
        self.treeWidget.topLevelItem(1).setText(0, _translate("MainWindow", "effefe"))
        self.treeWidget.setSortingEnabled(__sortingEnabled)
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_2), _translate("MainWindow", "选项设置"))
        self.Button_Start.setText(_translate("MainWindow", "启动策略自动交易（已停止）"))
        self.Button_Stop.setText(_translate("MainWindow", "停止策略自动交易（运行中）"))

        # self.menu_a_popup.setText(_translate("MainWindow", "menu_a_popup"))
        self.menu_b_popup.setText(_translate("MainWindow", "交易功能项"))
        # self.menu_c_popup.setText(_translate("MainWindow", "menu_c_popup"))
        self.toolButton.setText(_translate("MainWindow", "..."))
        self.menuMenu.setTitle(_translate("MainWindow", "&功能菜单"))
        self.menuSubmenu_2.setTitle(_translate("MainWindow", "&Submenu 2"))
        # self.dockWidget1.setWindowTitle(_translate("MainWindow", "&Dock widget 1"))
        self.dockWidget1.setWindowTitle(_translate("MainWindow", "&行情和资金曲线"))
        self.dockWidget1.setFeatures(QDockWidget.NoDockWidgetFeatures)

        self.comboBox.setItemText(0, _translate("MainWindow", "M1"))
        self.comboBox.setItemText(1, _translate("MainWindow", "M3"))
        self.comboBox.setItemText(2, _translate("MainWindow", "M5"))
        self.comboBox.setItemText(3, _translate("MainWindow", "M10"))
        self.comboBox.setItemText(4, _translate("MainWindow", "M15"))
        self.comboBox.setItemText(5, _translate("MainWindow", "M30"))
        self.comboBox.setItemText(6, _translate("MainWindow", "M60"))
        self.comboBox.setItemText(7, _translate("MainWindow", "D1"))

        self.toolBar.setWindowTitle(_translate("MainWindow", "toolBar"))
        self.dockWidget2.setWindowTitle(_translate("MainWindow", "量化回测参数优化"))
        self.dockWidget2.setFeatures(QDockWidget.NoDockWidgetFeatures)

        self.Navgiate1.setText(_translate("MainWindow", "&VNPY官网"))
        self.Navgiate2.setText(_translate("MainWindow", "&VNPY知乎"))
        self.Navgiate3.setText(_translate("MainWindow", "&期货低佣金开户"))
        self.Navgiate4.setText(_translate("MainWindow", "&量化资源列表"))
        self.Navgiate5.setText(_translate("MainWindow", "&VNPY知乎"))
        self.Navgiate6.setText(_translate("MainWindow", "&VNPY知乎"))
        self.Navgiate7.setText(_translate("MainWindow", "&VNPY知乎"))
        self.Navgiate8.setText(_translate("MainWindow", "&VNPY知乎"))

        self.Navgiate2.setToolTip(_translate("MainWindow", "submenu"))
        self.Navgiate1_C.setText(_translate("MainWindow", "一键平仓"))

        # 增加账户
        self.Function1.setText(_translate("MainWindow", "&交易账户管理"))
        self.Function2.setText(_translate("MainWindow", "&行情服务器管理"))

        self.Navgiate1.triggered.connect(self.OpenUrl_VNPY)
        self.Navgiate2.triggered.connect(self.OpenUrl_ZHIHU)
        self.Navgiate3.triggered.connect(self.OpenUrl_KAIHU)
        self.Navgiate4.triggered.connect(self.OpenUrl_COOLQUANT)

        self.Button_Start.clicked.connect(self.OnStart)
        self.Button_Stop.clicked.connect(self.OnStop)

        self.Button_Investor.clicked.connect(self.OpenUrl_VNPY)
        self.Button_Simnow.clicked.connect(self.OpenUrl_VNPY)
        self.Button_SetServiceUser.clicked.connect(self.OpenUrl_VNPY)
        self.Button_BuyService.clicked.connect(self.OpenUrl_VNPY)

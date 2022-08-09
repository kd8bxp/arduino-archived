

#
# AprilTagを検出.
# https://docs.openmv.io/library/omv.image.html#image.image.find_apriltags
#

##################################################
# import
##################################################
import lcd
import sensor
from machine import UART
from Maix import GPIO
from fpioa_manager import *

fm.register(34,fm.fpioa.UART1_TX)
fm.register(35,fm.fpioa.UART1_RX)

#fm.register(board_info.LED_W, fm.fpioa.GPIO3)
uart_out = UART(UART.UART1, 115200,8,None,1, timeout=1000, read_buf_len=4096)
##################################################
# initialize
##################################################
# LCDを初期化
#lcd.init()
# LCDの方向を設定
#lcd.direction(lcd.YX_LRUD)

# カメラを初期化
sensor.reset()
sensor.set_brightness(0)
sensor.set_pixformat(sensor.RGB565) #GRAYSCALE) #RGB565)
sensor.set_framesize(sensor.QQVGA)
#sensor.set_hmirror(0)
sensor.skip_frames(time = 500)     # Takes n number of snapshots to let the camera image stabilize
sensor.run(1)

##################################################
# main
##################################################
while True:
    # カメラ画像を取得
    img = sensor.snapshot()
    # AprilTagを検出
    res = img.find_apriltags()

    # 全ての結果に対して実行
    for i in res:
        print(i)
        uart_out.write(repr(i))
        uart_out.write('\n')
        # 矩形を描画
        img.draw_rectangle(i.x(), i.y(), i.w(), i.h(), color = (255, 0, 0), thickness = 2)
        # 十字を描画
        img.draw_cross(i.cx(), i.cy(), color = (255, 0, 0), thickness = 2)
    # 画像をLCDに描画
    #lcd.display(img)

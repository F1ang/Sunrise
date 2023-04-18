"""
    V5.0----->V1.0 2023.4.15---4.18(finish)
    1，巡线->色块
    2，十字路口->色块宽度阈值
    3，TAG36H11识别（单点）

    Key Point:拐角路口、直行路口
"""

import sensor, image, time, math
from pyb import Servo,UART,LED
import json
from uarray import array
from pid import PID

sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) # use RGB565.
sensor.set_framesize(sensor.QQVGA) # use QQVGA for speed. QQVGA=160*120 QVGA=320*240
sensor.set_vflip(False)

sensor.skip_frames(time = 3000)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...

clock = time.clock() # Tracks FPS.

""" TAG """
f_x = (2.8 / 3.984) * 160 # 感光CCD的X方向焦距
f_y = (2.8 / 2.952) * 120 # 感光CCD的Y方向焦距
c_x = 160 * 0.5 # 感光CCD的中心点X
c_y = 120 * 0.5 # 感光CCD的中心点Y

def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob.pixels() > max_size:
            max_blob=blob
            max_size = blob.pixels()
    return max_blob

def degrees(radians):
    return (180 * radians) / math.pi

class PIx3:
    # uart3 #
    #构造函数
    def __init__(self, uart_num, baud , bits_num, parity_cal, stop_num):
        self.uart = UART(uart_num, baud)
        self.uart.init(baud, bits = bits_num,  parity = parity_cal, stop = 1)

    def crc8(self,CRCData, CRCLenth):
        crc = 0xff
        for crc_data in CRCData:
            crc ^= crc_data
            for index in range(8):
                if (crc & 0x80):
                    crc = (crc<<1) ^ 0x1d
                else:
                    crc = (crc<<1)
        return crc

    def pi2chassic(self, MessageData, MessageCode):
        """
            1,from uarray import array
            2,frame type
        """
        #帧头
        packet = array('b')
        packet.append(0x55)
        packet.append(0xAA)
        packet.append(len(MessageData) + 7)
        packet.append(MessageCode)

        #校验
        for i in range(4,len(MessageData) + 7 - 3):   #3byte-crc 尾1 尾2
            packet.append(MessageData[i - 4])

        #帧尾
        packet.append(self.crc8(packet, len(MessageData) + 7 - 3))
        packet.append(0x0D)
        packet.append(0x0A)
        self.uart.write(packet)

    def chassic2pi(self, ReadBuf, ReadLenth):
        """
            1,2*24byte + 7byte = 55byte frame
            2,ReadCode = 06
            3,帧格式同发一样
            4,readline() #接收到\n止
            ps:必须给底盘控制板上电
        """
        FrameBuf = [0]*24
        ReadBuf = self.uart.read()
        if ReadBuf == None:
            return 0
        else:
            if ReadBuf[0] == 0X55 and ReadBuf[54] == 0x0a :   #帧头1 + 帧尾2
                #print(ReadBuf[1]) #0xaa
                #print(ReadBuf[2]) #55=24*2+7byte
                #print(ReadBuf[3]) #code=06
                #print(ReadBuf[53]) #0x0d
                #print(ReadBuf[54]) #0x0a

                #拼接数据bytes->short 4~51
                #gyro-x y z
                FrameBuf[0] = int( ReadBuf[5]<<8 | ReadBuf[4] )
                FrameBuf[1] = int( ReadBuf[7]<<8 | ReadBuf[6] )
                FrameBuf[2] = int( ReadBuf[9]<<8 | ReadBuf[8] )
                #gyro-x y z
                FrameBuf[3] = int( ReadBuf[11]<<8 | ReadBuf[10] )
                FrameBuf[4] = int( ReadBuf[13]<<8 | ReadBuf[12] )
                FrameBuf[5] = int( ReadBuf[15]<<8 | ReadBuf[14] )
                #pitch roll yaw
                FrameBuf[6] = int( ReadBuf[17]<<8 | ReadBuf[16] )
                FrameBuf[7] = int( ReadBuf[19]<<8 | ReadBuf[18] )
                FrameBuf[8] = int( ReadBuf[21]<<8 | ReadBuf[20] )
                #里程计坐标 x(m) y(m) yaw(rad)  odom_frame
                FrameBuf[9] = int( ReadBuf[23]<<8 | ReadBuf[22] )
                FrameBuf[10] = int( ReadBuf[25]<<8 | ReadBuf[24] )
                FrameBuf[11] = int( ReadBuf[27]<<8 | ReadBuf[26] )
                #里程计坐标变化量  d_x(m) d_y(m) d_yaw(rad)  base_frame
                FrameBuf[12] = int( ReadBuf[29]<<8 | ReadBuf[28] )
                FrameBuf[13] = int( ReadBuf[31]<<8 | ReadBuf[30] )
                FrameBuf[14] = int( ReadBuf[33]<<8 | ReadBuf[32] )
                #编码器当前值
                FrameBuf[15] = int( ReadBuf[35]<<8 | ReadBuf[34] )
                FrameBuf[16] = int( ReadBuf[37]<<8 | ReadBuf[36] )
                FrameBuf[17] = int( ReadBuf[39]<<8 | ReadBuf[38] )
                FrameBuf[18] = int( ReadBuf[41]<<8 | ReadBuf[40] )
                #编码器目标值
                FrameBuf[19] = int( ReadBuf[43]<<8 | ReadBuf[42] )
                FrameBuf[20] = int( ReadBuf[45]<<8 | ReadBuf[44] )
                FrameBuf[21] = int( ReadBuf[47]<<8 | ReadBuf[46] )
                FrameBuf[22] = int( ReadBuf[49]<<8 | ReadBuf[48] )
                #电池
                FrameBuf[23] = int( ReadBuf[51]<<8 | ReadBuf[50] )

                #字节负数处理
                for p in range(0,24):
                    if FrameBuf[p] > 32767:
                        FrameBuf[p] =  FrameBuf[p]-65535

                #print(FrameBuf[7])
                return FrameBuf
            else:
                return 0

class WTX3:

    # uart1 #
    #构造函数
    def __init__(self, uart_num, baud , bits_num, parity_cal, stop_num):
        self.uart = UART(uart_num, baud)
        self.uart.init(baud, bits = bits_num,  parity = parity_cal, stop = 1)
        self.wt_sum = 0

    #和校验
    def crc_sum(self, Crc_Sum):
        """
            和校验0x55+0x53+RollH+RollL+PitchH+PitchL+YawH+YawL+VH+VL
        """
        for wt in range(0,10):
            self.wt_sum = self.wt_sum + int(Crc_Sum[wt])
        return self.wt_sum

    def wt_read(self, WTBuf, WTLenth):
        """
            1,读取11byte 0x55 0x53 Roll_L Roll_H Pitch_L Pitch_H Yaw_L Yaw_H V_L V_H Sum
            2,
        """
        FrameBuf = [0]*3
        WTBuf = self.uart.read()
        if WTBuf == None:
            return 0
        else:
            if WTBuf[0] == 0X55 :  #帧头 + 帧尾校验位
                #print(WTBuf[0])  #85
                #print(WTBuf[1])  #83
                #print(WTBuf[10]) #
                #拼接数据bytes->short 2~7
                FrameBuf[0] = int( WTBuf[3]<<8 | WTBuf[2] ) / 32768*180 #Roll
                FrameBuf[1] = int( WTBuf[5]<<8 | WTBuf[4] ) / 32768*180 #Pitch
                FrameBuf[2] = int( WTBuf[7]<<8 | WTBuf[6] ) / 32768*180 #Yaw

                #字节负数处理
                for p in range(0,3):
                    if FrameBuf[p] > 32767:
                        FrameBuf[p] =  FrameBuf[p]-65535

                #print(FrameBuf[2])
                return FrameBuf
            else:
                return 0

class PATH2PLAN:

    #构造函数
    def __init__(self, Px,Ix,Dx, max_iout, max_out, min_err):
        self.px = Px
        self.ix = Ix
        self.dx = Dx

        self.last_error = 0
        self.Integral = 0
        self.Derivative = 0

        self.min_err = min_err
        self.max_iout = max_iout
        self.max_out = max_out

    def pid_cal(self,err):
        if abs(err) <  self.min_err:
            return 0

        p_out = self.px * err
        self.Integral = self.Integral + err

        if self.Integral > self.max_iout:
            self.Integral = self.max_iout
        elif self.Integral < -self.max_iout:
            self.Integral = -self.max_iout

        pid_out = self.px * err + self.ix * self.Integral + self.dx * (err - self.last_error)
        self.last_error = err

        if pid_out > self.max_out:
            pid_out = self.max_out
            return pid_out
        elif pid_out < -self.max_out:
            pid_out = -self.max_out
            return pid_out
        else:
            return pid_out


def cal_angle(angle_error):
    """
        角度误差校正
    """
    if angle_error > 180:
        angle_error = angle_error - 360
    elif angle_error < -180:
        angle_error = 360 + angle_error
    else:
        angle_error = angle_error
    return angle_error

# PID #
line_pid = PATH2PLAN(Px=10,Ix=0,Dx=0, max_iout=100, max_out=300, min_err=2)

# TAG #
tag_num = -2    #记录TAG
tag_start = 0   #匹配TAG
roi_tag = {
    'left':   (0,  0,  66, 120),
    'right':  (80,0,  80, 120),
}

roi_rec = (40,22,91,73) #记录TAG
roi_turn_rec = (0,0,160,120)

class tag_id(object):
    tag_id = -1
    cx = -1

left_id = tag_id()
right_id = tag_id()

# Blob #
#thresholds = [(0, 95, -16, 81, 72, -42)]
thresholds = [(2, 100, -17, 98, 50, -76)]

ROIS = {    # 划分了上中下左右五个部分
    'down':   (0, 105, 160, 15),
    'middle': (0, 52,  160, 15),
    'up':	 (0,  0,  160, 15),
    'left':   (0,  0,  15, 120),
    'right':  (145,0,  15, 120),
    'All':	(0,  0,  160,120),
}

roi_rects = (22, 38, 127, 74)

class ControlFlag(object):
    turn_yaw = 0 #1-左拐路口 2-直行路口 3-右拐路口 0-巡线  4-大转弯--暂定2个ROI识别TAG

    turn_record_num = -2 #掉头记录TAG
    turn_round = 0 #掉头卡 1-左拐路口 2-直行路口 3-右拐路口 0-巡线 4-大转弯
    turn_round_num = -1 #掉头卡 要拐的路口数
    turn_round_is = 0 #掉头卡 0-不掉头 1-掉头识别TAG 2-掉头直行
    cross_cnt = 0 #拐角互锁 1-锁前行 2-锁转角

class Control_speed(object):
    speed_x = 0
    speed_y = 0
    speed_yaw = 0

class LineFlag(object):
    flag = 0
    cross_y = 0
    delta_x = 0

class EndFlag(object):
    endline_type = 0
    endline_y = 0

LineFlag=LineFlag()
EndFlag=EndFlag()
ControlFlag = ControlFlag()
ControlSpeed = Control_speed()

def find_blobs_line(img):
    # 基本巡线 #
    global ROIS
    roi_blobs_result = {}
    for roi_direct in ROIS.keys():
        roi_blobs_result[roi_direct] = {
            'cx': -1,
            'cy': -1,
            'blob_flag': False
        }

    for roi_direct,roi in ROIS.items():
        blobs=img.find_blobs(thresholds, roi=roi, merge=True, pixels_area=10, invert=True)
        if len(blobs) == 0:
            continue
        largest_blob = max(blobs, key=lambda b: b.pixels())
        x,y,width,height = largest_blob[:4]
        if not(width >=3 and width <= 45 and height >= 3 and height <= 45):
            continue
        # debug #
        img.draw_rectangle(largest_blob.rect())
        img.draw_cross(largest_blob.cx(), largest_blob.cy())
        # debug #
        roi_blobs_result[roi_direct]['cx'] = largest_blob.cx()
        roi_blobs_result[roi_direct]['cy'] = largest_blob.cy()
        roi_blobs_result[roi_direct]['blob_flag'] = True

    if (roi_blobs_result['down']['blob_flag']):
        if (roi_blobs_result['left']['blob_flag'] and roi_blobs_result['right']['blob_flag']):
            LineFlag.flag = 2 #十字路口或丁字路口
        elif (roi_blobs_result['left']['blob_flag']):
            LineFlag.flag = 3 # 左转路口
        elif (roi_blobs_result['right']['blob_flag']):
            LineFlag.flag = 4 # 右转路口
        elif (roi_blobs_result['middle']['blob_flag']):
            LineFlag.flag = 1 #直线
        else:
            LineFlag.flag = 0 # 未检测到

    else:
        if(roi_blobs_result['middle']['blob_flag']and roi_blobs_result['up']['blob_flag']):
            if (roi_blobs_result['left']['blob_flag']and roi_blobs_result['right']['blob_flag']):
                LineFlag.flag = 5 # 即将跨过十字路口
            elif (roi_blobs_result['left']['blob_flag']):
                LineFlag.flag = 6 # 即将跨过左拐丁字路口
            elif (roi_blobs_result['right']['blob_flag']):
                LineFlag.flag = 7 # 即将跨过右拐丁字路口
            else:
                LineFlag.flag = 8 # 直线（无down块）
        else:
            LineFlag.flag = 0
    # 下面这部分是特例的判断，防止出现
    # “本来是直线道路，但是太靠近左侧或者右侧，被识别成了左转或者右转”
    # 这样的特殊情况
    if (LineFlag.flag == 3 and roi_blobs_result['left']['cy']<10):
        LineFlag.flag = 1
    if (LineFlag.flag == 4 and roi_blobs_result['right']['cy']<10):
        LineFlag.flag = 1
    if (LineFlag.flag == 3 and roi_blobs_result['down']['cx']<30):
        LineFlag.flag = 1
    if (LineFlag.flag == 4 and roi_blobs_result['down']['cy']>130):
        LineFlag.flag = 1

    # 计算两个输出值，路口交叉点的纵坐标和直线时红线的偏移量
    LineFlag.cross_y = 0
    LineFlag.delta_x = 0

    if (LineFlag.flag == 1 or LineFlag.flag == 2 or LineFlag.flag == 3 or LineFlag.flag == 4) :
        LineFlag.delta_x = roi_blobs_result['down']['cx']
    elif (LineFlag.flag == 5 or LineFlag.flag == 6 or LineFlag.flag == 7 or LineFlag.flag == 8):
        LineFlag.delta_x = roi_blobs_result['middle']['cx']
    else:
        LineFlag.delta_x = 0

    if (LineFlag.flag == 2 or LineFlag.flag == 5):
        LineFlag.cross_y = (roi_blobs_result['left']['cy']+roi_blobs_result['right']['cy'])//2
    elif (LineFlag.flag == 3 or LineFlag.flag == 6):
        LineFlag.cross_y = roi_blobs_result['left']['cy']
    elif (LineFlag.flag == 4 or LineFlag.flag == 7):
        LineFlag.cross_y = roi_blobs_result['right']['cy']
    else:
        LineFlag.cross_y = 0

def find_endline(img):
    endbox_num = 0
    for r in img.find_rects(roi_rects,threshold=10000):
        endbox_size = r.magnitude()
        endbox_w = r.w()
        endbox_h = r.h()
        k=1
        # 筛选黑色矩形大小
        if (endbox_size<24000*k*k and endbox_h<25*k and endbox_w<25*k) :
            endbox_num = endbox_num + 1;
        img.draw_rectangle(r[0:4], color = (255, 0, 0))
        #print(endbox_num)
    # 判断是否是终点线
    EndFlag.endline_type = 0
    if (endbox_num>2 and endbox_num<6):
        EndFlag.endline_type = 1 # 检测到第一条终点线
    elif(endbox_num >=6 ):
        EndFlag.endline_type = 2 # 检测到两条终点线
    else:
        EndFlag.endline_type = 0 # 未检测到终点线

def find_tag(img):
    """
        2个ROI匹配TAG
    """
    global tag_num,tag_start

    global roi_tag
    roi_tags_result = {}
    for roi_direct in roi_tag.keys():
        roi_tags_result[roi_direct] = {
            'cx': -1,
            'id': -1,
        }
    for roi_direct, roi in roi_tag.items():
        tag = img.find_apriltags(roi=roi, fx=f_x, fy=f_y, cx=c_x, cy=c_y) # 默认TAG36H11
        if tag:
            # debug #
            img.draw_rectangle(tag[0][0:4], color = (255, 0, 0))
            img.draw_cross(tag[0][6], tag[0][7], color = (0, 255, 0))
            # debug #
            #roi_tags_result[roi_direct]['cx'] = tag[0][6]
            roi_tags_result[roi_direct]['id'] = tag[0][4]

            left_id.tag_id = roi_tags_result['left']['id']
            right_id.tag_id = roi_tags_result['right']['id']
            #print("1=%d" %left_id.tag_id)
            #print("2=%d" %right_id.tag_id)
            #print(tag_num)
            if left_id.tag_id == tag_num:
                ControlFlag.turn_yaw = 1 #左拐
            elif right_id.tag_id == tag_num:
                ControlFlag.turn_yaw = 3 #右拐
            else:
                ControlFlag.turn_yaw = 2 #直走
            tag_start = 0
            #print(ControlFlag.turn_yaw)

def record_tag(img):
    """
        记录目标TAG
    """
    global tag_num
    tag = img.find_apriltags(roi=roi_rec,fx=f_x, fy=f_y, cx=c_x, cy=c_y) # 默认TAG36H11
    if tag:
        # debug #
        img.draw_rectangle(tag[0][0:4], color = (255, 0, 0))
        img.draw_cross(tag[0][6], tag[0][7], color = (0, 255, 0))
        # debug #
        tag_num = tag[0][4]
        print(tag_num)

def turn_round_tag(img):
    """
        掉头TAG
    """
    tag = img.find_apriltags(roi=roi_turn_rec,fx=f_x, fy=f_y, cx=c_x, cy=c_y) # 默认TAG36H11
    if tag:
        # debug #
        img.draw_rectangle(tag[0][0:4], color = (255, 0, 0))
        img.draw_cross(tag[0][6], tag[0][7], color = (0, 255, 0))
        # debug #
        ControlFlag.turn_record_num = tag[0][4]
        print(ControlFlag.turn_record_num)

        if ControlFlag.turn_record_num == 1:
            ControlFlag.turn_round = 1      #左拐
            ControlFlag.turn_round_num = 1  #1拐弯

        elif ControlFlag.turn_record_num == 3:
            ControlFlag.turn_round = 3      #右拐
            ControlFlag.turn_round_num = 1  #1拐弯

        ControlFlag.turn_yaw = 4 #先大转弯
        ControlFlag.turn_round_is = 2 #掉头直行
        print("turn_yaw = %d" %ControlFlag.turn_yaw)

def lookfor_line(err):
    """
        巡线
    """
    ControlSpeed.speed_yaw = line_pid.pid_cal(err)
    ControlSpeed.speed_x = 100
    #print(err)
    #print(ControlSpeed.speed_yaw)
    tag_start = 0

def stop_line():
    """
        停下
    """
    ControlSpeed.speed_x = 0
    ControlSpeed.speed_yaw = 0

def gofordword_cross():
    """
        直行路口
    """
    ControlSpeed.speed_x = 100
    ControlSpeed.speed_y = 0
    ControlSpeed.speed_yaw = 0

def left_cross():
    """
        左拐路口
    """
    ControlSpeed.speed_x = 100
    ControlSpeed.speed_y = 0
    ControlSpeed.speed_yaw = -400

def right_cross():
    """
        右拐路口
    """
    ControlSpeed.speed_x = 100
    ControlSpeed.speed_y = 0
    ControlSpeed.speed_yaw = 400

def turn_round():
    """
        终点掉头
    """
    ControlSpeed.speed_x = 0
    ControlSpeed.speed_y = 0
    ControlSpeed.speed_yaw = 400

#读取2串口BUF情况 0-None 1-非空
uart1_sta = 0
uart2_sta = 0

#串口
pi2driver = PIx3(uart_num = 3, baud = 115200, bits_num = 8, parity_cal = None, stop_num = 1) #uart2
wt2pi = WTX3(uart_num = 1, baud = 115200, bits_num = 8, parity_cal = None, stop_num = 1) #imu uart1

while(True):
    clock.tick()
    img = sensor.snapshot() # Take a picture and return the image.
    img = img.replace(vflip=1,hmirror=1,transpose=0)

    # TAG #
    if tag_start == 1: #匹配TAG
       find_tag(img)
    elif tag_num == -2:  #记录TAG
       record_tag(img)
    elif ControlFlag.turn_round_is == 1:
       turn_round_tag(img)
       #print(ControlFlag.turn_round_is)
    print("%d %d %d " %(tag_start, tag_num, ControlFlag.turn_round_is))
    if tag_num != -2:
        find_blobs_line(img)
        find_endline(img)

        #------------控制--------------#
        if LineFlag.flag == 2 and ControlFlag.turn_round == 0:#停(非回时)
            stop_line()
            tag_start = 1
            #print("stop")

        elif LineFlag.flag == 1 and ControlFlag.turn_yaw == 0: #巡线1
            error_x = LineFlag.delta_x - 80
            lookfor_line(error_x)
            #print(error_x)

        elif EndFlag.endline_type == 2 and ControlFlag.turn_round_is == 0: #终点
            stop_line()
            ControlFlag.turn_round_is = 1
            #print("endline")

        #elif LineFlag.flag == 0 and ControlFlag.turn_round_num == -1: #回到最初
            #stop_line()
            #tag_num = -2 #记录TAG
            #tag_start = 0   #匹配TAG
            #ControlFlag.turn_round_num = -1
            #ControlFlag.cross_cnt = 0
            ##print("0-0")

        if ControlFlag.turn_round_is == 2:
            if LineFlag.flag == 2 and ControlFlag.turn_round != 0:#停(回时拐角)
                if ControlFlag.turn_round_num != 0:
                    ControlFlag.turn_yaw = ControlFlag.turn_round #选择拐弯
                    ControlFlag.turn_round_num = ControlFlag.turn_round_num - 1#拐弯数
            #elif LineFlag.flag == 2 and ControlFlag.turn_yaw == 0:
                #ControlFlag.turn_round = 0

        print("1=%d" %LineFlag.flag)
        print("2=%d" %ControlFlag.turn_yaw)
        #print("3=%d" %tag_num)
        print("4=%d" %ControlFlag.turn_round_is)
        print("5=%d" %ControlFlag.turn_round)
        ##------------寻找--------------#
        if ControlFlag.turn_yaw == 2:#直行路口
            if LineFlag.flag != 5: #直行转巡线---按想法，应该是!8
                gofordword_cross()
                #print("!8")
            else:
                ControlFlag.turn_yaw = 0
                #print("!8")

        if ControlFlag.turn_yaw == 1: #左拐路口
            if LineFlag.flag == 5:
                ControlFlag.cross_cnt = 1
            elif LineFlag.flag != 5 and ControlFlag.cross_cnt == 0:
                gofordword_cross()
                #print("!5")

            elif LineFlag.flag == 1:
                ControlFlag.cross_cnt = 2
            elif  LineFlag.flag != 1 and ControlFlag.cross_cnt == 1:
                left_cross()
                #print("!1")
            else:
               ControlFlag.turn_yaw = 0
               ControlFlag.cross_cnt = 0

        if ControlFlag.turn_yaw == 3: #右拐路口
            if LineFlag.flag == 5:
                ControlFlag.cross_cnt = 1
            elif LineFlag.flag != 5 and ControlFlag.cross_cnt == 0:
                gofordword_cross()

            elif LineFlag.flag == 1:
                ControlFlag.cross_cnt = 2
            elif  LineFlag.flag != 1 and ControlFlag.cross_cnt == 1:
                right_cross()
            else:
               ControlFlag.turn_yaw = 0
               ControlFlag.cross_cnt = 0

        #-------------返回-------------#
        if ControlFlag.turn_yaw == 4: #掉头
            if LineFlag.flag != 1:  #格外注意方格和down方块
                turn_round()
            else:
                ControlFlag.turn_yaw = 0

    # 速度下发 #
    #print("speedx = %d" %ControlSpeed.speed_x)
    x = int(ControlSpeed.speed_x)
    y = int(ControlSpeed.speed_y)
    yaw= int(ControlSpeed.speed_yaw)
    #负数处理
    if x<0 :
        x = int(hex(x & 0xFFFF))
    if y<0 :
        y = int(hex(y & 0xFFFF))
    if yaw<0 :
        yaw = int(hex(yaw & 0xFFFF))
    #print(x)

    #取高低字节
    x_l = int(x % 256) #低
    x_h = int(x / 256) #高
    y_l = int(y % 256)
    y_h = int(y / 256)
    yaw_l = int(yaw % 256)
    yaw_h = int(yaw / 256)

    #数据至少6byte,hex格式
    output_uchar = [0,0,0,0,0,0]  #short
    output_uchar[0] = x_l
    output_uchar[1] = x_h
    output_uchar[2] = y_l
    output_uchar[3] = y_h
    output_uchar[4] = yaw_l
    output_uchar[5] = yaw_h
    #print('Sending:',output_uchar)
    pi2driver.pi2chassic(output_uchar,11)#code:11-speed 12-pid 13-roboparam







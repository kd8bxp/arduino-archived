When you connect to the robot car's access point UCBOT_630C, 
you can control the status of the car by typing the following command in your browser.

(1)View video streams
http://192.168.4.1:2001/stream

(2)Capture one picture
http://192.168.4.1/capture

(3)Set the motor speed
http://192.168.4.1/control?var=motor_speed&val=0/.../100

(4)Set the motor direction 
http://192.168.4.1/control?var=motor_direction&val=1/2.../9

(5)Set the motor calibration value
http://192.168.4.1/control?var=motor_calibration&val=-20/.../20

(6)Set the RGB color led value
http://192.168.4.1/control?var=led_r&val=0 /.../255
http://192.168.4.1/control?var=led_g&val=0 /.../255
http://192.168.4.1/control?var=led_b&val=0 /.../255

(7)play one note
http://192.168.4.1/control?var=note&val=0/1/2/3/4...21
low pitch: 8-14
middle pitch: 1-7
high pitch: 15-21

(8)Play onboard music
http://192.168.4.1/control?var=music&val=0/1/2/3/4
0：Stop music
1: Happy birthday
2: Merry christmas
3: Jingle bells
4: Song of joy

(9)Automatic avoidance mode
http://192.168.4.1/control?var=avoid_mode&val=0/1
0：disable 1:enable

(10)Automatic follow mode
http://192.168.4.1/control?var=follow_mode&val=0/1
0：disable 1:enable

(11)Tracking mode
http://192.168.4.1/control?var=trace_mode&val=0/1
0：disable 1:enable




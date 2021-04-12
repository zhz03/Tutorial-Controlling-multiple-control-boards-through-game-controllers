import pygame
import serial

pygame.init()
pygame.joystick.init()
done = False

PORT = "COM5"  

serial_port = serial.Serial(PORT,115200)
serial_port.close()
serial_port.open()

control = 0
prev_control = control
PWM = 1
x_horizontal = 0
y_horizontal = 0
x_vertical = 0
y_vertical = 0
eps = 1e-3

while not done:
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			done = True
	
	joystick = pygame.joystick.Joystick(0)
	joystick.init()
	axes = joystick.get_numaxes()
	

	# x_horizontal_curr = round(joystick.get_axis(0))
	axis0 = float(joystick.get_axis(0))
	if ((axis0 +1 ) < eps):
		x_horizontal_curr = 0
	elif (axis0>-0.99 and axis0 <-0.2):
		x_horizontal_curr = 1
	elif ((axis0 - 0) < eps):
		x_horizontal_curr = 2
	elif (axis0>0.2 and axis0 < 0.99):
		x_horizontal_curr = 3
	elif ((axis0 - 1)< eps):
		x_horizontal_curr = 4
	y_horizontal_curr = round(joystick.get_axis(1))

	x_vertical_curr = round(joystick.get_axis(3))
	y_vertical_curr = round(joystick.get_axis(4))


	control_press_A = joystick.get_button(0)
	control_press_B = joystick.get_button(1)
	control_press_X = joystick.get_button(2)
	control_press_Y = joystick.get_button(3)
	control_val = control_press_A * 1 + control_press_B * 2 + control_press_X * 3 + control_press_Y * 4
        
        
	#PWM_L2_R2 = round(joystick.get_axis(2))
	#if PWM_L2_R2 < 0:
	#	PWM_L2_R2 = 0
	PWM_L1 = joystick.get_button(4)
	PWM_R1 = joystick.get_button(5)
	#PWM_val = PWM_L2_R2 * 1 + PWM_L1 * 3 + PWM_R1 * 4
	PWM_val = PWM_L1 * 3 + PWM_R1 * 4


	if x_horizontal != x_horizontal_curr or y_horizontal != y_horizontal_curr or x_vertical != x_vertical_curr or y_vertical != y_vertical_curr or (PWM_val != PWM and PWM_val != 0) or (control_val == 0 and prev_control != 0):
		x_horizontal = x_horizontal_curr
		y_horizontal = y_horizontal_curr
		x_vertical = x_vertical_curr
		y_vertical = y_vertical_curr
		control = prev_control if prev_control!= 0 else 0
		PWM = PWM_val if PWM_val != 0 else PWM
		
		value = str(x_horizontal) + str(y_horizontal+1) + str(x_vertical+1) + str(y_vertical+1) + str(control) + str(PWM)
		serial_port.write(value.encode())
		print("VAL:", value)
		print("X_horizontal : {}".format(str(x_horizontal).encode()))
		print("Y_horizontal : {}".format(str(y_horizontal+1).encode()))
		print("X_vertical : {}".format(str(x_vertical+1).encode()))
		print("Y_vertical : {}".format(str(y_vertical+1).encode()))
		print("Control : {}".format(str(control).encode()))
		print("PWM : {}".format(str(PWM).encode()))
		
	prev_control = control_val

pygame.quit()

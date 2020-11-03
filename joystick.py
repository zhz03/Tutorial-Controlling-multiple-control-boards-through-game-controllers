import pygame
import serial

pygame.init()
pygame.joystick.init()
done = False

serial_port = serial.Serial("COM9",115200)
serial_port.close()
serial_port.open()

press_prev = 0 
control = 0
x = 0
y = 0


while not done:
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			done = True
	
	joystick = pygame.joystick.Joystick(0)
	joystick.init()
	axes = joystick.get_numaxes()
	x_curr = round(joystick.get_axis(0))
	y_curr = round(joystick.get_axis(1))
	control_press = joystick.get_button(5)
	if x != x_curr or y != y_curr or (control_press == 0 and press_prev == 1):
		x = x_curr
		y = y_curr
		if press_prev:
			control = (control + 1) % 3
		value = str(x+1) + str(y+1) + str(control)
		serial_port.write(value.encode())
		#serial_port.write(str(y + 1).encode())
		#serial_port.write(str(control).encode())
		#print(len(str(x+1).encode()))
		#print(len(str(x+1).encode()))
		#print(len(str(control).encode()))
		print("value: {}".format(value))
		print("X: {}".format(str(x + 1).encode()))
		#print("Y: {}".format(str(y + 1).encode()))
		#print("Control: {}".format(str(control).encode()))
	press_prev = control_press
pygame.quit()
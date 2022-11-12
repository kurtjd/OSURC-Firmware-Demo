import modbus

node = modbus.Node('/dev/ttyACM0', 1, 115200)

while 1:
    led = input("Enter LED to toggle [G, Y, R]: ")
    node.write(512, [led])

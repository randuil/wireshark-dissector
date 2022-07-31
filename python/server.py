import socket
   
   def pretty_print(text, colour):
       print("\033[38;5;"+str(colour)+"m"+text+"\033[0;0m", end="") ## Andsi colours
   
   server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
   server.bind(('192.168.0.2', 1234))
   
   while True:
       data, address = server.recvfrom(2024)
       print(f'connect from: {address}')
       header = data[:8].hex()
       print('\tHeader:', end="")
       pretty_print(header[:2],220)
       pretty_print(header[2:4],225)
       pretty_print(header[4:6],117)
       pretty_print(header[6:8],196)
       pretty_print(header[8:16],141)
       pretty_print(header[16:],172)
       print(f"\n\tData: \033[38;5;123m {data[8:].hex()}\033[0;0m\n")

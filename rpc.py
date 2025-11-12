import xmlrpc.client

def menuDriven():
    flag = True
    proxy = xmlrpc.client.ServerProxy("http://localhost:8000/RPC2")
    while flag:
        print("\nMenu:")
        print("1. Add two numbers")
        print("2. Subtract two numbers")
        print("3. Exit")
        choice = input("Enter your choice: ")

        if choice == '1':
            x = float(input("Enter first number: "))
            y = float(input("Enter second number: "))
            result = proxy.add(x, y)
            print(f"The result of addition is: {result}")
        elif choice == '2':
            x = float(input("Enter first number: "))
            y = float(input("Enter second number: "))
            result = proxy.subtract(x, y)
            print(f"The result of subtraction is: {result}")
        elif choice == '3':
            flag = False
            print("Exiting the menu.")
        else:
            print("Invalid choice. Please try again.")


def main():
    menuDriven()

if __name__ == "__main__":
    main()





from xmlrpc.server import *

class MathOperations:
    def add(self, x, y):
        return x + y

    def subtract(self, x, y):
        return x - y

class RequestHandler(SimpleXMLRPCRequestHandler):
    rpc_paths = ('/RPC2',)

def main():
    server = SimpleXMLRPCServer(('localhost', 8000), requestHandler=RequestHandler)
    server.register_instance(MathOperations())

    print("RPC Server is listening on port 8000...")
    server.serve_forever()

if __name__ == "__main__":
    main()



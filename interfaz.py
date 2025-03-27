import tkinter as tk
from tkinter import ttk, messagebox
import serial
import serial.tools.list_ports
from threading import Thread
import queue

class SerialInterface:
    def __init__(self, root):
        self.root = root
        self.root.title("Control Arduino/ESP32")
        self.root.geometry("600x400")
        
        # Variables
        self.serial_connection = None
        self.serial_queue = queue.Queue()
        self.running = False
        self.data_values = [tk.StringVar(), tk.StringVar(), tk.StringVar()]
        self.led_states = [tk.BooleanVar(), tk.BooleanVar(), tk.BooleanVar()]
        
        # Configurar la interfaz
        self.setup_ui()
        
        # Iniciar hilo para lectura serial
        self.start_serial_thread()
        
    def setup_ui(self):
        # Frame de conexión
        connection_frame = ttk.LabelFrame(self.root, text="Conexión Serial", padding=10)
        connection_frame.grid(row=0, column=0, padx=10, pady=5, sticky="ew")
        
        # Puerto COM
        ttk.Label(connection_frame, text="Puerto:").grid(row=0, column=0)
        self.port_combobox = ttk.Combobox(connection_frame, values=self.get_serial_ports())
        self.port_combobox.grid(row=0, column=1, padx=5)
        if self.port_combobox['values']:
            self.port_combobox.current(0)
        
        # Baudrate
        ttk.Label(connection_frame, text="Baudrate:").grid(row=0, column=2, padx=(10,0))
        self.baudrate_combobox = ttk.Combobox(connection_frame, values=[9600, 19200, 38400, 57600, 115200])
        self.baudrate_combobox.grid(row=0, column=3, padx=5)
        self.baudrate_combobox.current(0)
        
        # Botones de conexión
        self.connect_button = ttk.Button(connection_frame, text="Conectar", command=self.connect_serial)
        self.connect_button.grid(row=0, column=4, padx=5)
        self.disconnect_button = ttk.Button(connection_frame, text="Desconectar", command=self.disconnect_serial, state=tk.DISABLED)
        self.disconnect_button.grid(row=0, column=5, padx=5)
        
        # Frame de datos recibidos
        data_frame = ttk.LabelFrame(self.root, text="Datos Recibidos", padding=10)
        data_frame.grid(row=1, column=0, padx=10, pady=5, sticky="ew")
        
        # Etiquetas para mostrar datos
        for i in range(3):
            ttk.Label(data_frame, text=f"Dato {i+1}:").grid(row=0, column=i*2, padx=5)
            ttk.Label(data_frame, textvariable=self.data_values[i], width=15, relief="sunken", padding=5).grid(
                row=0, column=i*2+1, padx=5)
        
        # Frame de control de LEDs
        leds_frame = ttk.LabelFrame(self.root, text="Control de LEDs", padding=10)
        leds_frame.grid(row=2, column=0, padx=10, pady=5, sticky="ew")
        
        # Botones para controlar LEDs
        for i in range(3):
            chk = ttk.Checkbutton(leds_frame, text=f"LED {i+1}", variable=self.led_states[i], 
                                 command=lambda idx=i: self.toggle_led(idx))
            chk.grid(row=0, column=i, padx=10, pady=5)
        
        # Consola de mensajes
        console_frame = ttk.LabelFrame(self.root, text="Consola", padding=10)
        console_frame.grid(row=3, column=0, padx=10, pady=5, sticky="nsew")
        
        self.console_text = tk.Text(console_frame, height=8, state=tk.DISABLED)
        self.console_text.pack(fill=tk.BOTH, expand=True)
        
        # Configurar grid para expansión
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(3, weight=1)
    
    def get_serial_ports(self):
        """Obtiene la lista de puertos seriales disponibles"""
        ports = serial.tools.list_ports.comports()
        return [port.device for port in ports]
    
    def connect_serial(self):
        """Establece la conexión serial"""
        port = self.port_combobox.get()
        baudrate = int(self.baudrate_combobox.get())
        
        try:
            self.serial_connection = serial.Serial(port, baudrate, timeout=1)
            self.running = True
            self.connect_button.config(state=tk.DISABLED)
            self.disconnect_button.config(state=tk.NORMAL)
            self.log_message(f"Conectado a {port} a {baudrate} baudios")
        except Exception as e:
            messagebox.showerror("Error", f"No se pudo conectar: {str(e)}")
    
    def disconnect_serial(self):
        """Cierra la conexión serial"""
        self.running = False
        if self.serial_connection and self.serial_connection.is_open:
            self.serial_connection.close()
        self.connect_button.config(state=tk.NORMAL)
        self.disconnect_button.config(state=tk.DISABLED)
        self.log_message("Desconectado")
    
    def toggle_led(self, led_num):
        """Envía comando para encender/apagar un LED"""
        if self.serial_connection and self.serial_connection.is_open:
            state = "1" if self.led_states[led_num].get() else "0"
            command = f"LED{led_num+1}={state}\n"
            try:
                self.serial_connection.write(command.encode())
                self.log_message(f"Enviado: {command.strip()}")
            except Exception as e:
                self.log_message(f"Error al enviar comando: {str(e)}")
    
    def log_message(self, message):
        """Agrega un mensaje a la consola"""
        self.console_text.config(state=tk.NORMAL)
        self.console_text.insert(tk.END, message + "\n")
        self.console_text.see(tk.END)
        self.console_text.config(state=tk.DISABLED)
    
    def start_serial_thread(self):
        """Inicia el hilo para lectura serial"""
        self.serial_thread = Thread(target=self.read_serial_data, daemon=True)
        self.serial_thread.start()
        self.root.after(100, self.process_serial_queue)
    
    def read_serial_data(self):
        """Lee datos del puerto serial (ejecutado en un hilo separado)"""
        while True:
            if self.running and self.serial_connection and self.serial_connection.is_open:
                try:
                    line = self.serial_connection.readline().decode('utf-8').strip()
                    if line:
                        self.serial_queue.put(line)
                except Exception as e:
                    self.log_message(f"Error de lectura: {str(e)}")
                    self.running = False
    
    def process_serial_queue(self):
        """Procesa los datos recibidos del puerto serial"""
        try:
            while not self.serial_queue.empty():
                data = self.serial_queue.get_nowait()
                self.parse_serial_data(data)
        except queue.Empty:
            pass
        
        self.root.after(100, self.process_serial_queue)
    
    def parse_serial_data(self, data):
        """Interpreta los datos recibidos y actualiza la interfaz"""
        self.log_message(f"Recibido: {data}")
        
        # Ejemplo de formato esperado: "D1=123,D2=456,D3=789"
        try:
            parts = data.split(',')
            for part in parts:
                if '=' in part:
                    key, value = part.split('=')
                    if key.startswith('D') and key[1:].isdigit():
                        idx = int(key[1:]) - 1
                        if 0 <= idx < 3:
                            self.data_values[idx].set(value)
        except Exception as e:
            self.log_message(f"Error al procesar datos: {str(e)}")

if __name__ == "__main__":
    root = tk.Tk()
    app = SerialInterface(root)
    root.mainloop()
import tkinter as tk
from tkinter import font
import threading
import time
from pymodbus.client import ModbusSerialClient
from pymodbus.payload import BinaryPayloadDecoder
from pymodbus.constants import Endian

# ==========================================
# CẤU HÌNH GIAO TIẾP
# ==========================================
PORT = 'COM10'  # Thay đổi cổng COM cho đúng
BAUDRATE = 9600
SLAVE_ID = 45

# Cấu hình Endianness cho Float
BYTE_ORDER = Endian.BIG
WORD_ORDER = Endian.LITTLE

# ==========================================
# BIẾN TOÀN CỤC CHỨA DỮ LIỆU (Shared Data)
# ==========================================
modbus_data = {
    "status": "Đang kết nối...",
    "imu": 0.0,
    "relays": [0, 0, 0],         # Pre Valve, Coa Valve, Clutch
    "buttons": [0, 0, 0, 0, 0]   # Manual, Azi Up, Azi Dw, Ele Up, Ele Dw
}

is_running = True # Cờ điều khiển luồng

# ==========================================
# LUỒNG XỬ LÝ MODBUS (CHẠY NGẦM)
# ==========================================
def modbus_polling_thread():
    client = ModbusSerialClient(
        port=PORT, baudrate=BAUDRATE, timeout=0.5, 
        stopbits=1, bytesize=8, parity='N'
    )
    
    if not client.connect():
        modbus_data["status"] = f"❌ LỖI: KHÔNG MỞ ĐƯỢC {PORT}"
        return

    modbus_data["status"] = "✅ Đã kết nối STM32"

    while is_running:
        try:
            # 1. Đọc IMU (Địa chỉ 56, 2 thanh ghi)
            res_imu = client.read_holding_registers(address=56, count=2, slave=SLAVE_ID)
            if not res_imu.isError():
                decoder = BinaryPayloadDecoder.fromRegisters(res_imu.registers, byteorder=BYTE_ORDER, wordorder=WORD_ORDER)
                modbus_data["imu"] = decoder.decode_32bit_float()
            
            # 2. Đọc Relay (Địa chỉ 87, đọc liền 3 thanh ghi: 87, 88, 89)
            res_relay = client.read_holding_registers(address=87, count=3, slave=SLAVE_ID)
            if not res_relay.isError():
                modbus_data["relays"] = res_relay.registers
                
            # 3. Đọc Nút nhấn (Địa chỉ 90, đọc liền 5 thanh ghi: 90->94)
            res_btn = client.read_holding_registers(address=90, count=5, slave=SLAVE_ID)
            if not res_btn.isError():
                modbus_data["buttons"] = res_btn.registers

            modbus_data["status"] = "✅ Đang truyền dữ liệu..."
            
        except Exception as e:
            modbus_data["status"] = "⚠️ Lỗi truyền thông!"
            
        # time.sleep(0.01) # Tốc độ quét 100ms/lần

    client.close()

# ==========================================
# GIAO DIỆN NGƯỜI DÙNG (GUI - TKINTER)
# ==========================================
def update_gui():
    """Hàm này tự động gọi lại mỗi 200ms để cập nhật giao diện"""
    if not is_running:
        return
        
    # Cập nhật Trạng thái kết nối
    lbl_status.config(text=modbus_data["status"], fg="green" if "✅" in modbus_data["status"] else "red")
    
    # Cập nhật IMU
    lbl_imu_val.config(text=f"{modbus_data['imu']:.2f}°")
    
    # Cập nhật Relay (0=Đỏ, 1=Xanh lá)
    relays = modbus_data["relays"]
    lbl_pre_valve.config(bg="lime" if relays[0] else "tomato", text="ON" if relays[0] else "OFF")
    lbl_coa_valve.config(bg="lime" if relays[1] else "tomato", text="ON" if relays[1] else "OFF")
    lbl_clutch.config(bg="lime" if relays[2] else "tomato", text="ON" if relays[2] else "OFF")
    
    # Cập nhật Buttons
    btns = modbus_data["buttons"]
    lbl_btn_man.config(bg="lime" if btns[0] else "lightgray")
    lbl_btn_aup.config(bg="lime" if btns[1] else "lightgray")
    lbl_btn_adw.config(bg="lime" if btns[2] else "lightgray")
    lbl_btn_eup.config(bg="lime" if btns[3] else "lightgray")
    lbl_btn_edw.config(bg="lime" if btns[4] else "lightgray")

    # Đặt lịch cập nhật lại sau 200ms
    root.after(10, update_gui)

def on_closing():
    """Hàm xử lý khi bấm dấu X tắt cửa sổ"""
    global is_running
    is_running = False # Ra lệnh dừng luồng Modbus
    root.destroy()

# --- Khởi tạo Cửa sổ ---
root = tk.Tk()
root.title("CONTROL STATION - BẢNG GIÁM SÁT")
root.geometry("500x550")
root.configure(bg="#2c3e50")
root.protocol("WM_DELETE_WINDOW", on_closing)

# Font chữ
title_font = font.Font(family="Helvetica", size=16, weight="bold")
lbl_font = font.Font(family="Helvetica", size=12)
val_font = font.Font(family="Helvetica", size=24, weight="bold")

# Tiêu đề & Status
tk.Label(root, text="BẢNG GIÁM SÁT HỆ THỐNG", font=title_font, bg="#2c3e50", fg="white").pack(pady=10)
lbl_status = tk.Label(root, text="Đang khởi động...", font=lbl_font, bg="#2c3e50", fg="yellow")
lbl_status.pack()

# --- KHUNG IMU ---
frame_imu = tk.Frame(root, bg="#34495e", bd=2, relief="groove")
frame_imu.pack(pady=15, padx=20, fill="x")
tk.Label(frame_imu, text="GÓC CẢM BIẾN (IMU)", font=lbl_font, bg="#34495e", fg="cyan").pack(pady=5)
lbl_imu_val = tk.Label(frame_imu, text="0.00°", font=val_font, bg="#34495e", fg="white")
lbl_imu_val.pack(pady=10)

# --- KHUNG RELAY ---
frame_relay = tk.Frame(root, bg="#34495e", bd=2, relief="groove")
frame_relay.pack(pady=10, padx=20, fill="x")
tk.Label(frame_relay, text="TRẠNG THÁI RELAY", font=lbl_font, bg="#34495e", fg="cyan").grid(row=0, column=0, columnspan=2, pady=5)

tk.Label(frame_relay, text="Precision Valve:", font=lbl_font, bg="#34495e", fg="white").grid(row=1, column=0, sticky="e", padx=10, pady=5)
lbl_pre_valve = tk.Label(frame_relay, text="OFF", width=8, bg="tomato", font=lbl_font)
lbl_pre_valve.grid(row=1, column=1, pady=5)

tk.Label(frame_relay, text="Coarse Valve:", font=lbl_font, bg="#34495e", fg="white").grid(row=2, column=0, sticky="e", padx=10, pady=5)
lbl_coa_valve = tk.Label(frame_relay, text="OFF", width=8, bg="tomato", font=lbl_font)
lbl_coa_valve.grid(row=2, column=1, pady=5)

tk.Label(frame_relay, text="Clutch:", font=lbl_font, bg="#34495e", fg="white").grid(row=3, column=0, sticky="e", padx=10, pady=5)
lbl_clutch = tk.Label(frame_relay, text="OFF", width=8, bg="tomato", font=lbl_font)
lbl_clutch.grid(row=3, column=1, pady=5)

# --- KHUNG NÚT NHẤN ---
frame_btn = tk.Frame(root, bg="#34495e", bd=2, relief="groove")
frame_btn.pack(pady=10, padx=20, fill="x")
tk.Label(frame_btn, text="TRẠNG THÁI NÚT NHẤN", font=lbl_font, bg="#34495e", fg="cyan").pack(pady=5)

btn_container = tk.Frame(frame_btn, bg="#34495e")
btn_container.pack(pady=5)

# Tạo các ô vuông báo trạng thái nút
def create_btn_indicator(parent, text):
    frame = tk.Frame(parent, bg="#34495e")
    frame.pack(side="left", padx=10)
    lbl_color = tk.Label(frame, width=4, height=2, bg="lightgray", relief="raised")
    lbl_color.pack()
    tk.Label(frame, text=text, font=("Helvetica", 9), bg="#34495e", fg="white").pack()
    return lbl_color

lbl_btn_man = create_btn_indicator(btn_container, "Manual")
lbl_btn_aup = create_btn_indicator(btn_container, "Azi Up")
lbl_btn_adw = create_btn_indicator(btn_container, "Azi Dw")
lbl_btn_eup = create_btn_indicator(btn_container, "Ele Up")
lbl_btn_edw = create_btn_indicator(btn_container, "Ele Dw")

# ==========================================
# KHỞI ĐỘNG CHƯƠNG TRÌNH
# ==========================================
if __name__ == "__main__":
    # Khởi động luồng chạy Modbus ngầm
    modbus_thread = threading.Thread(target=modbus_polling_thread, daemon=True)
    modbus_thread.start()
    
    # Kích hoạt vòng lặp cập nhật giao diện
    update_gui()
    
    # Hiển thị cửa sổ
    root.mainloop()
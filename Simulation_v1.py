import serial
import time
import threading

# --- CẤU HÌNH CỔNG UART ---
PORT = 'COM10'  # Cổng COM của bạn
BAUDRATE = 9600

# Biến toàn cục giao tiếp giữa 2 luồng
running = True
current_payload = bytearray([0x42, 0x00, 0x45]) 
current_command_name = "KHÓA SÚNG (0x00)"

# Thời gian nghỉ giữa các lần bắn lệnh (Thay đổi số này để ép STM32 chạy nhanh hơn)
# Ví dụ: Để 0.1 là STM32 sẽ bị "spam" 10 lệnh/giây
LOOP_DELAY = 0.01

def keyboard_listener():
    """Luồng chạy ngầm: Lắng nghe phím người dùng gõ"""
    global current_payload, current_command_name, running, LOOP_DELAY
    
    print("\n" + "="*50)
    print(" BẢNG ĐIỀU KHIỂN & ĐO TỐC ĐỘ STM32 (PING TEST)")
    print("="*50)
    print("[1] + Enter: MỞ SÚNG (0x01)")
    print("[2] + Enter: KHÓA SÚNG (0x00)")
    print("[3] + Enter: CẤP 150 ĐẠN (0x96)")
    print("[4] + Enter: LỆNH ĐẶC BIỆT 6 BYTE (0x02)")
    print("[f] + Enter: Chế độ Bắn siêu tốc (0.1s/lần)")
    print("[s] + Enter: Chế độ Chậm rãi (2s/lần)")
    print("[q] + Enter: Thoát chương trình")
    print("="*50 + "\n")
    
    while running:
        try:
            val = input()
            if val == '1':
                current_payload = bytearray([0x42, 0x01, 0x45])
                current_command_name = "MỞ SÚNG (0x01)"
            elif val == '2':
                current_payload = bytearray([0x42, 0x00, 0x45])
                current_command_name = "KHÓA SÚNG (0x00)"
            elif val == '3':
                current_payload = bytearray([0x42, 0x96, 0x45])
                current_command_name = "CẤP 150 ĐẠN (0x96)"
            elif val == '4':
                current_payload = bytearray([0x42, 0x02, 0x45])
                current_command_name = "LỆNH ĐẶC BIỆT (0x02)"
            elif val.lower() == 'f':
                LOOP_DELAY = 0.1
                print("\n>>> ĐÃ BẬT CHẾ ĐỘ SPAM LỆNH (0.1s) <<<\n")
                continue
            elif val.lower() == 's':
                LOOP_DELAY = 2.0
                print("\n>>> ĐÃ BẬT CHẾ ĐỘ BÌNH THƯỜNG (2.0s) <<<\n")
                continue
            elif val.lower() == 'q':
                running = False
                print("\n[*] Đang tắt hệ thống...")
                continue
            
            print(f"\n>>> ĐÃ CHUYỂN TRẠNG THÁI: {current_command_name} <<<\n")
        except:
            pass

def main():
    global running
    try:
        # Timeout 0.5s: Nếu quá 500ms STM32 không trả lời, coi như rớt mạng
        ser = serial.Serial(PORT, BAUDRATE, timeout=0.05)
        print(f"[*] Kết nối cổng {PORT} thành công!")
        
        kb_thread = threading.Thread(target=keyboard_listener)
        kb_thread.daemon = True
        kb_thread.start()

        packet_count = 1
        
        while running:
            # Dọn sạch rác trong ống bơ trước khi bắt đầu chu kỳ mới
            ser.reset_input_buffer()
            
            # --- BẮT ĐẦU BẤM GIỜ ---
            start_time = time.perf_counter() 
            
            # 1. Gửi lệnh
            ser.write(current_payload)
            tx_hex = " ".join([f"0x{b:02X}" for b in current_payload])
            print(f"[{packet_count}] [TX] Gửi: {tx_hex} -> {current_command_name}")

            # 2. Đọc phản hồi tự động
            # Lệnh 0x02 cần lấy 6 byte, các lệnh khác chỉ lấy 1 byte
            expected_bytes = 6 if current_payload[1] == 0x02 else 1
            
            rx_data = ser.read(expected_bytes)
            
            # --- DỪNG BẤM GIỜ ---
            end_time = time.perf_counter() 
            
            # Tính toán thời gian phản hồi (mili-giây)
            latency_ms = (end_time - start_time) * 1000 
            
            if len(rx_data) > 0:
                rx_hex = " ".join([f"0x{b:02X}" for b in rx_data])
                
                # Cảnh báo nếu STM32 trả về thiếu byte
                if len(rx_data) < expected_bytes:
                    print(f"    -> [RX] LỖI THIẾU BYTE: {rx_hex} (Cần {expected_bytes}, Có {len(rx_data)}) | Thời gian: {latency_ms:.1f} ms")
                else:
                    print(f"    -> [RX] Nhận: {rx_hex} | Tốc độ phản hồi: {latency_ms:.1f} ms")
            else:
                print(f"    -> [RX] MẤT KẾT NỐI==============================================================================(Timeout > 500ms)")

            packet_count += 1
            
            # Vòng lặp chờ thông minh (giúp bấm 'q' thoát mượt hơn)
            steps = int(LOOP_DELAY / 0.1)
            for _ in range(max(1, steps)):
                if not running: break
                time.sleep(0.1)

    except serial.SerialException as e:
        print(f"\n[-] Lỗi cổng COM: {e}")
        print("Hãy rút cáp cắm lại hoặc tắt các phần mềm Terminal khác đi nhé.")
    except KeyboardInterrupt:
        running = False
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("[*] Đã đóng cổng COM an toàn.")

if __name__ == "__main__":
    main()